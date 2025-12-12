#pragma once

#include <simplemath.h>
#include <directxcollision.h>	// Collision の変更を一部する、XMFLOAT4で扱うのが本当は良いらしい。クオータニオンの始まりか？
#include <unordered_map>
#include "Component.h"
#include "Transform.h"

// ライン（無限の長さの線）
struct Line {
	DirectX::SimpleMath::Vector3 point; // 通過地点
	DirectX::SimpleMath::Vector3 vec; // 線の方向ベクトル
};

// プレーン(無限の広さの平面)
struct PlaneNormal {
	DirectX::SimpleMath::Vector3 point; // 平面上の1点
	DirectX::SimpleMath::Vector3 normal; // 平面の法線ベクトル
	//float d;         // 平面の方程式: ax + by + cz + d = 0 の d
};

// セグメント（有限の長さの線分）
struct Segment {
	DirectX::SimpleMath::Vector3 start; // 始点
	DirectX::SimpleMath::Vector3 end; // 終点
};

// 三角形ポリゴン（有限の広さの平面）
struct TrianglePolygon {
	const DirectX::SimpleMath::Vector3 p0; //頂点0
	const DirectX::SimpleMath::Vector3 p1; //頂点1
	const DirectX::SimpleMath::Vector3 p2; //頂点2
};

// 球体
struct Sphere {
	DirectX::SimpleMath::Vector3 center = {}; // 中心
	float radius = 0.0f; // 半径
};

//// 円柱
//struct Cylinder {
//	DirectX::SimpleMath::Vector3 top; // 上
//	DirectX::SimpleMath::Vector3 bottom; // 底
//	float radius; // 半径
//};

//// カプセル
//struct Capsule {
//	DirectX::SimpleMath::Vector3 top; // 上の中心 
//	DirectX::SimpleMath::Vector3 bottom; // 底の中心
//	float radius; // 半径
//};

struct PushNormalize {
	GameObject* dirObj = nullptr;

	DirectX::SimpleMath::Vector3 pushDir = DirectX::SimpleMath::Vector3::Zero;
};

// BOX定義
struct AABB {
	DirectX::SimpleMath::Vector3 min = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 max = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 offsetCenter = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 offsetSize = DirectX::SimpleMath::Vector3::Zero;

	DirectX::SimpleMath::Vector3 beforeMin = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 beforeMax = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 beforeOffsetCenter = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 beforeOffsetSize = DirectX::SimpleMath::Vector3::Zero;

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
	DirectX::SimpleMath::Vector3 center = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 size = DirectX::SimpleMath::Vector3::Zero;
	DirectX::XMVECTOR rotation = { 0.0f,0.0f,0.0f,1.0f };
	DirectX::SimpleMath::Vector3 offsetCenter = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 offsetSize = DirectX::SimpleMath::Vector3::Zero;
	DirectX::XMVECTOR offsetQuat = {0.0f,0.0f,0.0f,1.0f};
	DirectX::SimpleMath::Vector3 axisX = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 axisY = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 axisZ = DirectX::SimpleMath::Vector3::Zero;

	DirectX::SimpleMath::Vector3 drawCenter = DirectX::SimpleMath::Vector3::Zero;

	DirectX::XMVECTOR axisQuat = {};
	DirectX::XMMATRIX worldOBBMatrix = DirectX::XMMatrixIdentity();
};

class ColliderComponent : public Component{
protected:

private:
	AABB beforeColl_ab; // 前回のAABBの当たり判定用
	AABB coll_ab; // AABBの当たり判定用
	OBB coll_ob; // OBBの当たり判定用
	Sphere coll_sp; // 球体の当たり判定用

	GameObject* beforeTouchObj = nullptr;	// 前回のタッチオブジェクト
	
	bool m_activeColliderFlag = true; // コライダーの有効・無効フラグ

	DirectX::SimpleMath::Vector2 beforeTouchAxis = {}; // 前回のタッチ軸

	std::unordered_map<GameObject*, DirectX::SimpleMath::Vector3> 
		touchObjects; // タッチしているオブジェクトのリスト

//	DirectX::SimpleMath::Vector3 offsetRotation = {};

public:
	ColliderComponent() = default;
	ColliderComponent(GameObject& obj);
	~ColliderComponent() = default;

	void Update() override;

	//当たり判定
	bool CheckHit(const Line& line, const PlaneNormal& plane); //線(無限の長さ)と平面(無限の大きさ)
	bool CheckHit(const Segment& segment, const PlaneNormal& plane); //線分と平面(無限の大きさ)
	bool CheckHit(const Line& line, const TrianglePolygon& polygon); //線(無限の長さ)とポリゴン
	bool CheckHit(const Line& line, const TrianglePolygon& polygon, DirectX::SimpleMath::Vector3& contact); //同上
	bool CheckHit(const Segment& segment, const TrianglePolygon& polygon); //線分とポリゴン
	bool CheckHit(const Segment& segment, const TrianglePolygon& polygon, DirectX::SimpleMath::Vector3& contact); //同上
	bool CheckHit(const Sphere& sphere, const PlaneNormal& plane); //球体と平面(無限の大きさ)
	bool CheckHit(const Sphere& sphere, const TrianglePolygon& polygon); //球体とポリゴン
	bool CheckHit(const Sphere& sphere, const TrianglePolygon& polygon, DirectX::SimpleMath::Vector3& contact); //同上
	//bool CheckHit(const Cylinder& cylinder, const Plane& plane); //円柱と平面
	//bool CheckHit(const Cylinder& cylinder, const Polygon& polygon); //円柱とポリゴン
	//bool CheckHit(const Cylinder& cylinder, const Polygon& polygon, DirectX::SimpleMath::Vector3& contact); //同上
	//bool CheckHit(const Capsule& capsule, const Plane& plane); //カプセルと平面(無限の大きさ)
	//bool CheckHit(const Capsule& capsule, const Polygon& polygon); //カプセルとポリゴン
	//bool CheckHit(const Capsule& capsule, const Polygon& polygon, DirectX::SimpleMath::Vector3& contact); //同上
	bool CheckHit(Sphere sphere1, Sphere sphere2); //球体と球体
	bool CheckHit(Sphere sphere1, Sphere sphere2, DirectX::SimpleMath::Vector3& contact); //同上

	// 直方体どうしの当たり判定
	// 触れているかどうかだけを検知
	bool CheckHit_CubeAndCube_IsTrigger3D(const ColliderComponent& p1, const ColliderComponent& p2); // AABBとAABB

	// 検知と押し出し
	bool CheckHit_CubeAndCube_NoTrigger2D(const ColliderComponent& p1, const ColliderComponent& p2, DirectX::XMFLOAT3& pos); // AABBとAABB
	
	bool CheckHit_CubeAndCube_NoTrigger2D_Normal(const ColliderComponent& p1, const ColliderComponent& p2, DirectX::SimpleMath::Vector3& hitNormal); // AABBとAABB

	bool CheckHit_CubeAndCube_IsTrigger2D_Normal(const ColliderComponent& p1, const ColliderComponent& p2, DirectX::SimpleMath::Vector3& hitNormal); // AABBとAABB
	
	bool CheckHit_CubeAndCube_NoTrigger3D(const ColliderComponent& p1, const ColliderComponent& p2, DirectX::XMFLOAT3& pos); // AABBとAABB

	// レイとAABBの当たり判定
	bool IntersectRayAABB(const DirectX::XMVECTOR& rayOrigin, const DirectX::XMVECTOR& rayDir,
		const AABB& hit, float& tMinOut);

	bool TestNormal(const ColliderComponent& p1, const ColliderComponent& p2, DirectX::SimpleMath::Vector3& hitNormal);


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
	bool CheckHit_AABBAndOBB_IsTrigger3D(const ColliderComponent& p1, const ColliderComponent& p2);

	// 立方体どうしの当たり判定（調べるだけ）
	bool CubeAndCubeCheck_OBB(const OBB& col1, const OBB& col2);

	// 立方体どうしの当たり判定（めり込みを直す）
	bool CubeAndCubeHit_OBB(const OBB& col1, const OBB& col2);

	bool CompareLengthOBBvsAABB(
		const OBB& obb,
		const AABB& aabb,
		const DirectX::SimpleMath::Vector3& axis,
		const DirectX::SimpleMath::Vector3& vecDistance);

	// どの１面が当たっているのか判定
	//AABB& DetectCollisionFace(const AABB& a, const AABB& b);

	//内積・外積
	float Dot(const DirectX::SimpleMath::Vector3& v1, const DirectX::SimpleMath::Vector3& v2);
	DirectX::SimpleMath::Vector3 Cross(const DirectX::SimpleMath::Vector3& v1, const DirectX::SimpleMath::Vector3& v2);


	DirectX::SimpleMath::Vector3 ClosestPointOnSegment(const DirectX::SimpleMath::Vector3& point, const Segment& segment);
	float DistanceSquaredPointToSegment(const DirectX::SimpleMath::Vector3& point, const Segment& segment);
	float DistanceSquaredPointToSegment(const DirectX::SimpleMath::Vector3& point, const Segment& segment, DirectX::SimpleMath::Vector3& contact);
	float DistancePointToSegment(const DirectX::SimpleMath::Vector3& point, const Segment& segment);
	float DistancePointToSegment(const DirectX::SimpleMath::Vector3& point, const Segment& segment, DirectX::SimpleMath::Vector3& contact);
	float DistancePointToPlane(const DirectX::SimpleMath::Vector3& point, const PlaneNormal& plane);
	DirectX::SimpleMath::Vector3 ProjectPointToPlane(const DirectX::SimpleMath::Vector3& point, const PlaneNormal& plane);
	bool PointInTriangle(const DirectX::SimpleMath::Vector3& point, const TrianglePolygon& polygon);
	DirectX::SimpleMath::Vector3 ClosestPointOnTriangle(const DirectX::SimpleMath::Vector3& point, const TrianglePolygon& polygon);
	DirectX::SimpleMath::Vector3 GetNormal(const TrianglePolygon& polygon);


	DirectX::SimpleMath::Vector3 moveSphere(const Segment& capsule, const float& radius, const TrianglePolygon& polygon, const DirectX::SimpleMath::Vector3& contact, float& distance);
	DirectX::SimpleMath::Vector3 moveSphere(const Sphere& sphere, const TrianglePolygon& polygon, const DirectX::SimpleMath::Vector3& contact);

	// セッター
	inline void SetColliderSize_OBB(const DirectX::SimpleMath::Vector3& pos,
		const DirectX::SimpleMath::Vector3& size, const DirectX::XMVECTOR& angle) {

		DirectX::SimpleMath::Vector3 offsetSize = { size.x + coll_ob.offsetSize.x,
									 size.y + coll_ob.offsetSize.y,
									 size.z + coll_ob.offsetSize.z };

		// 位置
		coll_ob.center.x = pos.x;
		coll_ob.center.y = pos.y;
		coll_ob.center.z = pos.z;

		// サイズ
		coll_ob.size.x = offsetSize.x;
		coll_ob.size.y = offsetSize.y;
		coll_ob.size.z = offsetSize.z;

		// アングル
		coll_ob.axisQuat = angle;
		coll_ob.axisQuat = DirectX::XMQuaternionMultiply(coll_ob.axisQuat, coll_ob.offsetQuat);

		coll_ob.rotation = coll_ob.axisQuat;

		DirectX::XMMATRIX mtx = DirectX::XMMatrixRotationQuaternion(coll_ob.axisQuat);

		DirectX::XMStoreFloat3(&coll_ob.axisX, mtx.r[0]); // X軸
		DirectX::XMStoreFloat3(&coll_ob.axisY, mtx.r[1]); // Y軸
		DirectX::XMStoreFloat3(&coll_ob.axisZ, mtx.r[2]); // Z軸

		// 4. ワールド中心座標の決定 (オフセットの位置を回転後に適用)
		DirectX::XMFLOAT3 offsetPos = DirectX::XMFLOAT3(coll_ob.offsetCenter.x, coll_ob.offsetCenter.y, coll_ob.offsetCenter.z);
		DirectX::XMVECTOR vOffsetCenter = XMLoadFloat3(&offsetPos);

		DirectX::XMVECTOR vPos = XMLoadFloat3(&coll_ob.center); // posをロード

		DirectX::XMVECTOR vWorldOffset = DirectX::XMVector3TransformNormal(vOffsetCenter, mtx);
		DirectX::XMVECTOR vFinalCenter = DirectX::XMVectorAdd(vPos, vWorldOffset);
		DirectX::XMStoreFloat3(&coll_ob.center, vFinalCenter); // 最終的なワールド中心座標を coll_ob.center に上書き
	};

	inline void SetColliderSize_AABB(const DirectX::SimpleMath::Vector3& pos,
		const DirectX::SimpleMath::Vector3& size) {

		DirectX::SimpleMath::Vector3 offsetCenter = { pos.x + coll_ab.offsetCenter.x,
									 pos.y + coll_ab.offsetCenter.y,
									 pos.z + coll_ab.offsetCenter.z };

		DirectX::SimpleMath::Vector3 offsetSize =   { size.x + coll_ab.offsetSize.x,
									 size.y + coll_ab.offsetSize.y,
									 size.z + coll_ab.offsetSize.z };

		// 最小値
		coll_ab.min.x = offsetCenter.x - offsetSize.x;
		coll_ab.min.y = offsetCenter.y - offsetSize.y;
		coll_ab.min.z = offsetCenter.z - offsetSize.z;

		// 最大値
		coll_ab.max.x = offsetCenter.x + offsetSize.x;
		coll_ab.max.y = offsetCenter.y + offsetSize.y;
		coll_ab.max.z = offsetCenter.z + offsetSize.z;
	};

	inline void SetColliderSize_AABB(const TransformComponent& trans) {

		DirectX::SimpleMath::Vector3 pos = trans.GetPosition();
		DirectX::SimpleMath::Vector3 scale = trans.GetScale();

		SetColliderSize_AABB(pos, scale);
	};

	inline void SetColliderSize_Sphere(const DirectX::XMFLOAT3& pos, const float radius) {
		this->coll_sp.center = pos;
		this->coll_sp.radius = radius;
	};

	inline void SetOffsetCenterAABB(const DirectX::SimpleMath::Vector3& offset) { coll_ab.offsetCenter = offset; };
	inline void SetOffsetSizeAABB(const DirectX::SimpleMath::Vector3& offset) { coll_ab.offsetSize = offset; };
	
	inline void SetOffsetCenterOBB(const DirectX::SimpleMath::Vector3& offset) { coll_ob.offsetCenter = offset; };
	inline void SetOffsetSizeOBB(const DirectX::SimpleMath::Vector3& offset) { coll_ob.offsetSize = offset; };
	inline void SetOffsetRotationOBB(const DirectX::XMFLOAT3& offset) { 
		
		float pitch = DirectX::XMConvertToRadians(offset.x);	// X軸回転
		float yaw = DirectX::XMConvertToRadians(offset.y);     // Y軸回転
		float roll = DirectX::XMConvertToRadians(offset.z);    // Z軸回転

		DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYaw(
			pitch,
			yaw,
			roll
		);

		coll_ob.offsetQuat = quat; };

	inline void SetActiveColliderFlag(const bool flag) { this->m_activeColliderFlag = flag; };

	//	inline void SetOffsetRotation(const DirectX::SimpleMath::Vector3& offset) { this->offsetRotation = offset; };

	// ゲッター
	inline OBB& GetColliderSize_OBB() { return coll_ob; };
	inline AABB& GetColliderSize_AABB() { return coll_ab; };
	inline Sphere& GetColliderSize_Sphere() { return coll_sp; };

	inline DirectX::SimpleMath::Vector3 GetOffsetCenterAABB() const { return coll_ab.offsetCenter; };
	inline DirectX::SimpleMath::Vector3 GetOffsetSizeAABB() const { return coll_ab.offsetSize; };
//	inline DirectX::SimpleMath::Vector3 GetOffsetRotation() const { return offsetRotation; };	

	inline DirectX::XMMATRIX GetWorldAABBMatrix() const { return coll_ab.worldAABBMatrix; };
	inline DirectX::XMMATRIX GetWorldOBBMatrix() const { return coll_ob.worldOBBMatrix; };

	inline bool GetActiveColliderFlag() const { return this->m_activeColliderFlag; };

	void MakeWorldAABBMatrix();
	void MakeWorldOBBMatrix();

	//struct Plane {
	//	DirectX::SimpleMath::Vector3 normal; // 平面の法線ベクトル
	//	float d;         // 平面の方程式: ax + by + cz + d = 0 の d
	//};

	// make AABB
	AABB SetAABB(DirectX::SimpleMath::Vector3 centerposition, float width, float height, float depth);

	/*
		// 線分に平行な与えられた幅の４角形の座標を求める
		void CalcQuadOrientedLine(
			DirectX::SimpleMath::Vector3 startpos,
			DirectX::SimpleMath::Vector3 endpos,
			DirectX::SimpleMath::Vector3* v,
			float width);


		// 幅をもった線分（四角形）と球の当たり判定を行う
		bool CollisionSphereOrientedQuad(
			DirectX::SimpleMath::Vector3 StartPos,						// 開始位置
			DirectX::SimpleMath::Vector3 EndPos,							// 終了位置
			float width,							// 幅
			float radius,							// 半径
			DirectX::SimpleMath::Vector3 CirclePos);						// 中心座標
		*/
};