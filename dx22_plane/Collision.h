#pragma once

#include <simplemath.h>
#include <directxcollision.h>	// Collision の変更を一部する、XMFLOAT4で扱うのが本当は良いらしい。クオータニオンの始まりか？

namespace Collision
{
	// ライン（無限の長さの線）
	struct Line {
		DirectX::SimpleMath::Vector3 point; // 通過地点
		DirectX::SimpleMath::Vector3 vec; // 線の方向ベクトル
	};

	// プレーン(無限の広さの平面)
	struct Plane {
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
	struct Polygon {
		const DirectX::SimpleMath::Vector3 p0; //頂点0
		const DirectX::SimpleMath::Vector3 p1; //頂点1
		const DirectX::SimpleMath::Vector3 p2; //頂点2
	};

	// 球体
	struct Sphere {
		DirectX::SimpleMath::Vector3 center; // 中心
		float radius; // 半径
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

	// BOX定義
	struct AABB {
		DirectX::SimpleMath::Vector3 min = {};
		DirectX::SimpleMath::Vector3 max = {};

		/*bool none = false;
		bool left = false;
		bool right = false;
		bool top = false;
		bool bottom = false;
		bool front = false;
		bool back = false;*/
	};

	// 当たり判定でのみ使う
	struct Vector9 {
		float x = 0.0f;		
		float y = 0.0f; 
		float z = 0.0f;     // 中心座標
		float w = 0.0f; 
		float h = 0.0f; 
		float u = 0.0f;     // 幅、高さ、奥行き
		float pitch = 0.0f;       // X軸回転角度（ピッチ、度数法）
		float yaw = 0.0f;         // Y軸回転角度（ヨー、度数法）
		float roll = 0.0f;        // Z軸回転角度（ロール、度数法）
	};


	//当たり判定
	bool CheckHit(const Line& line, const Plane& plane); //線(無限の長さ)と平面(無限の大きさ)
	bool CheckHit(const Segment& segment, const Plane& plane); //線分と平面(無限の大きさ)
	bool CheckHit(const Line& line, const Polygon& polygon); //線(無限の長さ)とポリゴン
	bool CheckHit(const Line& line, const Polygon& polygon, DirectX::SimpleMath::Vector3& contact); //同上
	bool CheckHit(const Segment& segment, const Polygon& polygon); //線分とポリゴン
	bool CheckHit(const Segment& segment, const Polygon& polygon, DirectX::SimpleMath::Vector3& contact); //同上
	bool CheckHit(const Sphere& sphere, const Plane& plane); //球体と平面(無限の大きさ)
	bool CheckHit(const Sphere& sphere, const Polygon& polygon); //球体とポリゴン
	bool CheckHit(const Sphere& sphere, const Polygon& polygon, DirectX::SimpleMath::Vector3& contact); //同上
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
	bool CheckHit_CubeAndCube_IsTrigger3D(AABB p1, AABB p2); // AABBとAABB

	// 検知と押し出し
	bool CheckHit_CubeAndCube_NoTrigger2D(const AABB& p1, const AABB& p2,DirectX::XMFLOAT3& pos); // AABBとAABB
	bool CheckHit_CubeAndCube_NoTrigger2D_Normal(const AABB& p1, const AABB& p2,DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& hitNormal); // AABBとAABB
	bool CheckHit_CubeAndCube_NoTrigger3D(const AABB& p1, const AABB& p2,DirectX::XMFLOAT3& pos); // AABBとAABB

	// レイとAABBの当たり判定
	bool IntersectRayAABB(const DirectX::XMVECTOR& rayOrigin, const DirectX::XMVECTOR& rayDir, 
						  const AABB& hit, float& tMinOut);


	// 円どうしの当たり判定
	// 触れているかどうかだけを検知
	bool CheckHit_SphereAndSphere_IsTrigger3D(const Sphere& p1,const Sphere& p2);

	// 検知と押し出し
	bool CheckHit_SphereAndSphere_NoTrigger2D(const Sphere& p1,const Sphere& p2,DirectX::XMFLOAT3& pos);
	bool CheckHit_SphereAndSphere_NoTrigger3D(const Sphere& p1,const Sphere& p2,DirectX::XMFLOAT3& pos);

	// 円と直方体の当たり判定
	// 検知のみ
	bool CheckHit_SphereAndCube_IsTrigger3D(const Sphere& p1, const AABB& p2);

	// 検知と押し出し
	bool CheckHit_SphereAndCube_NoTrigger2D(const Sphere& p1, const AABB& p2,DirectX::XMFLOAT3& pos);
	bool CheckHit_SphereAndCube_NoTrigger3D(const Sphere& p1, const AABB& p2,DirectX::XMFLOAT3& pos);

	// 立方体どうしの当たり判定（調べるだけ）
	bool CubeAndCubeCheck_OBB(const Vector9& col1,const Vector9& col2);

	// 立方体どうしの当たり判定（めり込みを直す）
	bool CubeAndCubeHit_OBB(const Vector9& col1, const Vector9& col2);

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
	float DistancePointToPlane(const DirectX::SimpleMath::Vector3& point, const Plane& plane);
	DirectX::SimpleMath::Vector3 ProjectPointToPlane(const DirectX::SimpleMath::Vector3& point, const Plane& plane);
	bool PointInTriangle(const DirectX::SimpleMath::Vector3& point, const Polygon& polygon);
	DirectX::SimpleMath::Vector3 ClosestPointOnTriangle(const DirectX::SimpleMath::Vector3& point, const Polygon& polygon);
	DirectX::SimpleMath::Vector3 GetNormal(const Polygon& polygon);


	DirectX::SimpleMath::Vector3 moveSphere(const Segment& capsule, const float& radius, const Polygon& polygon, const DirectX::SimpleMath::Vector3& contact, float& distance);
	DirectX::SimpleMath::Vector3 moveSphere(const Sphere& sphere, const Polygon& polygon, const DirectX::SimpleMath::Vector3& contact);


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





}