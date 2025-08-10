#include "Collider.h"
#include "Transform.h"
#include "RigidBodyComponent.h"
#include <algorithm>  // std::min, std::maxのためのヘッダー
#include <cmath>     // fabsのためのヘッダー
#include <iostream>

using namespace DirectX::SimpleMath;

ColliderComponent::ColliderComponent(GameObject& obj) : Component(obj) {
	m_sortNum = COLLIDER; // ソート番号を設定
}

void ColliderComponent::Update()
{
	auto transform = p_object->GetComponent<TransformComponent>();

	auto pos = transform->GetPosition();
	auto scale = transform->GetScale();
	auto rot = transform->GetRotation();

	SetColliderSize_AABB(pos, scale);

	SetColliderSize_OBB(pos, scale, rot);

	MakeWorldAABBMatrix();
	MakeWorldOBBMatrix();
}

void ColliderComponent::MakeWorldAABBMatrix() {
	Vector3 outCenter = {};
	Vector3 outSize = {};

	outCenter.x = (coll_ab.min.x + coll_ab.max.x) * 0.5f;
	outCenter.y = (coll_ab.min.y + coll_ab.max.y) * 0.5f;
	outCenter.z = (coll_ab.min.z + coll_ab.max.z) * 0.5f;

	outSize.x = (coll_ab.max.x - coll_ab.min.x) * 0.5f;
	outSize.y = (coll_ab.max.y - coll_ab.min.y) * 0.5f;
	outSize.z = (coll_ab.max.z - coll_ab.min.z) * 0.5f;

	// クォータニオン作成
	Quaternion q = Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, 0.0f);

	// SRT情報作成
	Matrix r = Matrix::CreateFromQuaternion(q);
	Matrix s = Matrix::CreateScale(outSize);
	Matrix t = Matrix::CreateTranslation(outCenter);

	// ワールド行列を作成し、保存
	coll_ab.worldAABBMatrix = s * r * t;
	//	coll_ab.worldAABBMatrix = coll_ab.worldAABBMatrix.Transpose(); // 行列を転置
}

void ColliderComponent::MakeWorldOBBMatrix() {

	// ここについては協議の必要アリ
	float PitchRadians = DirectX::XMConvertToRadians(coll_ob.rotation.x); // X軸回転
	float YawRadians = DirectX::XMConvertToRadians(coll_ob.rotation.y);     // Y軸回転
	float RollRadians = DirectX::XMConvertToRadians(coll_ob.rotation.z);   // Z軸回転

	// クォータニオン作成
	Quaternion q = Quaternion::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);

	// SRT情報作成
	Matrix r = Matrix::CreateFromQuaternion(q);
	Matrix s = Matrix::CreateScale(coll_ob.size);
	Matrix t = Matrix::CreateTranslation(coll_ob.center);

	// ワールド行列を作成し、保存
	coll_ob.worldOBBMatrix = s * r * t;
	//	coll_ob.worldOBBMatrix = coll_ob.worldOBBMatrix.Transpose(); // 行列を転置
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
	Vector3 p;
	return CheckHit(line, polygon, p);
}
bool ColliderComponent::CheckHit(const Line& line, const TrianglePolygon& polygon, DirectX::SimpleMath::Vector3& contact)
{
	// 三角形の法線を計算
	Vector3 normal = GetNormal(polygon);

	// 線と法線が平行である場合
	float denom = Dot(normal, line.vec);
	if (fabs(denom) < 1e-6f) {
		return false; // 交差なし
	}

	// 線上の交点を計算
	float d = Dot(normal, polygon.p0);
	float t = (d - Dot(normal, line.point)) / denom;
	contact = line.point + t * line.vec;

	// 三角形内に交点があるかを判定
	return PointInTriangle(contact, polygon);
}

//==================================
// ■CheckHit関数
// 線分とポリゴンの当たり判定
//==================================
bool ColliderComponent::CheckHit(const Segment& segment, const TrianglePolygon& polygon)
{
	Vector3 p;
	return CheckHit(segment, polygon, p);
}
bool ColliderComponent::CheckHit(const Segment& segment, const TrianglePolygon& polygon, Vector3& contact)
{
	PlaneNormal plane(polygon.p0, GetNormal(polygon));

	if (CheckHit(segment, plane))
	{
		// 線分と平面の交点位置を計算
		float denom = Dot(plane.normal, segment.end - segment.start);
		float t = Dot(plane.normal, plane.point - segment.start) / denom;

		// tが0から1の範囲内であれば線分と平面が交差する
		if (t >= 0.0f && t <= 1.0f) {
			contact = segment.start + t * (segment.end - segment.start); // 交点を計算

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
	Vector3 p;
	return CheckHit(sphere, polygon, p);
}
bool ColliderComponent::CheckHit(const Sphere& sphere, const TrianglePolygon& polygon, Vector3& contact)
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
bool ColliderComponent::CheckHit(Sphere sphere1, Sphere sphere2)
{
	Vector3 p;
	return CheckHit(sphere1, sphere2, p);
}
bool ColliderComponent::CheckHit(Sphere sphere1, Sphere sphere2, Vector3& contact)
{
	float len2 = (sphere1.center - sphere2.center).LengthSquared();
	float r2 = (sphere1.radius + sphere2.radius) * (sphere1.radius + sphere2.radius);
	if (r2 > len2) {

		// 接触点を計算 ※sphere1がsphere2に後からぶつかって来たとしている
		Vector3 v = (sphere1.center - sphere2.center);
		v.Normalize();
		contact = sphere2.center + v * sphere2.radius;

		return true;
	}
	return false;
}

//==================================
// ■CheckHit関数
// AABBとAABBの当たり判定
//==================================
bool ColliderComponent::CheckHit_CubeAndCube_IsTrigger3D(AABB p1, AABB p2) {

	// X座標
	if (p1.max.x < p2.min.x) {
		return false;
	}

	if (p1.min.x > p2.max.x) {
		return false;
	}

	// Y座標
	if (p1.max.y < p2.min.y) {
		return false;
	}

	if (p1.min.y > p2.max.y) {
		return false;
	}

	// Z座標
	if (p1.max.z < p2.min.z) {
		return false;
	}

	if (p1.min.z > p2.max.z) {
		return false;
	}

	return true;
}

//==================================
// ■CheckHit関数
// AABBとAABBの当たり判定（2D用）
// 戻しアリ
//==================================
bool ColliderComponent::CheckHit_CubeAndCube_NoTrigger2D(const AABB& p1, const AABB& p2, DirectX::XMFLOAT3& pos) {

	// そもそも衝突しているかどうかのチェック
	// 奥行きからの敵の登場のことも考えて、念のため
	// Z軸も考慮した当たり判定とする
	// 当たって押し出されるのはX軸とY軸みたいな
	bool check = CheckHit_CubeAndCube_IsTrigger3D(p1, p2);

	if (check == true) {
		// 最終的のオブジェクトを押し戻すためのベクトル
		DirectX::XMFLOAT3 pushBack(0.0f, 0.0f, 0.0f);

		// X軸での「めり込み量」を計算
		float dx1 = (p1.max.x - p2.min.x);	// p1の右端 - p2の左端（p2が左から押し込んでいる時）
		float dx2 = (p1.min.x - p2.max.x);	// p1の左端 - p2の右端（p2が右から押し込んでいる時）

		// Y軸での「めり込み量」を計算
		float dy1 = (p1.max.y - p2.min.y);	// p1の上端 - p2の下端（Aが上から押し込んでいる時）
		float dy2 = (p1.min.y - p2.max.y);	// p1の下端 - p2の上端（Aが下から押し込んでいる時）

		// X軸の処理
		// X軸方向でどちらに押し戻すべきか（小さい方が自然）
		if (abs(dx1) < abs(dx2)) {
			pushBack.x = dx1;	// 左から来たので右に押し戻し
		}
		else {
			pushBack.x = dx2;	// 右から来たので左に押し戻し
		}

		// Y軸の処理
		// Y軸方向でどちらに押し戻すべきか（小さい方が自然）
		if (abs(dy1) < abs(dy2)) {
			pushBack.y = dy1;	// 下から来たので上に押し戻し
		}
		else {
			pushBack.y = dy2;	// 上から来たので下に押し戻し
		}

		// Z軸は完全に無視（2.5Dのため）
		pushBack.z = 0.0f;

		// X軸とY軸のうち、より「めり込みが少ない」軸でのみ押し戻す
		// →壁にぶつかったとき、斜めではなく「垂直な方向」だけで修正されるようにするため
		if (abs(pushBack.x) < abs(pushBack.y)) {
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
bool ColliderComponent::CheckHit_CubeAndCube_IsTrigger2D_Normal(const ColliderComponent& p1, const ColliderComponent& p2, Vector3& hitNormal) {
	AABB coll1 = p1.coll_ab;
	AABB coll2 = p2.coll_ab;

	bool hit = CheckHit_CubeAndCube_IsTrigger3D(coll1, coll2);

	if (hit == false) {
		// 衝突していない場合は法線をゼロベクトルに設定
		hitNormal = { 0.0f, 0.0f, 0.0f };
		return false;
	}

	// 最終的のオブジェクトを押し戻すためのベクトル
	DirectX::XMFLOAT3 pushBack(0.0f, 0.0f, 0.0f);

	// X軸での「めり込み量」を計算
	float dx1 = (coll1.max.x - coll2.min.x);	// p1の右端 - p2の左端（p2が左から押し込んでいる時）
	float dx2 = (coll1.min.x - coll2.max.x);	// p1の左端 - p2の右端（p2が右から押し込んでいる時）

	// Y軸での「めり込み量」を計算
	float dy1 = (coll1.max.y - coll2.min.y);	// p1の上端 - p2の下端（p2が上から押し込んでいる時）
	float dy2 = (coll1.min.y - coll2.max.y);	// p1の下端 - p2の上端（p2が下から押し込んでいる時）

	// X軸の処理
	// X軸方向でどちらに押し戻すべきか（小さい方が自然）
	if (abs(dx1) < abs(dx2)) {
		pushBack.x = dx1;	// 左から来たので右に押し戻し
	}
	else {
		pushBack.x = dx2;	// 右から来たので左に押し戻し
	}

	// Y軸の処理
	// Y軸方向でどちらに押し戻すべきか（小さい方が自然）
	if (abs(dy1) < abs(dy2)) {
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
	if (abs(pushBack.x) < abs(pushBack.y)) {
		pushBack.y = 0.0f;	// 横方向で押し戻す

		// X方向に押し戻された → 壁
		hitNormal.x = (pushBack.x > 0.0f) ? -1.0f : 1.0f;
	}
	else {
		pushBack.x = 0.0f;	// 縦方向で押し戻す

		// Y方向に押し戻された → 地面 or 天井
		hitNormal.y = (pushBack.y > 0.0f) ? -1.0f : 1.0f;
	}

	return true;
}

//==================================
// ■CheckHit関数
// AABBとAABBの当たり判定（2D用）
// どこから当たっているのか含む
// 戻しアリ
//==================================
bool ColliderComponent::CheckHit_CubeAndCube_NoTrigger2D_Normal(const ColliderComponent& p1, const ColliderComponent& p2, Vector3& hitNormal) {
	AABB coll1 = p1.coll_ab;
	AABB coll2 = p2.coll_ab;

	// そもそも衝突しているかどうかのチェック
	// 奥行きからの敵の登場のことも考えて、念のため
	// Z軸も考慮した当たり判定とする
	// 当たって押し出されるのはX軸とY軸みたいな
	bool check = CheckHit_CubeAndCube_IsTrigger3D(coll1, coll2);

	if (check == true) {

		auto returnDir = Vector3::Zero;
		auto it = touchObjects.find(p1.p_object);
		if (it != touchObjects.end()) {

			if (it->second != Vector3::Zero) {
				hitNormal = it->second;
				returnDir = hitNormal;
			}
		}

		// 最終的のオブジェクトを押し戻すためのベクトル
		Vector3 pushBack(0.0f, 0.0f, 0.0f);

		// X軸での「めり込み量」を計算
		float dx1 = (coll1.max.x - coll2.min.x);	// p1の右端 - p2の左端（p2が左から押し込んでいる時）
		float dx2 = (coll1.min.x - coll2.max.x);	// p1の左端 - p2の右端（p2が右から押し込んでいる時）

		// Y軸での「めり込み量」を計算
		float dy1 = (coll1.max.y - coll2.min.y);	// p1の上端 - p2の下端（p2が上から押し込んでいる時）
		float dy2 = (coll1.min.y - coll2.max.y);	// p1の下端 - p2の上端（p2が下から押し込んでいる時）

		// X軸の処理
		// X軸方向でどちらに押し戻すべきか（小さい方が自然）
		if (abs(dx1) < abs(dx2)) {
			pushBack.x = dx1;	// 左から来たので右に押し戻し
		}
		else {
			pushBack.x = dx2;	// 右から来たので左に押し戻し
		}

		// Y軸の処理
		// Y軸方向でどちらに押し戻すべきか（小さい方が自然）
		if (abs(dy1) < abs(dy2)) {
			pushBack.y = dy1;	// 上から来たので下に押し戻し
		}
		else {
			pushBack.y = dy2;	// 下から来たので上に押し戻し
		}

		// Z軸は完全に無視（2.5Dのため）
		pushBack.z = 0.0f;

		if (returnDir == Vector3::Zero) {

			// X軸とY軸のうち、より「めり込みが少ない」軸でのみ押し戻す
			// →壁にぶつかったとき、斜めではなく「垂直な方向」だけで修正されるようにするため
			if (abs(pushBack.x) < abs(pushBack.y)) {
				pushBack.y = 0.0f;	// 横方向で押し戻す

				// X方向に押し戻された → 壁
				hitNormal.x = (pushBack.x > 0.0f) ? -1.0f : 1.0f;
			}
			else {
				auto collsize = coll1.max.x - coll1.min.x;

				pushBack.x = 0.0f;	// 縦方向で押し戻す

				// Y方向に押し戻された → 地面 or 天井
				hitNormal.y = (pushBack.y > 0.0f) ? -1.0f : 1.0f;
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
		touchObjects.insert({ p1.p_object,hitNormal });
		touchObjects[p1.p_object] = hitNormal;

		// 位置補正をして、SRT情報を再計算
		auto transform = p_object->GetComponent<TransformComponent>();
		transform->AddPosition({ pushBack });
		//		transform->MakeWorldMatrix();

		auto pos = transform->GetPosition();
		auto size = transform->GetScale();
		SetColliderSize_AABB(pos, size);
		MakeWorldAABBMatrix();
	}
	else {
		// 存在確認をして、second情報をリセット上書き
		auto it = touchObjects.find(p1.p_object);
		if (it != touchObjects.end()) {
			it->second = Vector3::Zero;
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
bool ColliderComponent::CheckHit_CubeAndCube_NoTrigger3D(const AABB& p1, const AABB& p2, DirectX::XMFLOAT3& pos) {

	bool check = CheckHit_CubeAndCube_IsTrigger3D(p1, p2);

	if (check == true) {
		// 最終的のオブジェクトを押し戻すためのベクトル
		DirectX::XMFLOAT3 pushBack(0.0f, 0.0f, 0.0f);

		// 各軸での「めり込み量」を計算
		float dx1 = (p1.max.x - p2.min.x);	// Bの右端 - Aの左端（Aが左から押し込んでいる時）
		float dx2 = (p1.min.x - p2.max.x);	// Bの右端 - Aの左端（Aが右から押し込んでいる時）

		float dy1 = (p1.max.y - p2.min.y);	// Bの右端 - Aの左端（Aが下から押し込んでいる時）
		float dy2 = (p1.min.y - p2.max.y);	// Bの右端 - Aの左端（Aが上から押し込んでいる時）

		float dz1 = (p1.max.z - p2.min.z);	// Bの右端 - Aの左端（Aが奥から押し込んでいる時）
		float dz2 = (p1.min.z - p2.max.z);	// Bの右端 - Aの左端（Aが前から押し込んでいる時）

		// X軸の処理
		if (abs(dx1) < abs(dx2))
		{
			pushBack.x = dx1;
		}
		else {
			pushBack.x = dx2;
		}

		// Y軸の処理
		if (abs(dy1) < abs(dy2)) {
			pushBack.y = dy1;
		}
		else {
			pushBack.y = dy2;
		}

		// Z軸の処理
		if (abs(dz1) < abs(dz2)) {
			pushBack.z = dz1;
		}
		else {
			pushBack.z = dz2;
		}

		// 最小のめり込み量の軸を1つ選んで、その軸にだけ押し戻しを適用（自然な反応にする）
		float absX = abs(pushBack.x);
		float absY = abs(pushBack.y);
		float absZ = abs(pushBack.z);

		// 最小軸を選ぶ
		if (absX <= absY && absX <= absZ) {	// X軸が最小
			pushBack.y = 0.0f;
			pushBack.z = 0.0f;
		}
		else if (absY <= absX && absY <= absZ) {	// Y軸が最小
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
		float origin = rayOrigin.m128_f32[i];
		float dir = rayDir.m128_f32[i];
		float bmin = (&hit.min.x)[i];	// AABBの最小値
		float bmax = (&hit.max.x)[i];

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

bool ColliderComponent::CheckHit_SphereAndSphere_IsTrigger3D(const Sphere& p1, const Sphere& p2) {
	float dx = p1.center.x - p2.center.x;	// ２つのオブジェクトの中心位置の差分
	float dy = p1.center.y - p2.center.y;	// ２つのオブジェクトの中心位置の差分
	float dz = p1.center.z - p2.center.z;	// ２つのオブジェクトの中心位置の差分

	float distSq = dx * dx + dy * dy + dz * dz;
	float radiusSum = p1.radius + p2.radius;	// ２つのオブジェクトの半径を足す

	// 半径から距離を比較
	return distSq < (radiusSum * radiusSum);
}

// 検知と押し出し
bool ColliderComponent::CheckHit_SphereAndSphere_NoTrigger2D(const Sphere& p1, const Sphere& p2, DirectX::XMFLOAT3& pos) {
	// p2が押す方、p1が押される方とする

	float dx = p2.center.x - p1.center.x;	// ２つのオブジェクトの中心位置の差分
	float dy = p2.center.y - p1.center.y;	// ２つのオブジェクトの中心位置の差分
	float dz = p2.center.z - p1.center.z;	// ２つのオブジェクトの中心位置の差分

	float distSq1 = dx * dx + dy * dy;
	float distSq2 = dx * dx + dy * dy + dz * dz;
	float radiusSum = p1.radius + p2.radius;	// ２つのオブジェクトの半径を足す

	if (distSq2 < (radiusSum * radiusSum)) {	// 念のため、Z軸も当たっているかを確認
		float dist = std::sqrt(distSq1);		// ２つのオブジェクトの中心位置の距離（平方根を使い算出）
		float overlap = radiusSum - dist;	// ２つのオブジェクトの半径の合計から距離を引いて、重なり量を算出

		// 正規化
		if (dist > 0.0f) {
			// 方向ベクトル（p2 → p1の方向）を正規化して逆向きに押し出す
			float nx = dx / dist;
			float ny = dy / dist;

			// p1をp2から遠ざける方向に押し出す（当たってきた方向の逆）
			pos.x = p2.center.x + nx * overlap;
			pos.y = p2.center.y + ny * overlap;
			//pos.z = 0.0f;	// 2.5DなのでZ軸は無視
		}
		else {
			// 中心が完全一致 → 適切な方向に話す
			pos = DirectX::XMFLOAT3(0.0f, radiusSum, 0.0f);
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
	DirectX::XMFLOAT3 closestPoint = {
		std::clamp(p1.center.x, p2.min.x, p2.max.x),
		std::clamp(p1.center.y, p2.min.y, p2.max.y),
		std::clamp(p1.center.z, p2.min.z, p2.max.z),
	};

	float dx = p1.center.x - closestPoint.x;	// ２つのオブジェクトの中心位置の差分
	float dy = p1.center.y - closestPoint.y;	// ２つのオブジェクトの中心位置の差分
	float dz = p1.center.z - closestPoint.z;	// ２つのオブジェクトの中心位置の差分

	float distSq = dx * dx + dy * dy + dz * dz;	// ２つのオブジェクトの中心位置の距離の二乗
	float r = p1.radius;	// 球体の半径

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
	DirectX::XMFLOAT3 closestPoint = {
		std::clamp(p1.center.x, p2.min.x, p2.max.x),
		std::clamp(p1.center.y, p2.min.y, p2.max.y),
		std::clamp(p1.center.z, p2.min.z, p2.max.z),
	};

	float dx = p1.center.x - closestPoint.x;	// ２つのオブジェクトの中心位置の差分
	float dy = p1.center.y - closestPoint.y;	// ２つのオブジェクトの中心位置の差分
	float dz = p1.center.z - closestPoint.z;	// ２つのオブジェクトの中心位置の差分

	float distSq1 = dx * dx + dy * dy;	// ２つのオブジェクトの中心位置の距離の二乗
	float distSq2 = dx * dx + dy * dy + dz * dz;	// ２つのオブジェクトの中心位置の距離の二乗
	float r = p1.radius;	// 球体の半径

	if (distSq2 < (r * r)) {	// 半径から距離を比較
		float dist = std::sqrt(distSq1);	// ２つのオブジェクトの中心位置の距離（平方根を使い算出）
		float overlap = r - dist;	// ２つのオブジェクトの半径から距離を引いて、重なり量を算出

		if (dist > 0.0001f) {
			// 方向ベクトル（p2 → p1の方向）を正規化して逆向きに押し出す
			float nx = dx / dist;
			float ny = dy / dist;

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

// 触れているかどうかだけを調べる
bool ColliderComponent::CubeAndCubeCheck_OBB(const OBB& col1, const OBB& col2) {
	DirectX::BoundingOrientedBox obb1;
	DirectX::BoundingOrientedBox obb2;

	// 回転角をラジアンに変換
	float col1PitchRadians = DirectX::XMConvertToRadians(col1.rotation.x); // X軸回転
	float col1YawRadians = DirectX::XMConvertToRadians(col1.rotation.y);     // Y軸回転
	float col1RollRadians = DirectX::XMConvertToRadians(col1.rotation.z);   // Z軸回転

	float col2PitchRadians = DirectX::XMConvertToRadians(col2.rotation.x); // X軸回転
	float col2YawRadians = DirectX::XMConvertToRadians(col2.rotation.y);     // Y軸回転
	float col2RollRadians = DirectX::XMConvertToRadians(col2.rotation.z);   // Z軸回転

	// OBB1の設定
	obb1.Center = DirectX::XMFLOAT3(col1.center.x, col1.center.y, col1.center.z);
	obb1.Extents = DirectX::XMFLOAT3(col1.size.x / 2.0f, col1.size.y / 2.0f, col1.size.z / 2.0f);

	// OBB1の回転
	DirectX::XMVECTOR quaternion1 = DirectX::XMQuaternionNormalize(
		DirectX::XMQuaternionRotationRollPitchYaw(col1PitchRadians, col1YawRadians, col1RollRadians)
	);
	DirectX::XMStoreFloat4(&obb1.Orientation, quaternion1);

	// OBB2の設定
	obb2.Center = DirectX::XMFLOAT3(col2.center.x, col2.center.y, col2.center.z);
	obb2.Extents = DirectX::XMFLOAT3(col2.size.x / 2.0f, col2.size.y / 2.0f, col2.size.z / 2.0f);

	// OBB2の回転
	DirectX::XMVECTOR quaternion2 = DirectX::XMQuaternionNormalize(
		DirectX::XMQuaternionRotationRollPitchYaw(col2PitchRadians, col2YawRadians, col2RollRadians)
	);
	DirectX::XMStoreFloat4(&obb2.Orientation, quaternion2);

	// 衝突判定
	return obb1.Intersects(obb2);
}

// 触れているかを調べ、めり込みを直す
bool ColliderComponent::CubeAndCubeHit_OBB(const OBB& col1, const OBB& col2) {
	DirectX::BoundingOrientedBox obb1;
	DirectX::BoundingOrientedBox obb2;

	// 回転角をラジアンに変換
	float col1PitchRadians = DirectX::XMConvertToRadians(col1.rotation.x); // X軸回転
	float col1YawRadians = DirectX::XMConvertToRadians(col1.rotation.y);     // Y軸回転
	float col1RollRadians = DirectX::XMConvertToRadians(col1.rotation.z);   // Z軸回転

	float col2PitchRadians = DirectX::XMConvertToRadians(col2.rotation.x); // X軸回転
	float col2YawRadians = DirectX::XMConvertToRadians(col2.rotation.y);     // Y軸回転
	float col2RollRadians = DirectX::XMConvertToRadians(col2.rotation.z);   // Z軸回転

	// OBB1の設定
	obb1.Center = DirectX::XMFLOAT3(col1.center.x, col1.center.y, col1.center.z);
	obb1.Extents = DirectX::XMFLOAT3(col1.size.x / 2.0f, col1.size.y / 2.0f, col1.size.z / 2.0f);

	// OBB1の回転
	DirectX::XMVECTOR quaternion1 = DirectX::XMQuaternionNormalize(
		DirectX::XMQuaternionRotationRollPitchYaw(col1PitchRadians, col1YawRadians, col1RollRadians)
	);
	DirectX::XMStoreFloat4(&obb1.Orientation, quaternion1);

	// OBB2の設定
	obb2.Center = DirectX::XMFLOAT3(col2.center.x, col2.center.y, col2.center.z);
	obb2.Extents = DirectX::XMFLOAT3(col2.size.x / 2.0f, col2.size.y / 2.0f, col2.size.z / 2.0f);

	// OBB2の回転
	DirectX::XMVECTOR quaternion2 = DirectX::XMQuaternionNormalize(
		DirectX::XMQuaternionRotationRollPitchYaw(col2PitchRadians, col2YawRadians, col2RollRadians)
	);
	DirectX::XMStoreFloat4(&obb2.Orientation, quaternion2);

	// 衝突判定
	return obb1.Intersects(obb2);
}

bool ColliderComponent::CheckHit_AABBAndOBB_IsTrigger3D(const AABB& aabb, const OBB& obb) {

	auto AABBCenter = (aabb.max + aabb.min) * 0.5f;
	auto OBBCenter = obb.center;

	Vector3 vecDistance = AABBCenter - OBBCenter;

	const Vector3* obbAxes[3] = { &obb.axisX, &obb.axisY, &obb.axisZ };
	const Vector3 aabbAxes[3] = { Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1) };

	// OBBの3軸
	for (int i = 0; i < 3; i++) {
		if (!CompareLengthOBBvsAABB(obb, aabb, *obbAxes[i], vecDistance)) return false;
	}

	// AABBの3軸
	for (int i = 0; i < 3; i++) {
		if (!CompareLengthOBBvsAABB(obb, aabb, aabbAxes[i], vecDistance)) return false;
	}

	// 外積9本
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Vector3 axis = obbAxes[i]->Cross(aabbAxes[j]);
			float lengthSq = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
			if (lengthSq < 1e-6f) continue; // 平行なら無視
			axis.Normalize();
			if (!CompareLengthOBBvsAABB(obb, aabb, axis, vecDistance)) return false;
		}
	}

	return true;
}

bool ColliderComponent::CompareLengthOBBvsAABB(
	const OBB& obb,
	const AABB& aabb,
	const Vector3& axis,
	const Vector3& vecDistance)
{
	// 軸がほぼゼロベクトルなら比較不要
	if (axis.LengthSquared() < 1e-6f)
		return true;

	Vector3 axisN = axis; // コピー
	axisN.Normalize();

	// OBBの半径（各軸方向の投影長を合計）
	float rOBB =
		fabsf(obb.size.x  * axisN.Dot(obb.axisX)) +
		fabsf(obb.size.y  * axisN.Dot(obb.axisY)) +
		fabsf(obb.size.z  * axisN.Dot(obb.axisZ));

	// AABBの半径（軸はワールド固定X,Y,Z）
	Vector3 aabbAxisX(1, 0, 0);
	Vector3 aabbAxisY(0, 1, 0);
	Vector3 aabbAxisZ(0, 0, 1);

	float extX = (aabb.max.x - aabb.min.x) * 0.5f;
	float extY = (aabb.max.y - aabb.min.y) * 0.5f;
	float extZ = (aabb.max.z - aabb.min.z) * 0.5f;

	float rAABB =
		fabsf(extX * axisN.Dot(aabbAxisX)) +
		fabsf(extY * axisN.Dot(aabbAxisY)) +
		fabsf(extZ * axisN.Dot(aabbAxisZ));

	// 中心間距離を投影
	float dist = fabsf(axisN.Dot(vecDistance));

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
float ColliderComponent::Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//==================================
// ■Cross関数
// ベクトルの外積を求める
//==================================
Vector3 ColliderComponent::Cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3(
		v1.y * v2.z - v1.z * v2.y, // x成分
		v1.z * v2.x - v1.x * v2.z, // y成分
		v1.x * v2.y - v1.y * v2.x); // z成分
}

//==================================
// ■ClosestPointOnSegment関数
// 点から線分までの最近地点
//==================================
Vector3 ColliderComponent::ClosestPointOnSegment(const Vector3& point, const Segment& segment)
{
	// 線分のベクトルを計算（終点 - 始点）
	Vector3 vec = segment.end - segment.start;

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
		Vector3 ab = segment.end - segment.start; // 線分のベクトル
		float lengthSq = ab.LengthSquared(); // 線分の長さの二乗

		// 線分が退化（両端が同じ点）している場合
		if (lengthSq == 0.0f) {
			return segment.start; // 線分の端点を返す
		}

		// 点が線分上に投影される位置を計算（0 <= t <= 1 の範囲）
		float t = (point - segment.start).Dot(ab) / lengthSq;
		t = std::clamp(t, 0.0f, 1.0f);

		// 線分上の最近接点を返す
		return (segment.start + t * ab);
	}
}

//==================================
// ■DistanceSquaredPointToSegment関数
// 点から線分までの距離の2乗
//==================================
float ColliderComponent::DistanceSquaredPointToSegment(const Vector3& point, const Segment& segment)
{
	Vector3 p;
	return DistanceSquaredPointToSegment(point, segment, p);
}
float ColliderComponent::DistanceSquaredPointToSegment(const Vector3& point, const Segment& segment, Vector3& contact)
{
	contact = ClosestPointOnSegment(point, segment);

	return (point - contact).LengthSquared();
}

//==================================
// ■DistancePointToSegment関数
// 点から線分までの距離
//==================================
float ColliderComponent::DistancePointToSegment(const Vector3& point, const Segment& segment)
{
	Vector3 p;
	return DistancePointToSegment(point, segment, p);
}
float ColliderComponent::DistancePointToSegment(const Vector3& point, const Segment& segment, Vector3& contact)
{
	contact = ClosestPointOnSegment(point, segment);

	return (point - contact).Length();
}

//==================================
// ■DistancePointToPlane関数
// 点から平面までの距離
//==================================
float ColliderComponent::DistancePointToPlane(const Vector3& point, const PlaneNormal& plane)
{
	return fabs(Dot((point - plane.point), plane.normal) / Dot(plane.normal, plane.normal));
}

//==================================
// ■ProjectPointToPlane関数
// 点から平面に下ろした垂線の交点
//==================================
Vector3 ColliderComponent::ProjectPointToPlane(const Vector3& point, const PlaneNormal& plane)
{
	double t = -Dot((point - plane.point), plane.normal) / Dot(plane.normal, plane.normal);

	// 交点の座標を計算
	return  point + (plane.normal * (float)t);
}


//==================================
// ■PointInTriangle関数
// 三角形の内部かどうかを判定する
//==================================
bool ColliderComponent::PointInTriangle(const Vector3& point, const TrianglePolygon& polygon)
{
	// 3辺のベクトル
	Vector3 ab = polygon.p1 - polygon.p0;
	Vector3 bc = polygon.p2 - polygon.p1;
	Vector3 ca = polygon.p0 - polygon.p2;

	// 内部の点とのベクトル
	Vector3 ap = point - polygon.p0;
	Vector3 bp = point - polygon.p1;
	Vector3 cp = point - polygon.p2;

	// 3辺と内部の点との法線ベクトル
	Vector3	n1 = Cross(ab, ap);
	Vector3	n2 = Cross(bc, bp);
	Vector3	n3 = Cross(ca, cp);

	// 三角形の法線ベクトル
	Vector3	normal = Cross(ab, bc);

	float dot = n1.Dot(normal);
	if (dot < 0) return false; // 為す角度が鈍角

	dot = n2.Dot(normal);
	if (dot < 0) return false; // 為す角度が鈍角

	dot = n3.Dot(normal);
	if (dot < 0) return false; // 為す角度が鈍角

	return true;
}


//==================================
// ■ClosestPointOnTriangle関数
// 点と三角形間の最近接点を求める
//==================================
Vector3 ColliderComponent::ClosestPointOnTriangle(const Vector3& point, const TrianglePolygon& polygon)
{
	PlaneNormal plane(polygon.p0, GetNormal(polygon));

	// 線分と平面の交点を計算
	Vector3 p = ProjectPointToPlane(point, plane);

	// 交点が三角形ポリゴンの内側にあるか判定
	if (PointInTriangle(p, polygon))
	{
		return p;
	}

	// 交点が三角形ポリゴンの外側なら各辺との距離を計算
	Vector3 p1, p2, p3;
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
Vector3 ColliderComponent::GetNormal(const TrianglePolygon& polygon)
{
	Vector3 n = Cross((polygon.p1 - polygon.p0), (polygon.p2 - polygon.p0));
	n.Normalize();
	return n;
}

//==================================
// 球体を当たった後の地点に動かす
//==================================
Vector3 ColliderComponent::moveSphere(const Segment& segment, const float& radius, const TrianglePolygon& polygon, const Vector3& contact, float& distance)
{
	// 線分の方向ベクトル
	Vector3 direction = segment.end - segment.start;
	float length = direction.Length();
	direction.Normalize();

	// 線分の長さが0の場合は計算不能
	if (length == 0.0f) return segment.start;

	// 二次方程式を計算する (a*t^2 + b*t + c = 0)
	float a = 1.0f; // directionは正規化されているのでa=1
	float b = 2.0f * Dot((segment.start - contact), direction);
	float c = (segment.start - contact).LengthSquared() - radius * radius;
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
Vector3 ColliderComponent::moveSphere(const Sphere& sphere, const TrianglePolygon& polygon, const Vector3& contact)
{
	Vector3 v = sphere.center - contact;
	v.Normalize();

	return contact + v * sphere.radius;
}



// 線分に平行な与えられた幅の４角形の座標を求める
//void CalcQuadOrientedLine(
//	DirectX::SimpleMath::Vector3 startpos,
//	DirectX::SimpleMath::Vector3 endpos,
//	DirectX::SimpleMath::Vector3* v,
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
AABB ColliderComponent::SetAABB(Vector3 centerposition, float width, float height, float depth)
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
