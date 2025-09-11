#pragma once

#include <simplemath.h>
#include <directxcollision.h>	// Collision の変更を一部する、XMFLOAT4で扱うのが本当は良いらしい。クオータニオンの始まりか？
#include <unordered_map>
#include "Component.h"

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
	DirectX::SimpleMath::Vector3 min = {};
	DirectX::SimpleMath::Vector3 max = {};
	DirectX::SimpleMath::Vector3 offsetCenter = {};
	DirectX::SimpleMath::Vector3 offsetSize = {};

	DirectX::SimpleMath::Vector3 beforeMin = {};
	DirectX::SimpleMath::Vector3 beforeMax = {};
	DirectX::SimpleMath::Vector3 beforeOffsetCenter = {};
	DirectX::SimpleMath::Vector3 beforeOffsetSize = {};

	// ワールド行列
	DirectX::SimpleMath::Matrix worldAABBMatrix = {};

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
	DirectX::SimpleMath::Vector3 center = {};
	DirectX::SimpleMath::Vector3 size = {};
	DirectX::SimpleMath::Vector3 rotation = {};
	DirectX::SimpleMath::Vector3 offsetCenter = {};
	DirectX::SimpleMath::Vector3 offsetSize = {};
	DirectX::SimpleMath::Vector3 offsetRotation = {};
	DirectX::SimpleMath::Vector3 axisX = {};
	DirectX::SimpleMath::Vector3 axisY = {};
	DirectX::SimpleMath::Vector3 axisZ = {};

	DirectX::SimpleMath::Matrix worldOBBMatrix = {}; // ワールド行列
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
		const DirectX::SimpleMath::Vector3& size, const DirectX::SimpleMath::Vector3& angle) {
		
		DirectX::SimpleMath::Vector3 offsetCenter = { pos.x + coll_ob.offsetCenter.x,
							 pos.y + coll_ob.offsetCenter.y,
							 pos.z + coll_ob.offsetCenter.z };

		DirectX::SimpleMath::Vector3 offsetSize = { size.x + coll_ob.offsetSize.x,
									 size.y + coll_ob.offsetSize.y,
									 size.z + coll_ob.offsetSize.z };

		DirectX::SimpleMath::Vector3 offsetRotation = { angle.x + coll_ob.offsetRotation.x,
									 angle.y + coll_ob.offsetRotation.y,
									 angle.z + coll_ob.offsetRotation.z };
		
		// 位置
		coll_ob.center.x = offsetCenter.x;
		coll_ob.center.y = offsetCenter.y;
		coll_ob.center.z = offsetCenter.z;

		// サイズ
		coll_ob.size.x = offsetSize.x;
		coll_ob.size.y = offsetSize.y;
		coll_ob.size.z = offsetSize.z;

		// アングル
		coll_ob.rotation.x = offsetRotation.x;
		coll_ob.rotation.y = offsetRotation.y;
		coll_ob.rotation.z = offsetRotation.z;

		float PitchRadians = DirectX::XMConvertToRadians(coll_ob.rotation.x); // X軸回転
		float YawRadians = DirectX::XMConvertToRadians(coll_ob.rotation.y);     // Y軸回転
		float RollRadians = DirectX::XMConvertToRadians(coll_ob.rotation.z);   // Z軸回転

		// 軸
		DirectX::SimpleMath::Matrix mtx = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);
		coll_ob.axisX = DirectX::SimpleMath::Vector3(mtx._11, mtx._12, mtx._13);
		coll_ob.axisY = DirectX::SimpleMath::Vector3(mtx._21, mtx._22, mtx._23);
		coll_ob.axisZ = DirectX::SimpleMath::Vector3(mtx._31, mtx._32, mtx._33);

		DirectX::SimpleMath::Vector3 worldOffset = DirectX::SimpleMath::Vector3::TransformNormal(coll_ob.offsetCenter, mtx);
		coll_ob.center = pos + worldOffset;
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

	inline void SetColliderSize_Sphere(const DirectX::XMFLOAT3& pos, const float radius) {
		this->coll_sp.center = pos;
		this->coll_sp.radius = radius;
	};

	inline void SetOffsetCenterAABB(const DirectX::SimpleMath::Vector3& offset) { coll_ab.offsetCenter = offset; };
	inline void SetOffsetSizeAABB(const DirectX::SimpleMath::Vector3& offset) { coll_ab.offsetSize = offset; };
	
	inline void SetOffsetCenterOBB(const DirectX::SimpleMath::Vector3& offset) { coll_ob.offsetCenter = offset; };
	inline void SetOffsetSizeOBB(const DirectX::SimpleMath::Vector3& offset) { coll_ob.offsetSize = offset; };
	inline void SetOffsetRotationOBB(const DirectX::SimpleMath::Vector3& offset) { coll_ob.offsetRotation = offset; };

	inline void SetActiveColliderFlag(bool flag) { this->m_activeColliderFlag = flag; };

	//	inline void SetOffsetRotation(const DirectX::SimpleMath::Vector3& offset) { this->offsetRotation = offset; };

	// ゲッター
	inline OBB& GetColliderSize_OBB() { return this->coll_ob; };
	inline AABB& GetColliderSize_AABB() { return this->coll_ab; };
	inline Sphere& GetColliderSize_Sphere() { return this->coll_sp; };

	inline DirectX::SimpleMath::Vector3 GetOffsetCenterAABB() const { return coll_ab.offsetCenter; };
	inline DirectX::SimpleMath::Vector3 GetOffsetSizeAABB() const { return coll_ab.offsetSize; };
//	inline DirectX::SimpleMath::Vector3 GetOffsetRotation() const { return offsetRotation; };	

	inline DirectX::SimpleMath::Matrix GetWorldAABBMatrix() const { return coll_ab.worldAABBMatrix; };
	inline DirectX::SimpleMath::Matrix GetWorldOBBMatrix() const { return coll_ob.worldOBBMatrix; };

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