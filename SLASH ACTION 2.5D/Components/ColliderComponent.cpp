#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include <algorithm>  // std::min, std::maxのためのヘッダー
#include <cmath>     // fabsのためのヘッダー
#include <iostream>

using namespace DirectX;

ColliderComponent::ColliderComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("COLLIDER"); // ソート番号を設定
}

void ColliderComponent::Update()
{
	beforeColl_ab = coll_ab; // 前回のAABBの当たり判定用を保存

	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if(transform == nullptr) {
		std::cerr << "Error: ColliderComponent requires a TransformComponent." << std::endl;
		return;
	}

	transform->MakeChildWorld(); // ワールド行列を更新

	DirectX::XMFLOAT3 pos = transform->GetPosition();
	DirectX::XMFLOAT3 scale = transform->GetScale();
	DirectX::XMVECTOR rot = transform->GetQuaternion();

	DirectX::XMVECTOR childRot = transform->GetLocalQuaternion();

	SetColliderSize_AABB(pos, scale);

	// 親がいる場合はローカル回転を使用
	if (m_Object->GetParent() != nullptr) {
		SetColliderSize_OBB(pos, scale, childRot);
	}
	else {
		SetColliderSize_OBB(pos, scale, rot);
	}

	MakeWorldAABBMatrix();
	MakeWorldOBBMatrix();
}

void ColliderComponent::MakeWorldAABBMatrix() {
	XMFLOAT3 outCenter = {};
	XMFLOAT3 outSize = {};

	outCenter.x = (coll_ab.min.x + coll_ab.max.x) * 0.5f;
	outCenter.y = (coll_ab.min.y + coll_ab.max.y) * 0.5f;
	outCenter.z = (coll_ab.min.z + coll_ab.max.z) * 0.5f;

	outSize.x = (coll_ab.max.x - coll_ab.min.x) * 0.5f;
	outSize.y = (coll_ab.max.y - coll_ab.min.y) * 0.5f;
	outSize.z = (coll_ab.max.z - coll_ab.min.z) * 0.5f;

	// クォータニオン作成
	const XMVECTOR quat = XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

	// SRT情報作成
	const XMMATRIX r = XMMatrixRotationQuaternion(quat);
	const XMVECTOR scale = XMVectorSet(outSize.x, outSize.y, outSize.z, 1.0f);
	const XMMATRIX s = XMMatrixScalingFromVector(scale);
	const XMVECTOR pos = XMVectorSet(outCenter.x, outCenter.y, outCenter.z, 1.0f);
	const XMMATRIX t = XMMatrixTranslationFromVector(pos);

	// ワールド行列を作成し、保存
	coll_ab.worldAABBMatrix = s * r * t;
}

void ColliderComponent::MakeWorldOBBMatrix() {

	// SRT情報作成
	const XMMATRIX r = XMMatrixRotationQuaternion(coll_ob.rotation);
	const XMVECTOR scale = XMVectorSet(coll_ob.size.x, coll_ob.size.y, coll_ob.size.z, 1.0f);
	const XMMATRIX s = XMMatrixScalingFromVector(scale);
	const XMVECTOR pos = XMVectorSet(coll_ob.center.x, coll_ob.center.y, coll_ob.center.z, 1.0f);
	const XMMATRIX t = XMMatrixTranslationFromVector(pos);

	// ワールド行列を作成し、保存
	coll_ob.worldOBBMatrix = s * r * t;
}

//==================================
// ■CheckHit関数
// 線(無限の長さ)と平面(無限の大きさ)の当たり判定
//==================================
bool ColliderComponent::CheckHit(const Line& line, const PlaneNormal& plane)
{
	// 無限の線が無限の平面にぶつからないのは線と平面が平行な時のみ。
	// 線の方向ベクトルと平面の法線ベクトルが垂直(→内積が0)かどうかをチェックする
	if (Dot((plane.point - line.point), plane.normal) == 0 || Dot(line.vec, plane.normal) != 0)
	{
		return true;
	}
	return false;
}

//==================================
// ■CheckHit関数
// 線分と平面(無限の大きさ)の当たり判定
//==================================
bool ColliderComponent::CheckHit(const Segment& segment, const PlaneNormal& plane)
{
	if (Dot((segment.start - plane.point), plane.normal) * Dot((segment.end - plane.point), plane.normal) <= 0)
	{
		return true;
	}
	return false;
}

//==================================
// ■CheckHit関数
// 線(無限の長さ)とポリゴンの当たり判定
//==================================
bool ColliderComponent::CheckHit(const Line& line, const TrianglePolygon& polygon)
{
	XMFLOAT3 p;
	return CheckHit(line, polygon, p);
}
bool ColliderComponent::CheckHit(const Line& line, const TrianglePolygon& polygon, DirectX::XMFLOAT3& contact)
{
	// 三角形の法線を計算
	XMFLOAT3 normal = GetNormal(polygon);

	// 線と法線が平行である場合
	float denom = Dot(normal, line.vec);
	if (fabs(denom) < 1e-6f) {
		return false; // 交差なし
	}

	// 線上の交点を計算
	float d = Dot(normal, polygon.p0);
	float t = (d - Dot(normal, line.point)) / denom;
	contact = line.point + line.vec * t;

	// 三角形内に交点があるかを判定
	return PointInTriangle(contact, polygon);
}

//==================================
// ■CheckHit関数
// 線分とポリゴンの当たり判定
//==================================
bool ColliderComponent::CheckHit(const Segment& segment, const TrianglePolygon& polygon)
{
	XMFLOAT3 p;
	return CheckHit(segment, polygon, p);
}
bool ColliderComponent::CheckHit(const Segment& segment, const TrianglePolygon& polygon, XMFLOAT3& contact)
{
	PlaneNormal plane(polygon.p0, GetNormal(polygon));

	if (CheckHit(segment, plane))
	{
		// 線分と平面の交点位置を計算
		float denom = Dot(plane.normal, segment.end - segment.start);
		float t = Dot(plane.normal, plane.point - segment.start) / denom;

		// tが0から1の範囲内であれば線分と平面が交差する
		if (t >= 0.0f && t <= 1.0f) {
			contact = segment.start + (segment.end - segment.start) * t; // 交点を計算

			// 交点が三角形ポリゴンの内側にあるか判定
			return PointInTriangle(contact, polygon);
		}
	}
	return false;

}

//==================================
// ■CheckHit関数
// 球体と平面の当たり判定
//==================================
bool ColliderComponent::CheckHit(const Sphere& sphere, const PlaneNormal& plane)
{
	// 球体の中心から平面への距離を計算
	float distance = DistancePointToPlane(sphere.center, plane);

	// 球体の半径以内なら衝突
	return (distance <= sphere.radius);
}

//==================================
// ■CheckHit関数
// 球体とポリゴンの当たり判定
//==================================
bool ColliderComponent::CheckHit(const Sphere& sphere, const TrianglePolygon& polygon)
{
	XMFLOAT3 p;
	return CheckHit(sphere, polygon, p);
}
bool ColliderComponent::CheckHit(const Sphere& sphere, const TrianglePolygon& polygon, XMFLOAT3& contact)
{
	PlaneNormal plane(polygon.p0, GetNormal(polygon));

	// 球体の中心から平面への距離を計算
	float distance = DistancePointToPlane(sphere.center, plane);

	// 球体の半径以内なら
	if (distance <= sphere.radius)
	{
		// 線分と平面の交点を計算
		contact = ProjectPointToPlane(sphere.center, plane);

		// 交点が三角形ポリゴンの内側にあるか判定
		if (PointInTriangle(contact, polygon))
		{
			return true;
		}

		// 交点が三角形ポリゴンの外側なら各辺との距離を計算
		if (DistancePointToSegment(sphere.center, { polygon.p0 , polygon.p1 }, contact) <= sphere.radius)
		{
			return true;
		}

		if (DistancePointToSegment(sphere.center, { polygon.p1 , polygon.p2 }, contact) <= sphere.radius)
		{
			return true;
		}

		if (DistancePointToSegment(sphere.center, { polygon.p2 , polygon.p0 }, contact) <= sphere.radius)
		{
			return true;
		}
	}

	return false;
}

////==================================
//// ■CheckHit関数
//// 円柱と平面の当たり判定
////==================================
//bool CheckHit(const Cylinder& cylinder, const Plane& plane)
//{
//	// 円柱の両端を平面の法線に投影し、その距離を計算
//	float distanceToTop = Dot((cylinder.top - plane.point), plane.normal);
//	float distanceToBottom = Dot((cylinder.bottom - plane.point), plane.normal);

//	// 円柱の軸の端点が平面の反対側にあるかどうか
//	if ((distanceToTop * distanceToBottom) <= 0.0f)
//	{
//		return true; // 軸が平面を横切る
//	}

//	// 円柱の半径を考慮した交差判定
//	float distanceToAxis = fabs(distanceToTop);
//	return distanceToAxis <= cylinder.radius;
//}

////==================================
//// ■CheckHit関数
//// 円柱とポリゴンの当たり判定
////==================================
//bool CheckHit(const Cylinder& cylinder, const Polygon& polygon)
//{
//	Vector3 p;
//	return CheckHit(cylinder, polygon, p);
//}
//bool CheckHit(const Cylinder& cylinder, const Polygon& polygon, Vector3& contact)
//{
//	Vector3 axis = cylinder.top - cylinder.bottom;

//	// 平面上での三角形の3頂点の射影を計算
//	Vector3 projP0 = polygon.p0 - (axis * axis.Dot(polygon.p0 - cylinder.bottom));
//	Vector3 projP1 = polygon.p1 - (axis * axis.Dot(polygon.p1 - cylinder.bottom));
//	Vector3 projP2 = polygon.p2 - (axis * axis.Dot(polygon.p2 - cylinder.bottom));

//	// 半径の二乗を計算
//	float radiusSq = cylinder.radius * cylinder.radius;

//	// 底面と三角形の最寄点を計算
//	Vector3 closestToBottom = ClosestPointOnTriangle(cylinder.bottom, { projP0, projP1, projP2 });
//	if ((closestToBottom - cylinder.bottom).LengthSquared() <= radiusSq) {
//		contact = closestToBottom;
//		return true;
//	}

//	// 上面と三角形の最寄点を計算
//	Vector3 closestToTop = ClosestPointOnTriangle(cylinder.top, { projP0, projP1, projP2 });
//	if ((closestToTop - cylinder.top).LengthSquared() <= radiusSq) {
//		contact = closestToTop;
//		return true;
//	}

//	return false;
//}


////==================================
//// ■CheckHit関数
//// カプセルと平面の当たり判定
////==================================
//bool CheckHit(const Capsule& capsule, const Plane& plane)
//{
//	// カプセルの中心線（top と bottom）を定義
//	Vector3 capsuleDirection = capsule.bottom - capsule.top;

//	float distanceToTop = Dot((capsule.top - plane.point), plane.normal);
//	float distanceToBottom = Dot((capsule.bottom - plane.point), plane.normal);

//	// 中心線の両端が平面の反対側にある場合、交差している
//	if (distanceToTop * distanceToBottom <= 0.0f) {
//		return true;
//	}

//	// 両端が平面の同じ側にある場合、カプセルの半径を考慮
//	float distClosest = min(fabs(distanceToTop), fabs(distanceToBottom));
//	return distClosest <= capsule.radius;
//}

////==================================
//// ■CheckHit関数
//// カプセルとポリゴンとポリゴンの当たり判定
////==================================
//bool CheckHit(const Capsule& capsule, const Polygon& polygon)
//{
//	Vector3 p;
//	return CheckHit(capsule, polygon, p);
//}
//bool CheckHit(const Capsule& capsule, const Polygon& polygon, Vector3& contact)
//{
//	// 線分とポリゴンの当たり判定
//	Segment seg = { capsule.top, capsule.bottom };
//	if (CheckHit(seg, polygon, contact))
//	{
//		return true;
//	}

//	// 球体とポリゴンの当たり判定
//	Sphere sphere = { capsule.top, capsule.radius };
//	if (CheckHit(sphere, polygon, contact))
//	{
//		return true;
//	}
//	sphere = { capsule.bottom, capsule.radius };
//	if (CheckHit(sphere, polygon, contact))
//	{
//		return true;
//	}

//	// 各三角形のエッジ
//	Vector3 edges[3][2] = {
//		{polygon.p0, polygon.p1},
//		{polygon.p1, polygon.p2},
//		{polygon.p2, polygon.p0}
//	};

//	// カプセル線分と各エッジの最近接点を計算
//	for (const auto& edge : edges) {
//		Vector3 edgeClosest = ClosestPointOnSegment(capsule.top, { edge[0], edge[1] });
//		Vector3 capsuleClosest = ClosestPointOnSegment(edgeClosest, { capsule.top, capsule.bottom });
//		if ((edgeClosest - capsuleClosest).LengthSquared() <= capsule.radius * capsule.radius)
//		{
//			contact = edgeClosest;
//			return true;
//		}
//	}

//	return false;
//}

//==================================
// ■CheckHit関数
// 球体と球体の当たり判定
//==================================
bool ColliderComponent::CheckHit(const Sphere& sphere1,const Sphere& sphere2)
{
	XMFLOAT3 p;
	return CheckHit(sphere1, sphere2, p);
}

bool ColliderComponent::CheckHit(const Sphere& s1, const Sphere& s2, XMFLOAT3& contact)
{
	// center をベクトル化
	const XMVECTOR c1 = XMLoadFloat3(&s1.center);
	const XMVECTOR c2 = XMLoadFloat3(&s2.center);

	// 差分ベクトル
	const XMVECTOR v = XMVectorSubtract(c1, c2);

	// 距離の二乗
	const float dist2 = XMVectorGetX(XMVector3LengthSq(v));

	// 半径の和の二乗
	const float r = s1.radius + s2.radius;
	const float r2 = r * r;

	if (dist2 < r2)
	{
		// 正規化（方向ベクトル）
		const XMVECTOR n = XMVector3Normalize(v);

		// 接触点 = sphere2.center + n * sphere2.radius
		const XMVECTOR contactPos = XMVectorMultiplyAdd(
			n,
			XMVectorReplicate(s2.radius),
			c2
		);

		XMStoreFloat3(&contact, contactPos);
		return true;
	}

	return false;
}

//==================================
// ■CheckHit関数
// AABBとAABBの当たり判定
//==================================
bool ColliderComponent::CheckHit_CubeAndCube_IsTrigger3D(const ColliderComponent& p1, const ColliderComponent& p2) {
	if (p1.m_activeColliderFlag == false || p2.m_activeColliderFlag == false) {
		return false; // コライダーが無効な場合は衝突しない
	}

	const AABB coll1 = p1.coll_ab;
	const AABB coll2 = p2.coll_ab;

	// X座標
	if (coll1.max.x < coll2.min.x) {
		return false;
	}

	if (coll1.min.x > coll2.max.x) {
		return false;
	}

	// Y座標
	if (coll1.max.y < coll2.min.y) {
		return false;
	}

	if (coll1.min.y > coll2.max.y) {
		return false;
	}

	// Z座標
	if (coll1.max.z < coll2.min.z) {
		return false;
	}

	if (coll1.min.z > coll2.max.z) {
		return false;
	}

	return true;
}

//==================================
// ■CheckHit関数
// AABBとAABBの当たり判定（2D用）
// 戻しアリ
//==================================
bool ColliderComponent::CheckHit_CubeAndCube_NoTrigger2D(const ColliderComponent& p1, const ColliderComponent& p2, DirectX::XMFLOAT3& pos) {

	const AABB coll1 = p1.coll_ab;
	const AABB coll2 = p2.coll_ab;

	// そもそも衝突しているかどうかのチェック
	// 奥行きからの敵の登場のことも考えて、念のため
	// Z軸も考慮した当たり判定とする
	// 当たって押し出されるのはX軸とY軸みたいな
	bool check = CheckHit_CubeAndCube_IsTrigger3D(p1, p2);

	if (check == true) {
		// 最終的のオブジェクトを押し戻すためのベクトル
		DirectX::XMFLOAT3 pushBack(0.0f, 0.0f, 0.0f);

		// X軸での「めり込み量」を計算
		const float dx1 = (coll1.max.x - coll2.min.x);	// p1の右端 - p2の左端（p2が左から押し込んでいる時）
		const float dx2 = (coll1.min.x - coll2.max.x);	// p1の左端 - p2の右端（p2が右から押し込んでいる時）

		// Y軸での「めり込み量」を計算
		const float dy1 = (coll1.max.y - coll2.min.y);	// p1の上端 - p2の下端（Aが上から押し込んでいる時）
		const float dy2 = (coll1.min.y - coll2.max.y);	// p1の下端 - p2の上端（Aが下から押し込んでいる時）

		// X軸の処理
		// X軸方向でどちらに押し戻すべきか（小さい方が自然）
		if (fabsf(dx1) < fabsf(dx2)) {
			pushBack.x = dx1;	// 左から来たので右に押し戻し
		}
		else {
			pushBack.x = dx2;	// 右から来たので左に押し戻し
		}

		// Y軸の処理
		// Y軸方向でどちらに押し戻すべきか（小さい方が自然）
		if (fabsf(dy1) < fabsf(dy2)) {
			pushBack.y = dy1;	// 下から来たので上に押し戻し
		}
		else {
			pushBack.y = dy2;	// 上から来たので下に押し戻し
		}

		// Z軸は完全に無視（2.5Dのため）
		pushBack.z = 0.0f;

		// X軸とY軸のうち、より「めり込みが少ない」軸でのみ押し戻す
		// →壁にぶつかったとき、斜めではなく「垂直な方向」だけで修正されるようにするため
		if (fabsf(pushBack.x) < fabsf(pushBack.y)) {
			pushBack.y = 0.0f;	// 横方向で押し戻す
		}
		else {
			pushBack.x = 0.0f;	// 縦方向で押し戻す
		}

		// オブジェクトの位置を押し戻しベクトル分だけ移動させて、衝突によるめり込みを解消する
		pos.x += pushBack.x;
		pos.y += pushBack.y;
		pos.z += 0.0f;	// 2.5DなのでZ軸は無視
	}

	// 衝突しているかどうかを返す
	return check;
}

//==================================
// ■CheckHit関数
// AABBとAABBの当たり判定（2D用）
// どこから当たっているのか含む
// 戻しナシ
//==================================
bool ColliderComponent::CheckHit_CubeAndCube_IsTrigger2D_Normal(const ColliderComponent& p1, const ColliderComponent& p2, XMFLOAT3& hitNormal) {
	const AABB coll1 = p1.coll_ab;
	const AABB coll2 = p2.coll_ab;

	bool hit = CheckHit_CubeAndCube_IsTrigger3D(p1, p2);

	if (hit == false) {
		// 衝突していない場合は法線をゼロベクトルに設定
		hitNormal = { 0.0f, 0.0f, 0.0f };
		return false;
	}

	// 最終的のオブジェクトを押し戻すためのベクトル
	DirectX::XMFLOAT3 pushBack(0.0f, 0.0f, 0.0f);

	// X軸での「めり込み量」を計算
	const float dx1 = (coll1.max.x - coll2.min.x);	// p1の右端 - p2の左端（p2が左から押し込んでいる時）
	const float dx2 = (coll1.min.x - coll2.max.x);	// p1の左端 - p2の右端（p2が右から押し込んでいる時）

	// Y軸での「めり込み量」を計算
	const float dy1 = (coll1.max.y - coll2.min.y);	// p1の上端 - p2の下端（p2が上から押し込んでいる時）
	const float dy2 = (coll1.min.y - coll2.max.y);	// p1の下端 - p2の上端（p2が下から押し込んでいる時）

	// X軸の処理
	// X軸方向でどちらに押し戻すべきか（小さい方が自然）
	if (fabsf(dx1) < fabsf(dx2)) {
		pushBack.x = dx1;	// 左から来たので右に押し戻し
	}
	else {
		pushBack.x = dx2;	// 右から来たので左に押し戻し
	}

	// Y軸の処理
	// Y軸方向でどちらに押し戻すべきか（小さい方が自然）
	if (fabsf(dy1) < fabsf(dy2)) {
		pushBack.y = dy1;	// 上から来たので下に押し戻し
	}
	else {
		pushBack.y = dy2;	// 下から来たので上に押し戻し
	}

	// Z軸は完全に無視（2.5Dのため）
	pushBack.z = 0.0f;

	// 法線初期化
	// ※ 出力される法線方向は逆向きになることに注意
	hitNormal = { 0.0f,0.0f,0.0f };

	// X軸とY軸のうち、より「めり込みが少ない」軸でのみ押し戻す
	// →壁にぶつかったとき、斜めではなく「垂直な方向」だけで修正されるようにするため
	if (fabsf(pushBack.x) < fabsf(pushBack.y)) {
		pushBack.y = 0.0f;	// 横方向で押し戻す

		// X方向に押し戻された → 壁
		//hitNormal.x = (pushBack.x > 0.0f) ? -1.0f : 1.0f;

		if (pushBack.x > 0.0f) {
			hitNormal.x = -1.0f;
		}
		else if (pushBack.x < 0.0f) {
			hitNormal.x = 1.0f;
		}
	}
	else {
		pushBack.x = 0.0f;	// 縦方向で押し戻す

		// Y方向に押し戻された → 地面 or 天井
	//	hitNormal.y = (pushBack.y > 0.0f) ? -1.0f : 1.0f;

		if (pushBack.y > 0.0f) {
			hitNormal.y = -1.0f;
		}
		else if (pushBack.y < 0.0f) {
			hitNormal.y = 1.0f;
		}
	}

	return true;
}

//==================================
// ■CheckHit関数
// AABBとAABBの当たり判定（2D用）
// どこから当たっているのか含む
// 戻しアリ
//==================================
bool ColliderComponent::CheckHit_CubeAndCube_NoTrigger2D_Normal(const ColliderComponent& p1, const ColliderComponent& p2, XMFLOAT3& hitNormal) {
	const AABB coll1 = p1.coll_ab;
	const AABB coll2 = p2.coll_ab;

	// そもそも衝突しているかどうかのチェック
	// 奥行きからの敵の登場のことも考えて、念のため
	// Z軸も考慮した当たり判定とする
	// 当たって押し出されるのはX軸とY軸みたいな
	const bool check = CheckHit_CubeAndCube_IsTrigger3D(p1, p2);

	if (check == true) {

		XMFLOAT3 returnDir = {};
		std::unordered_map<GameObject*, DirectX::XMFLOAT3>::iterator it = touchObjects.find(p1.m_Object);
		if (it != touchObjects.end()) {

			if (it->second != XMFLOAT3()) {
				hitNormal = it->second;
				returnDir = hitNormal;
			}
		}

		// 最終的のオブジェクトを押し戻すためのベクトル
		XMFLOAT3 pushBack(0.0f, 0.0f, 0.0f);

		// X軸での「めり込み量」を計算
		const float dx1 = (coll1.max.x - coll2.min.x);	// p1の右端 - p2の左端（p2が左から押し込んでいる時）
		const float dx2 = (coll1.min.x - coll2.max.x);	// p1の左端 - p2の右端（p2が右から押し込んでいる時）

		// Y軸での「めり込み量」を計算
		const float dy1 = (coll1.max.y - coll2.min.y);	// p1の上端 - p2の下端（p2が上から押し込んでいる時）
		const float dy2 = (coll1.min.y - coll2.max.y);	// p1の下端 - p2の上端（p2が下から押し込んでいる時）

		// X軸の処理
		// X軸方向でどちらに押し戻すべきか（小さい方が自然）
		if (fabsf(dx1) < fabsf(dx2)) {
			pushBack.x = dx1;	// 左から来たので右に押し戻し
		}
		else {
			pushBack.x = dx2;	// 右から来たので左に押し戻し
		}

		// Y軸の処理
		// Y軸方向でどちらに押し戻すべきか（小さい方が自然）
		if (fabsf(dy1) < fabsf(dy2)) {
			pushBack.y = dy1;	// 上から来たので下に押し戻し
		}
		else {
			pushBack.y = dy2;	// 下から来たので上に押し戻し
		}

		// Z軸は完全に無視（2.5Dのため）
		pushBack.z = 0.0f;

		if (returnDir == XMFLOAT3()) {

			// X軸とY軸のうち、より「めり込みが少ない」軸でのみ押し戻す
			// →壁にぶつかったとき、斜めではなく「垂直な方向」だけで修正されるようにするため
			if (fabsf(pushBack.x) < fabsf(pushBack.y)) {
				pushBack.y = 0.0f;	// 横方向で押し戻す

				// X方向に押し戻された → 壁

				if (pushBack.x > 0.0f) {

					hitNormal.x = -1.0f;
				}
				else if (pushBack.x < 0.0f) {
					hitNormal.x = 1.0f;
				}
			}
			else {

				pushBack.x = 0.0f;	// 縦方向で押し戻す

				// Y方向に押し戻された → 地面 or 天井

				if (pushBack.y > 0.0f) {

					hitNormal.y = -1.0f;
				}
				else if (pushBack.y < 0.0f) {
					hitNormal.y = 1.0f;
				}
			}
		}
		else {
			if (hitNormal.x != 0.0f) {
				pushBack.y = 0.0f;	// 横方向で押し戻す
			}
			else if (hitNormal.y != 0.0f) {
				pushBack.x = 0.0f;	// 縦方向で押し戻す
			}
		}

		// 挿入
		touchObjects.insert({ p1.m_Object,hitNormal });
		touchObjects[p1.m_Object] = hitNormal;

		// 位置補正をして、SRT情報を再計算
		TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
		if (transform == nullptr) {
			return false;
		}

		transform->AddPosition({ pushBack });

		const XMFLOAT3 pos = transform->GetPosition();
		const XMFLOAT3 size = transform->GetScale();
		SetColliderSize_AABB(pos, size);
		MakeWorldAABBMatrix();
	}
	else {
		// 存在確認をして、second情報をリセット上書き
		std::unordered_map<GameObject*, DirectX::XMFLOAT3>::iterator it = touchObjects.find(p1.m_Object);
		if (it != touchObjects.end()) {
			it->second = XMFLOAT3();
		}
	}

	// 衝突しているかどうかを返す
	return check;
}

//==================================
// ■CheckHit関数
// AABBとAABBの当たり判定（3D用）
// 戻しアリ
//==================================
bool ColliderComponent::CheckHit_CubeAndCube_NoTrigger3D(const ColliderComponent& p1, const ColliderComponent& p2, XMFLOAT3& pos) {

	const bool check = CheckHit_CubeAndCube_IsTrigger3D(p1, p2);

	if (check == true) {
		// 最終的のオブジェクトを押し戻すためのベクトル
		DirectX::XMFLOAT3 pushBack(0.0f, 0.0f, 0.0f);

		const AABB coll1 = p1.coll_ab;
		const AABB coll2 = p2.coll_ab;

		// 各軸での「めり込み量」を計算
		const float dx1 = (coll1.max.x - coll2.min.x);	// Bの右端 - Aの左端（Aが左から押し込んでいる時）
		const float dx2 = (coll1.min.x - coll2.max.x);	// Bの右端 - Aの左端（Aが右から押し込んでいる時）

		const float dy1 = (coll1.max.y - coll2.min.y);	// Bの右端 - Aの左端（Aが下から押し込んでいる時）
		const float dy2 = (coll1.min.y - coll2.max.y);	// Bの右端 - Aの左端（Aが上から押し込んでいる時）

		const float dz1 = (coll1.max.z - coll2.min.z);	// Bの右端 - Aの左端（Aが奥から押し込んでいる時）
		const float dz2 = (coll1.min.z - coll2.max.z);	// Bの右端 - Aの左端（Aが前から押し込んでいる時）

		// X軸の処理
		if (fabsf(dx1) < fabsf(dx2))
		{
			pushBack.x = dx1;
		}
		else {
			pushBack.x = dx2;
		}

		// Y軸の処理
		if (fabsf(dy1) < fabsf(dy2)) {
			pushBack.y = dy1;
		}
		else {
			pushBack.y = dy2;
		}

		// Z軸の処理
		if (fabsf(dz1) < fabsf(dz2)) {
			pushBack.z = dz1;
		}
		else {
			pushBack.z = dz2;
		}

		// 最小のめり込み量の軸を1つ選んで、その軸にだけ押し戻しを適用（自然な反応にする）
		const float fabsfX = fabsf(pushBack.x);
		const float fabsfY = fabsf(pushBack.y);
		const float fabsfZ = fabsf(pushBack.z);

		// 最小軸を選ぶ
		if (fabsfX <= fabsfY && fabsfX <= fabsfZ) {	// X軸が最小
			pushBack.y = 0.0f;
			pushBack.z = 0.0f;
		}
		else if (fabsfY <= fabsfX && fabsfY <= fabsfZ) {	// Y軸が最小
			pushBack.x = 0.0f;
			pushBack.z = 0.0f;
		}
		else {								// Z軸が最小
			pushBack.x = 0.0f;
			pushBack.y = 0.0f;
		}

		// 押し戻しを反映
		pos.x += pushBack.x;
		pos.y += pushBack.y;
		pos.z += pushBack.z;
	}

	return check;
}

// レイとAABBの交差を調べ、交差していればレイの進んだ距離を返す関数
bool ColliderComponent::IntersectRayAABB(
	const DirectX::XMVECTOR& rayOrigin,	// レイの発射点
	const DirectX::XMVECTOR& rayDir,	// レイの方向（単位ベクトル）
	const AABB& hit,					// AABBの最小値と最大値
	float& tMinOut)						// ヒットした距離を返す
{
	float tmin = 0.0f;	// レイとAABBの最小交差距離
	float tmax = FLT_MAX;	// レイとAABBの最大交差距離

	// スラブ法
	// AABBを各軸（X,Y,Z）ごとに「スラブ = ２枚の平面」で切って考える
	for (int i = 0; i < 3; ++i) {
		const float origin = rayOrigin.m128_f32[i];
		const float dir = rayDir.m128_f32[i];
		const float bmin = (&hit.min.x)[i];	// AABBの最小値
		const float bmax = (&hit.max.x)[i];

		if (fabs(dir) < 1e-6f) {	// レイの方向がほぼゼロ（平行）
			if (origin < bmin || origin > bmax) {
				return false;	// レイはAABBに交差しない
			}
		}
		else {
			float invDir = 1.0f / dir;

			// スラブの入口と出口の距離
			float t1 = (bmin - origin) * invDir;	// AABBの最小値との交差距離
			float t2 = (bmax - origin) * invDir;	// AABBの最大値との交差距離
			if (t1 > t2) std::swap(t1, t2);	// t1とt2を入れ替え
			tmin = std::max(tmin, t1);	// 最小交差距離を更新
			tmax = std::min(tmax, t2);	// 最大交差距離を更新
			if (tmin > tmax) {
				return false;	// レイはAABBに交差しない
			}
		}
	}

	tMinOut = tmin;	// ヒットした距離を出力
	return true;	// レイはAABBに交差した
}

bool ColliderComponent::TestNormal(const ColliderComponent& p1, const ColliderComponent& p2, DirectX::XMFLOAT3& hitNormal) {

	const AABB coll1 = p1.coll_ab;
	const AABB coll2 = p2.coll_ab;
	const AABB beforeColl2 = p2.beforeColl_ab;

	const bool check = CheckHit_CubeAndCube_IsTrigger3D(p1, p2);

	if (check == true) {

		// 最終的のオブジェクトを押し戻すためのベクトル
		XMFLOAT3 pushBack(0.0f, 0.0f, 0.0f);

		// X軸での「めり込み量」を計算
		const float dx1 = (coll1.max.x - coll2.min.x);	// p1の右端 - p2の左端（p2が左から押し込んでいる時）
		const float dx2 = (coll1.min.x - coll2.max.x);	// p1の左端 - p2の右端（p2が右から押し込んでいる時）

		// Y軸での「めり込み量」を計算
		const float dy1 = (coll1.max.y - coll2.min.y);	// p1の上端 - p2の下端（p2が上から押し込んでいる時）
		const float dy2 = (coll1.min.y - coll2.max.y);	// p1の下端 - p2の上端（p2が下から押し込んでいる時）

		// X軸の処理
		// X軸方向でどちらに押し戻すべきか（小さい方が自然）
		if (fabsf(dx1) < fabsf(dx2)) {
			pushBack.x = dx1;	// 左から来たので右に押し戻し
		}
		else {
			pushBack.x = dx2;	// 右から来たので左に押し戻し
		}

		// Y軸の処理
		// Y軸方向でどちらに押し戻すべきか（小さい方が自然）
		if (fabsf(dy1) < fabsf(dy2)) {
			pushBack.y = dy1;	// 上から来たので上に押し戻し
		}
		else {
			pushBack.y = dy2;	// 下から来たので下に押し戻し
		}

		// めり込み四角形（交差領域）計算
		const float ixMin = std::max(coll1.min.x, coll2.min.x);
		const float iyMin = std::max(coll1.min.y, coll2.min.y);
		const float ixMax = std::min(coll1.max.x, coll2.max.x);
		const float iyMax = std::min(coll1.max.y, coll2.max.y);

		XMFLOAT2 result1 = {};
		XMFLOAT2 result2 = {};
		XMFLOAT2 result3 = {};

		bool forcedBeside = false;	// 強制的に横押し出しにするかどうか

		int  rightLeft = 0;			// 右か左のどっちに押し出すか

		// 上から来た場合
		if (coll1.max.y == iyMax) {

			// めり込みでできた四角形の下底から、前フレームの
			// AABBの上底に対してベクトルを引く
			if (coll1.max.x > coll2.max.x && coll1.min.x < coll2.min.x) {
				result1.y = 1.0f;	// 上に押し出す
				result2.y = 1.0f;	// 上に押し出す
				result3.y = 1.0f;	// 上に押し出す
			}
			else if (coll2.min.x == ixMin) {	// coll2が右から
				// 前のフレームの左下の頂点
				result1 = XMFLOAT2(beforeColl2.min.x, beforeColl2.max.y) - XMFLOAT2(ixMin, iyMin);

				result2 = XMFLOAT2(beforeColl2.min.x, beforeColl2.min.y) - XMFLOAT2(ixMax, iyMax);
				// 上に平行移動した位置
				const XMFLOAT2 newResult2 = XMFLOAT2(beforeColl2.min.x, beforeColl2.min.y + dy1);
				result3 = newResult2 - XMFLOAT2(coll1.max.x, iyMin);

				if (static_cast<int>(coll1.max.x) == static_cast<int>(beforeColl2.min.x)) {
					forcedBeside = true;	// 強制的に横押し出しにする
				}
				rightLeft = -1;
			}
			else if (coll2.max.x == ixMax) { // coll2が左から
				// 前のフレームの右下の頂点
				result1 = XMFLOAT2(beforeColl2.max.x, beforeColl2.max.y) - XMFLOAT2(ixMax, iyMin);
				result2 = XMFLOAT2(beforeColl2.max.x, beforeColl2.min.y) - XMFLOAT2(ixMin, iyMax);
				// 上に平行移動した位置
				const XMFLOAT2 newResult2 = XMFLOAT2(beforeColl2.max.x, beforeColl2.min.y + dy1);
				result3 = newResult2 - XMFLOAT2(coll1.min.x, iyMin);

				if (static_cast<int>(coll1.min.x) == static_cast<int>(beforeColl2.max.x)) {
					forcedBeside = true;	// 強制的に横押し出しにする
				}
				rightLeft = 1;
			}
		}

		// 下から来た場合
		else if (coll1.min.y == iyMin) {

			if (coll1.max.x > coll2.max.x && coll1.min.x < coll2.min.x) {
				result1.y = -1.0f;	// 下に押し出す
				result2.y = -1.0f;	// 下に押し出す
				result3.y = -1.0f;	// 下に押し出す
			}
			else if (coll2.min.x == ixMin) {	// coll2が右から
				// 前のフレームの左下の頂点
				result1 = XMFLOAT2(beforeColl2.min.x, beforeColl2.min.y) - XMFLOAT2(ixMin, iyMax);

				result2 = XMFLOAT2(beforeColl2.min.x, beforeColl2.max.y) - XMFLOAT2(ixMax, iyMin);
				// 上に平行移動した位置
				XMFLOAT2 newResult2 = XMFLOAT2(beforeColl2.min.x, beforeColl2.max.y + dy2);
				result3 = newResult2 - XMFLOAT2(coll1.max.x, iyMax);

				if (static_cast<int>(coll1.max.x) == static_cast<int>(beforeColl2.min.x)) {
					forcedBeside = true;	// 強制的に横押し出しにする
				}
				rightLeft = -1;
			}
			else if (coll2.max.x == ixMax) { // coll2が左から
				// 前のフレームの右下の頂点
				result1 = XMFLOAT2(beforeColl2.max.x, beforeColl2.min.y) - XMFLOAT2(ixMax, iyMax);

				result2 = XMFLOAT2(beforeColl2.max.x, beforeColl2.max.y) - XMFLOAT2(ixMin, iyMin);
				// 上に平行移動した位置
				XMFLOAT2 newResult2 = XMFLOAT2(beforeColl2.max.x, beforeColl2.max.y + dy2);
				result3 = newResult2 - XMFLOAT2(coll1.min.x, iyMax);
				// 上引っかかりを失くす
				if (static_cast<int>(coll1.max.x) == static_cast<int>(beforeColl2.min.x)) {
					forcedBeside = true;	// 強制的に横押し出しにする
				}
				rightLeft = 1;
			}
		}
		else {
			forcedBeside = true;	// 強制的に横押し出しにする
		}

		// 強制押し出しのとき、引っかかる個所を解除
		if (forcedBeside == true && result2.y == 0.0f/* && rightLeft == -1*/) {
			forcedBeside = false;	// 強制的に横押し出しにするのを解除
		}
		if (forcedBeside == false && result2.y != 0.0f && result2.x == 0.0f && result3.y != 0.0f && result3.x == 0.0f && rightLeft == 1) {
			forcedBeside = true;	// 強制的に横押し出しにする
		}

		// Z軸は完全に無視（2.5Dのため）
		pushBack.z = 0.0f;

		if (((result1.y > 0.0f && result2.y >= 0.0f && result3.y >= 0.0f) || (result1.y < 0.0f && result2.y <= 0.0f && result3.y <= 0.0f)) && forcedBeside == false) {
			pushBack.x = 0.0f;	// 縦方向で押し戻す

			// Y方向に押し戻された → 地面 or 天井
			if (pushBack.y > 0.0f) {

				hitNormal.y = -1.0f;
			}
			else if (pushBack.y < 0.0f) {
				hitNormal.y = 1.0f;
			}
		}
		else {

			pushBack.y = 0.0f;	// 横方向で押し戻す

			// X方向に押し戻された → 壁

			if (pushBack.x > 0.0f) {

				hitNormal.x = -1.0f;
			}
			else if (pushBack.x < 0.0f) {
				hitNormal.x = 1.0f;
			}
		}

		// 位置補正をして、SRT情報を再計算
		TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
		
		if(transform == nullptr) {
			return false;
		}
		transform->AddPosition({ pushBack });

		XMFLOAT3 pos = transform->GetPosition();
		XMFLOAT3 size = transform->GetScale();
		SetColliderSize_AABB(pos, size);
		MakeWorldAABBMatrix();
		//	beforeColl_ab = GetColliderSize_AABB();
	}

	// 衝突しているかどうかを返す
	return check;
}


bool ColliderComponent::CheckHit_SphereAndSphere_IsTrigger3D(const Sphere& p1, const Sphere& p2) {
	const float dx = p1.center.x - p2.center.x;	// ２つのオブジェクトの中心位置の差分
	const float dy = p1.center.y - p2.center.y;	// ２つのオブジェクトの中心位置の差分
	const float dz = p1.center.z - p2.center.z;	// ２つのオブジェクトの中心位置の差分

	const float distSq = dx * dx + dy * dy + dz * dz;
	const float radiusSum = p1.radius + p2.radius;	// ２つのオブジェクトの半径を足す

	// 半径から距離を比較
	return distSq < (radiusSum * radiusSum);
}

// 検知と押し出し
bool ColliderComponent::CheckHit_SphereAndSphere_NoTrigger2D(const Sphere& p1, const Sphere& p2, DirectX::XMFLOAT3& pos) {
	// p2が押す方、p1が押される方とする

	const float dx = p2.center.x - p1.center.x;	// ２つのオブジェクトの中心位置の差分
	const float dy = p2.center.y - p1.center.y;	// ２つのオブジェクトの中心位置の差分
	const float dz = p2.center.z - p1.center.z;	// ２つのオブジェクトの中心位置の差分

	const float distSq1 = dx * dx + dy * dy;
	const float distSq2 = dx * dx + dy * dy + dz * dz;
	const float radiusSum = p1.radius + p2.radius;	// ２つのオブジェクトの半径を足す

	if (distSq2 < (radiusSum * radiusSum)) {	// 念のため、Z軸も当たっているかを確認
		const float dist = std::sqrt(distSq1);		// ２つのオブジェクトの中心位置の距離（平方根を使い算出）
		const float overlap = radiusSum - dist;	// ２つのオブジェクトの半径の合計から距離を引いて、重なり量を算出

		// 正規化
		if (dist > 0.0f) {
			// 方向ベクトル（p2 → p1の方向）を正規化して逆向きに押し出す
			const float nx = dx / dist;
			const float ny = dy / dist;

			// p1をp2から遠ざける方向に押し出す（当たってきた方向の逆）
			pos.x = p2.center.x + nx * overlap;
			pos.y = p2.center.y + ny * overlap;
			//pos.z = 0.0f;	// 2.5DなのでZ軸は無視
		}
		else {
			// 中心が完全一致 → 適切な方向に話す
			pos = XMFLOAT3(0.0f, radiusSum, 0.0f);
		}
		return true;
	}

	return false;
}

bool ColliderComponent::CheckHit_SphereAndSphere_NoTrigger3D(const Sphere& p1, const Sphere& p2, DirectX::XMFLOAT3& pos) {

	return true;
}

bool ColliderComponent::CheckHit_SphereAndCube_IsTrigger3D(const Sphere& p1, const AABB& p2) {
	// 最近接点を求める
	XMFLOAT3 closestPoint = {
		std::clamp(p1.center.x, p2.min.x, p2.max.x),
		std::clamp(p1.center.y, p2.min.y, p2.max.y),
		std::clamp(p1.center.z, p2.min.z, p2.max.z),
	};

	const float dx = p1.center.x - closestPoint.x;	// ２つのオブジェクトの中心位置の差分
	const float dy = p1.center.y - closestPoint.y;	// ２つのオブジェクトの中心位置の差分
	const float dz = p1.center.z - closestPoint.z;	// ２つのオブジェクトの中心位置の差分

	const float distSq = dx * dx + dy * dy + dz * dz;	// ２つのオブジェクトの中心位置の距離の二乗
	const float r = p1.radius;	// 球体の半径

	if (distSq < (r * r)) {	// 半径から距離を比較
		// 衝突している
		return true;
	}

	// 衝突無し
	return false;
}

// 検知と押し出し
bool ColliderComponent::CheckHit_SphereAndCube_NoTrigger2D(const Sphere& p1, const AABB& p2, DirectX::XMFLOAT3& pos) {
	// 最近接点を求める
	XMFLOAT3 closestPoint = {
		std::clamp(p1.center.x, p2.min.x, p2.max.x),
		std::clamp(p1.center.y, p2.min.y, p2.max.y),
		std::clamp(p1.center.z, p2.min.z, p2.max.z),
	};

	const float dx = p1.center.x - closestPoint.x;	// ２つのオブジェクトの中心位置の差分
	const float dy = p1.center.y - closestPoint.y;	// ２つのオブジェクトの中心位置の差分
	const float dz = p1.center.z - closestPoint.z;	// ２つのオブジェクトの中心位置の差分

	const float distSq1 = dx * dx + dy * dy;	// ２つのオブジェクトの中心位置の距離の二乗
	const float distSq2 = dx * dx + dy * dy + dz * dz;	// ２つのオブジェクトの中心位置の距離の二乗
	const float r = p1.radius;	// 球体の半径

	if (distSq2 < (r * r)) {	// 半径から距離を比較
		const float dist = std::sqrt(distSq1);		// ２つのオブジェクトの中心位置の距離（平方根を使い算出）
		const float overlap = r - dist;	// ２つのオブジェクトの半径から距離を引いて、重なり量を算出

		if (dist > 0.0001f) {
			// 方向ベクトル（p2 → p1の方向）を正規化して逆向きに押し出す
			const float nx = dx / dist;
			const float ny = dy / dist;

			pos.x = p1.center.x + nx * overlap;
			pos.y = p1.center.y + ny * overlap;
			//pos.z = dz / dist * overlap;
		}
		else {
			// 球が直方体の中に入っている場合：適当な方向に押す
			pos = { 0.0f,overlap,0.0f };	// 上方向など
		}

		// 衝突している
		return true;
	}

	// 衝突無し
	return false;
}

bool ColliderComponent::CheckHit_SphereAndCube_NoTrigger3D(const Sphere& p1, const AABB& p2, DirectX::XMFLOAT3& pos) {

	return false;
}

// ===============================
// 後に再利用するかもしれない関数
// 
// 触れているかどうかだけを調べる
bool ColliderComponent::CubeAndCubeCheck_OBB(const OBB& col1, const OBB& col2) {
	//DirectX::BoundingOrientedBox obb1;
	//DirectX::BoundingOrientedBox obb2;

	//// 回転角をラジアンに変換
	//float col1PitchRadians = DirectX::XMConvertToRadians(col1.rotation.x); // X軸回転
	//float col1YawRadians = DirectX::XMConvertToRadians(col1.rotation.y);     // Y軸回転
	//float col1RollRadians = DirectX::XMConvertToRadians(col1.rotation.z);   // Z軸回転

	//float col2PitchRadians = DirectX::XMConvertToRadians(col2.rotation.x); // X軸回転
	//float col2YawRadians = DirectX::XMConvertToRadians(col2.rotation.y);     // Y軸回転
	//float col2RollRadians = DirectX::XMConvertToRadians(col2.rotation.z);   // Z軸回転

	//// OBB1の設定
	//obb1.Center = DirectX::XMFLOAT3(col1.center.x, col1.center.y, col1.center.z);
	//obb1.Extents = DirectX::XMFLOAT3(col1.size.x / 2.0f, col1.size.y / 2.0f, col1.size.z / 2.0f);

	//// OBB1の回転
	//DirectX::XMVECTOR quaternion1 = DirectX::XMQuaternionNormalize(
	//	DirectX::XMQuaternionRotationRollPitchYaw(col1PitchRadians, col1YawRadians, col1RollRadians)
	//);
	//DirectX::XMStoreFloat4(&obb1.Orientation, quaternion1);

	//// OBB2の設定
	//obb2.Center = DirectX::XMFLOAT3(col2.center.x, col2.center.y, col2.center.z);
	//obb2.Extents = DirectX::XMFLOAT3(col2.size.x / 2.0f, col2.size.y / 2.0f, col2.size.z / 2.0f);

	//// OBB2の回転
	//DirectX::XMVECTOR quaternion2 = DirectX::XMQuaternionNormalize(
	//	DirectX::XMQuaternionRotationRollPitchYaw(col2PitchRadians, col2YawRadians, col2RollRadians)
	//);
	//DirectX::XMStoreFloat4(&obb2.Orientation, quaternion2);

	//// 衝突判定
	//return obb1.Intersects(obb2);

	// 使うかどうかわからないので一度コメントアウト

	return false;
}

// 触れているかを調べ、めり込みを直す
bool ColliderComponent::CubeAndCubeHit_OBB(const OBB& col1, const OBB& col2) {
	//DirectX::BoundingOrientedBox obb1;
	//DirectX::BoundingOrientedBox obb2;

	//// 回転角をラジアンに変換
	//float col1PitchRadians = DirectX::XMConvertToRadians(col1.rotation.x); // X軸回転
	//float col1YawRadians = DirectX::XMConvertToRadians(col1.rotation.y);     // Y軸回転
	//float col1RollRadians = DirectX::XMConvertToRadians(col1.rotation.z);   // Z軸回転

	//float col2PitchRadians = DirectX::XMConvertToRadians(col2.rotation.x); // X軸回転
	//float col2YawRadians = DirectX::XMConvertToRadians(col2.rotation.y);     // Y軸回転
	//float col2RollRadians = DirectX::XMConvertToRadians(col2.rotation.z);   // Z軸回転

	//// OBB1の設定
	//obb1.Center = DirectX::XMFLOAT3(col1.center.x, col1.center.y, col1.center.z);
	//obb1.Extents = DirectX::XMFLOAT3(col1.size.x / 2.0f, col1.size.y / 2.0f, col1.size.z / 2.0f);

	//// OBB1の回転
	//DirectX::XMVECTOR quaternion1 = DirectX::XMQuaternionNormalize(
	//	DirectX::XMQuaternionRotationRollPitchYaw(col1PitchRadians, col1YawRadians, col1RollRadians)
	//);
	//DirectX::XMStoreFloat4(&obb1.Orientation, quaternion1);

	//// OBB2の設定
	//obb2.Center = DirectX::XMFLOAT3(col2.center.x, col2.center.y, col2.center.z);
	//obb2.Extents = DirectX::XMFLOAT3(col2.size.x / 2.0f, col2.size.y / 2.0f, col2.size.z / 2.0f);

	//// OBB2の回転
	//DirectX::XMVECTOR quaternion2 = DirectX::XMQuaternionNormalize(
	//	DirectX::XMQuaternionRotationRollPitchYaw(col2PitchRadians, col2YawRadians, col2RollRadians)
	//);
	//DirectX::XMStoreFloat4(&obb2.Orientation, quaternion2);

	//// 衝突判定
	//return obb1.Intersects(obb2);
	
	// 使うかどうかわからないので一度コメントアウト

	return false;
}
// ===============================

bool ColliderComponent::CheckHit_AABBAndOBB_IsTrigger3D(const ColliderComponent& aabb, const ColliderComponent& obb) {

	if (aabb.m_activeColliderFlag == false || obb.m_activeColliderFlag == false) {
		return false; // どちらかのコライダーが無効なら衝突無し
	}

	const XMFLOAT3 AABBCenter = (aabb.coll_ab.max + aabb.coll_ab.min) * 0.5f;
	const XMFLOAT3 OBBCenter = obb.coll_ob.center;

	XMFLOAT3 vecDistance = AABBCenter - OBBCenter;

	const XMFLOAT3 obbAxes[3] = { obb.coll_ob.axisX, obb.coll_ob.axisY, obb.coll_ob.axisZ };
	const XMFLOAT3 aabbAxes[3] = { XMFLOAT3(1.0f,0.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f) };

	// OBBの3軸
	for (int i = 0; i < 3; ++i) {
		if (!CompareLengthOBBvsAABB(obb.coll_ob, aabb.coll_ab, obbAxes[i], vecDistance)) return false;
	}

	// AABBの3軸
	for (int i = 0; i < 3; ++i) {
		if (!CompareLengthOBBvsAABB(obb.coll_ob, aabb.coll_ab, aabbAxes[i], vecDistance)) return false;
	}

	// 外積9本
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {

			XMVECTOR va = XMLoadFloat3(&obbAxes[i]);
			XMVECTOR vb = XMLoadFloat3(&aabbAxes[j]);

			XMVECTOR axisV = XMVector3Cross(va, vb);

			// 長さの２乗
			XMVECTOR lenSq = XMVector3LengthSq(axisV);

			// しきい値
			XMVECTOR threshold = XMVectorReplicate(1e-6f);

			if (XMVector3Less(lenSq, threshold)) {
				continue; // 軸がほぼゼロベクトルなら比較不要
			}

			axisV = XMVector3Normalize(axisV);
			XMFLOAT3 axis;
			XMStoreFloat3(&axis, axisV);

			if (!CompareLengthOBBvsAABB(obb.coll_ob, aabb.coll_ab, axis, vecDistance)) {
				return false;
			}
		}
	}

	return true;
}

bool ColliderComponent::CompareLengthOBBvsAABB(
	const OBB& obb,
	const AABB& aabb,
	const XMFLOAT3& axis,
	const XMFLOAT3& vecDistance)
{
	XMVECTOR axisV = XMLoadFloat3(&axis);

	// 長さの２乗
	const XMVECTOR lenSq = XMVector3LengthSq(axisV);

	// しきい値
	const XMVECTOR threshold = XMVectorReplicate(1e-6f);

	// 軸がほぼゼロベクトルなら比較不要
	if (XMVector3Less(lenSq, threshold)) {
		return true;
	}

	axisV = XMVector3Normalize(axisV);
	XMFLOAT3 axisN;
	XMStoreFloat3(&axisN, axisV);

	const XMVECTOR obbAxisX = XMLoadFloat3(&obb.axisX);
	const XMVECTOR obbAxisY = XMLoadFloat3(&obb.axisY);
	const XMVECTOR obbAxisZ = XMLoadFloat3(&obb.axisZ);

	const XMVECTOR dotX = XMVector3Dot(obbAxisX, axisV);
	const XMVECTOR dotY = XMVector3Dot(obbAxisY, axisV);
	const XMVECTOR dotZ = XMVector3Dot(obbAxisZ, axisV);

	const float dotXf = fabsf(XMVectorGetX(dotX));
	const float dotYf = fabsf(XMVectorGetX(dotY));
	const float dotZf = fabsf(XMVectorGetX(dotZ));

	// OBBの半径（各軸方向の投影長を合計）
	const float rOBB =
		fabsf(obb.size.x * dotXf) +
		fabsf(obb.size.y * dotYf) +
		fabsf(obb.size.z * dotZf);

	// AABBの半径（軸はワールド固定X,Y,Z）
	XMFLOAT3 aabbAxisX(1, 0, 0);
	XMFLOAT3 aabbAxisY(0, 1, 0);
	XMFLOAT3 aabbAxisZ(0, 0, 1);

	const float extX = (aabb.max.x - aabb.min.x) * 0.5f;
	const float extY = (aabb.max.y - aabb.min.y) * 0.5f;
	const float extZ = (aabb.max.z - aabb.min.z) * 0.5f;

	const XMVECTOR aabbAxisXV = XMLoadFloat3(&aabbAxisX);
	const XMVECTOR aabbAxisYV = XMLoadFloat3(&aabbAxisY);
	const XMVECTOR aabbAxisZV = XMLoadFloat3(&aabbAxisZ);

	const XMVECTOR dotAABBX = XMVector3Dot(aabbAxisXV, axisV);
	const XMVECTOR dotAABBY = XMVector3Dot(aabbAxisYV, axisV);
	const XMVECTOR dotAABBZ = XMVector3Dot(aabbAxisZV, axisV);

	const float dotAABBXf = fabsf(XMVectorGetX(dotAABBX));
	const float dotAABBYf = fabsf(XMVectorGetX(dotAABBY));
	const float dotAABBZf = fabsf(XMVectorGetX(dotAABBZ));

	const float rAABB =
		fabsf(extX * dotAABBXf) +
		fabsf(extY * dotAABBYf) +
		fabsf(extZ * dotAABBZf);

	const XMVECTOR vecDistanceV = XMLoadFloat3(&vecDistance);
	const XMVECTOR dotDistance = XMVector3Dot(vecDistanceV, axisV);
	const float dotDistanceF = fabsf(XMVectorGetX(dotDistance));

	// 中心間距離を投影
	float dist = fabsf(dotDistanceF);

	// 分離軸判定
	return dist <= (rOBB + rAABB);
}


//AABB& DetectCollisionFace(const AABB& a, const AABB& b) {
//	if (!CheckHit(a, b)) {
//		//return none;
//	}

//	float overlapX = std::min(DirectX::XMVectorGetX(a.max), DirectX::XMVectorGetX(b.max)) -
//		std::max(DirectX::XMVectorGetX(a.min), DirectX::XMVectorGetX(b.min));
//	float overlapY = std::min(DirectX::XMVectorGetY(a.max), DirectX::XMVectorGetY(b.max)) -
//		std::max(DirectX::XMVectorGetY(a.min), DirectX::XMVectorGetY(b.min));
//	float overlapZ = std::min(DirectX::XMVectorGetZ(a.max), DirectX::XMVectorGetZ(b.max)) -
//		std::max(DirectX::XMVectorGetZ(a.min), DirectX::XMVectorGetZ(b.min));

//	if (overlapX < overlapY && overlapX < overlapZ) {
//		if (a.max.x > b.max.x) {
//			return Face::Right;
//		}
//		else {
//			return CollisionFace::Left;
//		}
//	}
//	else if (overlapY < overlapX && overlapY < overlapZ) {
//		if (a.max.y > b.max.y) {
//			return CollisionFace::Top;
//		}
//		else {
//			return CollisionFace::Bottom;
//		}
//	}
//	else {
//		if (a.max.z > b.max.z) {
//			return CollisionFace::Front;
//		}
//		else {
//			return CollisionFace::Back;
//		}
//	}
//}


//=========================================================================================
//=========================================================================================
//=========================================================================================
//=========================================================================================
//=========================================================================================



//==================================
// ■Dot関数
// ベクトルの内積を求める
//==================================
float ColliderComponent::Dot(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//==================================
// ■Cross関数
// ベクトルの外積を求める
//==================================
XMFLOAT3 ColliderComponent::Cross(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return XMFLOAT3(
		v1.y * v2.z - v1.z * v2.y, // x成分
		v1.z * v2.x - v1.x * v2.z, // y成分
		v1.x * v2.y - v1.y * v2.x); // z成分
}

//==================================
// ■ClosestPointOnSegment関数
// 点から線分までの最近地点
//==================================
XMFLOAT3 ColliderComponent::ClosestPointOnSegment(const XMFLOAT3& point, const Segment& segment)
{
	// 線分のベクトルを計算（終点 - 始点）
	XMFLOAT3 vec = segment.end - segment.start;

	// 線分の長さの平方を計算（ベクトルの大きさの2乗）
	double r2 = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;

	// 点から線分の始点までのベクトルとの内積を計算
	double tt = -Dot(vec, (segment.start - point));

	// 点が始点よりも線分の外側
	if (tt < 0)
	{
		// 点と線分の始点の距離を返す
		return segment.start;
	}
	// 点が終点よりも線分の外側の場合
	else if (tt > r2)
	{
		// 点と線分の終点との距離を返す
		return segment.end;
	}
	// 点が線分の上に投影される場合、線分上の最短距離を計算
	else
	{
		XMFLOAT3 ab = segment.end - segment.start; // 線分のベクトル

		XMVECTOR abV = XMLoadFloat3(&ab);

		XMVECTOR lengthAbV = XMVector3LengthSq(abV);

		float lengthSq = XMVectorGetX(lengthAbV); // 線分の長さの二乗

		// 線分が退化（両端が同じ点）している場合
		if (lengthSq == 0.0f) {
			return segment.start; // 線分の端点を返す
		}

		// 点が線分上に投影される位置を計算（0 <= t <= 1 の範囲）
		XMFLOAT3 sub = point - segment.start; // 点から線分の始点へのベクトル
		XMVECTOR subV = XMLoadFloat3(&sub);

		const XMVECTOR dotSub = XMVector3Dot(subV, abV);

		float t = XMVectorGetX(dotSub / lengthSq);
		t = std::clamp(t, 0.0f, 1.0f);

		// 線分上の最近接点を返す
		return (segment.start + t * ab);
	}
}

//==================================
// ■DistanceSquaredPointToSegment関数
// 点から線分までの距離の2乗
//==================================
float ColliderComponent::DistanceSquaredPointToSegment(const XMFLOAT3& point, const Segment& segment)
{
	XMFLOAT3 p;
	return DistanceSquaredPointToSegment(point, segment, p);
}
float ColliderComponent::DistanceSquaredPointToSegment(const XMFLOAT3& point, const Segment& segment, XMFLOAT3& contact)
{
	contact = ClosestPointOnSegment(point, segment);

	const XMFLOAT3 sub = point - contact;

	XMVECTOR subV = XMLoadFloat3(&sub);

	XMVECTOR lengthSubV = XMVector3LengthSq(subV);

	float lengthSq = XMVectorGetX(lengthSubV);

	return lengthSq;
}

//==================================
// ■DistancePointToSegment関数
// 点から線分までの距離
//==================================
float ColliderComponent::DistancePointToSegment(const XMFLOAT3& point, const Segment& segment)
{
	XMFLOAT3 p;
	return DistancePointToSegment(point, segment, p);
}
float ColliderComponent::DistancePointToSegment(const XMFLOAT3& point, const Segment& segment, XMFLOAT3& contact)
{
	contact = ClosestPointOnSegment(point, segment);

	XMVECTOR va = XMLoadFloat3(&point);
	XMVECTOR vb = XMLoadFloat3(&contact);

	float dist = XMVectorGetX(XMVector3Length(va - vb));

	return dist;
}

//==================================
// ■DistancePointToPlane関数
// 点から平面までの距離
//==================================
float ColliderComponent::DistancePointToPlane(const XMFLOAT3& point, const PlaneNormal& plane)
{
	return fabs(Dot((point - plane.point), plane.normal) / Dot(plane.normal, plane.normal));
}

//==================================
// ■ProjectPointToPlane関数
// 点から平面に下ろした垂線の交点
//==================================
XMFLOAT3 ColliderComponent::ProjectPointToPlane(const XMFLOAT3& point, const PlaneNormal& plane)
{
	double t = -Dot((point - plane.point), plane.normal) / Dot(plane.normal, plane.normal);

	// 交点の座標を計算
	return  point + (plane.normal * (float)t);
}


//==================================
// ■PointInTriangle関数
// 三角形の内部かどうかを判定する
//==================================
bool ColliderComponent::PointInTriangle(const XMFLOAT3& point, const TrianglePolygon& polygon)
{
	// 3辺のベクトル
	XMFLOAT3 ab = polygon.p1 - polygon.p0;
	XMFLOAT3 bc = polygon.p2 - polygon.p1;
	XMFLOAT3 ca = polygon.p0 - polygon.p2;

	// 内部の点とのベクトル
	XMFLOAT3 ap = point - polygon.p0;
	XMFLOAT3 bp = point - polygon.p1;
	XMFLOAT3 cp = point - polygon.p2;

	// 3辺と内部の点との法線ベクトル
	XMFLOAT3	n1 = Cross(ab, ap);
	XMFLOAT3	n2 = Cross(bc, bp);
	XMFLOAT3	n3 = Cross(ca, cp);

	// 三角形の法線ベクトル
	XMFLOAT3	normal = Cross(ab, bc);

	const XMVECTOR normalV = XMLoadFloat3(&normal);
	const XMVECTOR n1V = XMLoadFloat3(&n1);
	const XMVECTOR n2V = XMLoadFloat3(&n2);
	const XMVECTOR n3V = XMLoadFloat3(&n3);

	const XMVECTOR dot1 = XMVector3Dot(n1V, normalV);
	const XMVECTOR dot2 = XMVector3Dot(n2V, normalV);
	const XMVECTOR dot3 = XMVector3Dot(n3V, normalV);

	float dot = XMVectorGetX(dot1);
	if (dot < 0) return false; // 為す角度が鈍角

	dot = XMVectorGetX(dot2);
	if (dot < 0) return false; // 為す角度が鈍角

	dot = XMVectorGetX(dot3);
	if (dot < 0) return false; // 為す角度が鈍角

	return true;
}


//==================================
// ■ClosestPointOnTriangle関数
// 点と三角形間の最近接点を求める
//==================================
XMFLOAT3 ColliderComponent::ClosestPointOnTriangle(const XMFLOAT3& point, const TrianglePolygon& polygon)
{
	PlaneNormal plane(polygon.p0, GetNormal(polygon));

	// 線分と平面の交点を計算
	XMFLOAT3 p = ProjectPointToPlane(point, plane);

	// 交点が三角形ポリゴンの内側にあるか判定
	if (PointInTriangle(p, polygon))
	{
		return p;
	}

	// 交点が三角形ポリゴンの外側なら各辺との距離を計算
	XMFLOAT3 p1, p2, p3;
	float d1 = DistanceSquaredPointToSegment(point, { polygon.p0 , polygon.p1 }, p1);
	float d2 = DistanceSquaredPointToSegment(point, { polygon.p1 , polygon.p2 }, p2);
	float d3 = DistanceSquaredPointToSegment(point, { polygon.p2 , polygon.p0 }, p3);

	if (d1 < d2)
	{
		if (d1 < d3)
		{
			return p1;
		}
		else
		{
			return p3;
		}
	}
	else
	{
		if (d2 < d3)
		{
			return p2;
		}
		else
		{
			return p3;
		}
	}
}

//==================================
// 法線を計算
//==================================
XMFLOAT3 ColliderComponent::GetNormal(const TrianglePolygon& polygon)
{
	XMFLOAT3 n = Cross((polygon.p1 - polygon.p0), (polygon.p2 - polygon.p0));

	const XMVECTOR nV = XMLoadFloat3(&n);

	const XMVECTOR lengthNV = XMVector3Length(nV);
	const float length = XMVectorGetX(lengthNV);
	if (length > 0.0001f) {
		n = n / length; // 正規化
	}
	
	return n;
}

//==================================
// 球体を当たった後の地点に動かす
//==================================
XMFLOAT3 ColliderComponent::moveSphere(const Segment& segment, const float& radius, const TrianglePolygon& polygon, const XMFLOAT3& contact, float& distance)
{
	// 線分の方向ベクトル
	XMFLOAT3 direction = segment.end - segment.start;
	XMVECTOR directionV = XMLoadFloat3(&direction);
	const XMVECTOR lengthNV = XMVector3Length(directionV);

	float length = XMVectorGetX(lengthNV);

	if (length > 0.0001f) {
		direction = direction / length; // 正規化
	}

	// 線分の長さが0の場合は計算不能
	if (length == 0.0f) return segment.start;

	const XMFLOAT3 sub = segment.start - contact;

	// 二次方程式を計算する (a*t^2 + b*t + c = 0)
	float a = 1.0f; // directionは正規化されているのでa=1
	float b = 2.0f * Dot((sub), direction);

	XMVECTOR subV = XMLoadFloat3(&sub);

	subV = XMVector3LengthSq(subV);

	const float subLengthSq = XMVectorGetX(subV);

	float c = subLengthSq - radius * radius;
	float discriminant = b * b - 4.0f * a * c;

	// 解があれば
	if (discriminant >= 0.0f) {

		// 解を求める
		float sqrt_discriminant = std::sqrt(discriminant);
		float t1 = (-b + sqrt_discriminant) / (2.0f * a);
		float t2 = (-b - sqrt_discriminant) / (2.0f * a);

		// 線分上の解を判定
		if (t1 < t2)
		{
			distance = t1;
			return  segment.start + t1 * direction;;
		}
		else
		{
			distance = t2;
			return  segment.start + t2 * direction;
		}
	}

	return segment.start;
}
XMFLOAT3 ColliderComponent::moveSphere(const Sphere& sphere, const TrianglePolygon& polygon, const XMFLOAT3& contact)
{
	const XMFLOAT3 v = sphere.center - contact;

	const XMVECTOR subV = XMLoadFloat3(&v);

	const XMVECTOR lengthSubV = XMVector3Length(subV);

	const float length = XMVectorGetX(lengthSubV);

	// 最終的にノーマライズする
	if(length > 0.0001f) {
		return contact + (v / length) * sphere.radius;
	}

	// 球の中心が三角形の面上にある場合、適当な方向に動かす
	return contact + XMFLOAT3(0.0f, sphere.radius, 0.0f);
}



// 線分に平行な与えられた幅の４角形の座標を求める
//void CalcQuadOrientedLine(
//	DirectX::XMFLOAT3 startpos,
//	DirectX::XMFLOAT3 endpos,
//	DirectX::XMFLOAT3* v,
//	float width)
//{
//	// 垂直なベクトルを求める
//	Vector3 zaxis(0, 0, 1);
//	Vector3 line = endpos - startpos;

//	Vector3 ans;
//	ans = line.Cross(zaxis); // 外積
//	ans.Normalize(); // 正規化

//	Vector3 startposplus = startpos + ans * width / 2.0f;
//	Vector3 startposminus = startpos - ans * width / 2.0f;

//	Vector3 endposplus = endpos + ans * width / 2.0f;
//	Vector3 endposminus = endpos - ans * width / 2.0f;

//	v[0] = startposplus;
//	v[1] = endposplus;

//	v[2] = startposminus;
//	v[3] = endposminus;
//}

//// 幅をもった線分（四角形）と球の当たり判定を行う
//bool CollisionSphereOrientedQuad(
//	Vector3 StartPos, // 開始位置
//	Vector3 EndPos,// 終了位置
//	float width, // 幅
//	float radius, // 半径
//	Vector3 CirclePos) // 中心座標
//{

//	Vector3 v[4]; // 四角形頂点
//	Vector3 intersectionpoint{}; // 交点

//	// 線分に平行な４角形の座標を求める
//	CalcQuadOrientedLine(StartPos, EndPos, v, width);

//	// 線分（４角形の線分）
//	Segment s[4];
//	s[0].startpoint = v[0];
//	s[0].endpoint = v[1];

//	s[1].startpoint = v[2];
//	s[1].endpoint = v[3];

//	s[2].startpoint = v[0];
//	s[2].endpoint = v[2];

//	s[3].startpoint = v[1];
//	s[3].endpoint = v[3];


//	float t;

//	// 四角形を囲む線分と点の距離を調べる
//	for (int idx = 0; idx < 4; idx++) {
//		// 円の中心と線分の距離を求める
//		float distance = calcPointSegmentDist(CirclePos, s[idx], intersectionpoint, t);

//		if (distance < radius) {

//			return true;
//		}
//	}

//	return false;

//}



//==================================
// AABBを設定
//==================================
AABB ColliderComponent::SetAABB(const DirectX::XMFLOAT3& centerposition, float width, float height, float depth)
{
	AABB aabb{};

	width = fabs(width);
	height = fabs(height);
	depth = fabs(depth);

	aabb.min.x = centerposition.x - width / 2.0f;
	aabb.min.y = centerposition.y - height / 2.0f;
	aabb.min.z = centerposition.z - depth / 2.0f;

	aabb.max.x = centerposition.x + width / 2.0f;
	aabb.max.y = centerposition.y + height / 2.0f;
	aabb.max.z = centerposition.z + depth / 2.0f;

	return aabb;
}
