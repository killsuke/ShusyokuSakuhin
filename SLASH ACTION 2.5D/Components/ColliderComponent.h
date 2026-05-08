#pragma once

#include <DirectXMath.h>
#include <directxcollision.h>	// Collision の変更を一部する、XMFLOAT4で扱うのが本当は良いらしい。クオータニオンの始まりか？
#include <unordered_map>
#include "Component.h"
#include "TransformComponent.h"

// ライン（無限の長さの線）
struct Line {
	DirectX::XMFLOAT3 point; // 通過地点
	DirectX::XMFLOAT3 vec; // 線の方向ベクトル
};

// プレーン(無限の広さの平面)
struct PlaneNormal {
	DirectX::XMFLOAT3 point; // 平面上の1点
	DirectX::XMFLOAT3 normal; // 平面の法線ベクトル
	//float d;         // 平面の方程式: ax + by + cz + d = 0 の d
};

// セグメント（有限の長さの線分）
struct Segment {
	DirectX::XMFLOAT3 start; // 始点
	DirectX::XMFLOAT3 end; // 終点
};

// 三角形ポリゴン（有限の広さの平面）
struct TrianglePolygon {
	const DirectX::XMFLOAT3 p0; //頂点0
	const DirectX::XMFLOAT3 p1; //頂点1
	const DirectX::XMFLOAT3 p2; //頂点2
};

// 球体
struct Sphere {
	DirectX::XMFLOAT3 center = {}; // 中心
	float radius = 0.0f; // 半径
};

//// 円柱
//struct Cylinder {
//	DirectX::XMFLOAT3 top; // 上
//	DirectX::XMFLOAT3 bottom; // 底
//	float radius; // 半径
//};

//// カプセル
//struct Capsule {
//	DirectX::XMFLOAT3 top; // 上の中心 
//	DirectX::XMFLOAT3 bottom; // 底の中心
//	float radius; // 半径
//};

struct PushNormalize {
	GameObject* dirObj = nullptr;

	DirectX::XMFLOAT3 pushDir = {};
};

// BOX定義
struct AABB {
	DirectX::XMFLOAT3 min = {};
	DirectX::XMFLOAT3 max = {};
	DirectX::XMFLOAT3 offsetCenter = {};
	DirectX::XMFLOAT3 offsetSize = {};

	DirectX::XMFLOAT3 beforeMin = {};
	DirectX::XMFLOAT3 beforeMax = {};
	DirectX::XMFLOAT3 beforeOffsetCenter = {};
	DirectX::XMFLOAT3 beforeOffsetSize = {};

	// ワールド行列
	DirectX::XMMATRIX worldAABBMatrix = DirectX::XMMatrixIdentity();

	/*bool none = false;
	bool left = false;
	bool right = false;
	bool top = false;
	bool bottom = false;
	bool front = false;
	bool back = false;*/
};

// 当たり判定でのみ使う
struct OBB {
	DirectX::XMFLOAT3 center = {};
	DirectX::XMFLOAT3 size = {};
	DirectX::XMVECTOR rotation = { 0.0f,0.0f,0.0f,1.0f };
	DirectX::XMFLOAT3 offsetCenter = {};
	DirectX::XMFLOAT3 offsetSize = {};
	DirectX::XMVECTOR offsetQuat = { 0.0f,0.0f,0.0f,1.0f };
	DirectX::XMFLOAT3 axisX = {};
	DirectX::XMFLOAT3 axisY = {};
	DirectX::XMFLOAT3 axisZ = {};

	DirectX::XMFLOAT3 drawCenter = {};

	DirectX::XMVECTOR axisQuat = {};
	DirectX::XMMATRIX worldOBBMatrix = DirectX::XMMatrixIdentity();
};

class ColliderComponent : public Component {
protected:

private:
	AABB m_BeforeColl_AABB; // 前回のAABBの当たり判定用
	AABB m_Coll_AABB; // AABBの当たり判定用
	OBB m_Coll_OBB; // OBBの当たり判定用
	Sphere m_Coll_Sphere; // 球体の当たり判定用

	GameObject* m_BeforeTouchObj = nullptr;	// 前回のタッチオブジェクト

	bool m_ActiveColliderFlag = true; // コライダーの有効・無効フラグ

	DirectX::XMFLOAT2 m_BeforeTouchAxis = {}; // 前回のタッチ軸

	std::unordered_map<GameObject*, DirectX::XMFLOAT3> m_TouchObjects; // タッチしているオブジェクトのリスト

public:
	ColliderComponent() = default;
	ColliderComponent(GameObject& obj);
	~ColliderComponent() = default;

	void Update() override;

	//当たり判定
	bool CheckHit(const Line& line, const PlaneNormal& plane); //線(無限の長さ)と平面(無限の大きさ)
	bool CheckHit(const Segment& segment, const PlaneNormal& plane); //線分と平面(無限の大きさ)
	bool CheckHit(const Line& line, const TrianglePolygon& polygon); //線(無限の長さ)とポリゴン
	bool CheckHit(const Line& line, const TrianglePolygon& polygon, DirectX::XMFLOAT3& contact); //同上
	bool CheckHit(const Segment& segment, const TrianglePolygon& polygon); //線分とポリゴン
	bool CheckHit(const Segment& segment, const TrianglePolygon& polygon, DirectX::XMFLOAT3& contact); //同上
	bool CheckHit(const Sphere& sphere, const PlaneNormal& plane); //球体と平面(無限の大きさ)
	bool CheckHit(const Sphere& sphere, const TrianglePolygon& polygon); //球体とポリゴン
	bool CheckHit(const Sphere& sphere, const TrianglePolygon& polygon, DirectX::XMFLOAT3& contact); //同上
	//bool CheckHit(const Cylinder& cylinder, const Plane& plane); //円柱と平面
	//bool CheckHit(const Cylinder& cylinder, const Polygon& polygon); //円柱とポリゴン
	//bool CheckHit(const Cylinder& cylinder, const Polygon& polygon, DirectX::XMFLOAT3& contact); //同上
	//bool CheckHit(const Capsule& capsule, const Plane& plane); //カプセルと平面(無限の大きさ)
	//bool CheckHit(const Capsule& capsule, const Polygon& polygon); //カプセルとポリゴン
	//bool CheckHit(const Capsule& capsule, const Polygon& polygon, DirectX::XMFLOAT3& contact); //同上
	bool CheckHit(const Sphere& sphere1, const Sphere& sphere2); //球体と球体
	bool CheckHit(const Sphere& sphere1, const Sphere& sphere2, DirectX::XMFLOAT3& contact); //同上

	// 直方体どうしの当たり判定
	// 触れているかどうかだけを検知
	bool CheckHit_CubeAndCube_IsTrigger3D(const ColliderComponent& p1, const ColliderComponent& p2); // AABBとAABB

	// 検知と押し出し
	bool CheckHit_CubeAndCube_NoTrigger2D(const ColliderComponent& p1, const ColliderComponent& p2, DirectX::XMFLOAT3& pos); // AABBとAABB

	bool CheckHit_CubeAndCube_NoTrigger2D_Normal(const ColliderComponent& p1, const ColliderComponent& p2, DirectX::XMFLOAT3& hitNormal); // AABBとAABB

	bool CheckHit_CubeAndCube_IsTrigger2D_Normal(const ColliderComponent& p1, const ColliderComponent& p2, DirectX::XMFLOAT3& hitNormal); // AABBとAABB

	bool CheckHit_CubeAndCube_NoTrigger3D(const ColliderComponent& p1, const ColliderComponent& p2, DirectX::XMFLOAT3& pos); // AABBとAABB

	// レイとAABBの当たり判定
	bool IntersectRayAABB(const DirectX::XMVECTOR& rayOrigin, const DirectX::XMVECTOR& rayDir,
		const AABB& hit, float& tMinOut);

	bool TestNormal(const ColliderComponent& p1, const ColliderComponent& p2, DirectX::XMFLOAT3& hitNormal);


	// 円どうしの当たり判定
	// 触れているかどうかだけを検知
	bool CheckHit_SphereAndSphere_IsTrigger3D(const Sphere& p1, const Sphere& p2);

	// 検知と押し出し
	bool CheckHit_SphereAndSphere_NoTrigger2D(const Sphere& p1, const Sphere& p2, DirectX::XMFLOAT3& pos);
	bool CheckHit_SphereAndSphere_NoTrigger3D(const Sphere& p1, const Sphere& p2, DirectX::XMFLOAT3& pos);

	// 円と直方体の当たり判定
	// 検知のみ
	bool CheckHit_SphereAndCube_IsTrigger3D(const Sphere& p1, const AABB& p2);

	// 検知と押し出し
	bool CheckHit_SphereAndCube_NoTrigger2D(const Sphere& p1, const AABB& p2, DirectX::XMFLOAT3& pos);
	bool CheckHit_SphereAndCube_NoTrigger3D(const Sphere& p1, const AABB& p2, DirectX::XMFLOAT3& pos);

	// AABB と OBB の当たり判定
	bool CheckHit_AABBAndOBB_IsTrigger3D(const ColliderComponent& aabb, const ColliderComponent& obb);

	// 立方体どうしの当たり判定（調べるだけ）
	bool CubeAndCubeCheck_OBB(const OBB& col1, const OBB& col2);

	// 立方体どうしの当たり判定（めり込みを直す）
	bool CubeAndCubeHit_OBB(const OBB& col1, const OBB& col2);

	bool CompareLengthOBBvsAABB(
		const OBB& obb,
		const AABB& aabb,
		const DirectX::XMFLOAT3& axis,
		const DirectX::XMFLOAT3& vecDistance);

	// どの１面が当たっているのか判定
	//AABB& DetectCollisionFace(const AABB& a, const AABB& b);

	//内積・外積
	float Dot(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);
	DirectX::XMFLOAT3 Cross(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);


	DirectX::XMFLOAT3 ClosestPointOnSegment(const DirectX::XMFLOAT3& point, const Segment& segment);
	float DistanceSquaredPointToSegment(const DirectX::XMFLOAT3& point, const Segment& segment);
	float DistanceSquaredPointToSegment(const DirectX::XMFLOAT3& point, const Segment& segment, DirectX::XMFLOAT3& contact);
	float DistancePointToSegment(const DirectX::XMFLOAT3& point, const Segment& segment);
	float DistancePointToSegment(const DirectX::XMFLOAT3& point, const Segment& segment, DirectX::XMFLOAT3& contact);
	float DistancePointToPlane(const DirectX::XMFLOAT3& point, const PlaneNormal& plane);
	DirectX::XMFLOAT3 ProjectPointToPlane(const DirectX::XMFLOAT3& point, const PlaneNormal& plane);
	bool PointInTriangle(const DirectX::XMFLOAT3& point, const TrianglePolygon& polygon);
	DirectX::XMFLOAT3 ClosestPointOnTriangle(const DirectX::XMFLOAT3& point, const TrianglePolygon& polygon);
	DirectX::XMFLOAT3 GetNormal(const TrianglePolygon& polygon);


	DirectX::XMFLOAT3 moveSphere(const Segment& capsule, const float& radius, const TrianglePolygon& polygon, const DirectX::XMFLOAT3& contact, float& distance);
	DirectX::XMFLOAT3 moveSphere(const Sphere& sphere, const TrianglePolygon& polygon, const DirectX::XMFLOAT3& contact);

	// セッター
	void SetColliderSize_OBB(const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& size, const DirectX::XMVECTOR& angle) {

		DirectX::XMFLOAT3 offsetSize = { size.x + m_Coll_OBB.offsetSize.x,
									 size.y + m_Coll_OBB.offsetSize.y,
									 size.z + m_Coll_OBB.offsetSize.z };

		// 位置
		m_Coll_OBB.center.x = pos.x;
		m_Coll_OBB.center.y = pos.y;
		m_Coll_OBB.center.z = pos.z;

		// サイズ
		m_Coll_OBB.size.x = offsetSize.x;
		m_Coll_OBB.size.y = offsetSize.y;
		m_Coll_OBB.size.z = offsetSize.z;

		// アングル
		m_Coll_OBB.axisQuat = angle;
		m_Coll_OBB.axisQuat = DirectX::XMQuaternionMultiply(m_Coll_OBB.axisQuat, m_Coll_OBB.offsetQuat);

		m_Coll_OBB.rotation = m_Coll_OBB.axisQuat;

		DirectX::XMMATRIX mtx = DirectX::XMMatrixRotationQuaternion(m_Coll_OBB.axisQuat);

		DirectX::XMStoreFloat3(&m_Coll_OBB.axisX, mtx.r[0]); // X軸
		DirectX::XMStoreFloat3(&m_Coll_OBB.axisY, mtx.r[1]); // Y軸
		DirectX::XMStoreFloat3(&m_Coll_OBB.axisZ, mtx.r[2]); // Z軸

		// 4. ワールド中心座標の決定 (オフセットの位置を回転後に適用)
		DirectX::XMFLOAT3 offsetPos = DirectX::XMFLOAT3(m_Coll_OBB.offsetCenter.x, m_Coll_OBB.offsetCenter.y, m_Coll_OBB.offsetCenter.z);
		DirectX::XMVECTOR vOffsetCenter = XMLoadFloat3(&offsetPos);

		DirectX::XMVECTOR vPos = XMLoadFloat3(&m_Coll_OBB.center); // posをロード

		DirectX::XMVECTOR vWorldOffset = DirectX::XMVector3TransformNormal(vOffsetCenter, mtx);
		DirectX::XMVECTOR vFinalCenter = DirectX::XMVectorAdd(vPos, vWorldOffset);
		DirectX::XMStoreFloat3(&m_Coll_OBB.center, vFinalCenter); // 最終的なワールド中心座標を coll_ob.center に上書き
	};

	void SetColliderSize_AABB(const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& size) {

		DirectX::XMFLOAT3 offsetCenter = { pos.x + m_Coll_AABB.offsetCenter.x,
									 pos.y + m_Coll_AABB.offsetCenter.y,
									 pos.z + m_Coll_AABB.offsetCenter.z };

		DirectX::XMFLOAT3 offsetSize = { size.x + m_Coll_AABB.offsetSize.x,
									 size.y + m_Coll_AABB.offsetSize.y,
									 size.z + m_Coll_AABB.offsetSize.z };

		// 最小値
		m_Coll_AABB.min.x = offsetCenter.x - offsetSize.x;
		m_Coll_AABB.min.y = offsetCenter.y - offsetSize.y;
		m_Coll_AABB.min.z = offsetCenter.z - offsetSize.z;

		// 最大値
		m_Coll_AABB.max.x = offsetCenter.x + offsetSize.x;
		m_Coll_AABB.max.y = offsetCenter.y + offsetSize.y;
		m_Coll_AABB.max.z = offsetCenter.z + offsetSize.z;
	};

	void SetColliderSize_AABB(const TransformComponent& trans) {

		DirectX::XMFLOAT3 pos = trans.GetPosition();
		DirectX::XMFLOAT3 scale = trans.GetScale();

		SetColliderSize_AABB(pos, scale);
	};

	void SetColliderSize_Sphere(const DirectX::XMFLOAT3& pos, const float radius) {
		this->m_Coll_Sphere.center = pos;
		this->m_Coll_Sphere.radius = radius;
	};

	void SetOffsetCenterAABB(const DirectX::XMFLOAT3& offset) { m_Coll_AABB.offsetCenter = offset; };
	void SetOffsetSizeAABB(const DirectX::XMFLOAT3& offset) { m_Coll_AABB.offsetSize = offset; };

	void SetOffsetCenterOBB(const DirectX::XMFLOAT3& offset) { m_Coll_OBB.offsetCenter = offset; };
	void SetOffsetSizeOBB(const DirectX::XMFLOAT3& offset) { m_Coll_OBB.offsetSize = offset; };
	void SetOffsetRotationOBB(const DirectX::XMFLOAT3& offset) {

		float pitch = DirectX::XMConvertToRadians(offset.x);	// X軸回転
		float yaw = DirectX::XMConvertToRadians(offset.y);     // Y軸回転
		float roll = DirectX::XMConvertToRadians(offset.z);    // Z軸回転

		DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYaw(
			pitch,
			yaw,
			roll
		);

		m_Coll_OBB.offsetQuat = quat;
	};

	void SetActiveColliderFlag(const bool flag) { this->m_ActiveColliderFlag = flag; };

	//	 void SetOffsetRotation(const DirectX::XMFLOAT3& offset) { this->offsetRotation = offset; };

	// ゲッター
	OBB& GetColliderSize_OBB() { return m_Coll_OBB; };
	AABB& GetColliderSize_AABB() { return m_Coll_AABB; };
	Sphere& GetColliderSize_Sphere() { return m_Coll_Sphere; };

	DirectX::XMFLOAT3 GetOffsetCenterAABB() const { return m_Coll_AABB.offsetCenter; };
	DirectX::XMFLOAT3 GetOffsetSizeAABB() const { return m_Coll_AABB.offsetSize; };
	//	 DirectX::XMFLOAT3 GetOffsetRotation() const { return offsetRotation; };	

	DirectX::XMMATRIX GetWorldAABBMatrix() const { return m_Coll_AABB.worldAABBMatrix; };
	DirectX::XMMATRIX GetWorldOBBMatrix() const { return m_Coll_OBB.worldOBBMatrix; };

	bool GetActiveColliderFlag() const { return this->m_ActiveColliderFlag; };

	void MakeWorldAABBMatrix();
	void MakeWorldOBBMatrix();

	//struct Plane {
	//	DirectX::XMFLOAT3 normal; // 平面の法線ベクトル
	//	float d;         // 平面の方程式: ax + by + cz + d = 0 の d
	//};

	// make AABB
	AABB SetAABB(const DirectX::XMFLOAT3& centerposition, float width, float height, float depth);

	void InitCollider();

	/*
		// 線分に平行な与えられた幅の４角形の座標を求める
		void CalcQuadOrientedLine(
			DirectX::XMFLOAT3 startpos,
			DirectX::XMFLOAT3 endpos,
			DirectX::XMFLOAT3* v,
			float width);


		// 幅をもった線分（四角形）と球の当たり判定を行う
		bool CollisionSphereOrientedQuad(
			DirectX::XMFLOAT3 StartPos,						// 開始位置
			DirectX::XMFLOAT3 EndPos,							// 終了位置
			float width,							// 幅
			float radius,							// 半径
			DirectX::XMFLOAT3 CirclePos);						// 中心座標
		*/
};