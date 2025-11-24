// -------------------------------------------
// リジットボディ
// （Ｚ軸をコメントアウトしているのは、
// 何かの手違いで裏面に描画されるのを防ぐため）
// -------------------------------------------

#pragma once
#include <DirectXMath.h> // DirextXの数学関連のヘッダーファイル
#include <SimpleMath.h> // DirectX::SimpleMathを使うためのヘッダーファイル
#include "Component.h"
#include <cmath>
#include <chrono>	// 時間を計測してくれる

#define GRAVITY (9.80665f)	  // 重力
#define GRAVITY_STOP (100.0f) // 重力を止めるためのしきい値、これ以上の速度が出たら重力を止める
#define FRICTION (0.1f)		  // 摩擦係数
#define RESTITUTION (0.8f)	  // 反発係数（０～１ぐらいが一般的らしい）、この値が大きいほど反発も大きくなる
#define AIRRESISTANCE (0.05f) // 空気抵抗係数
#define MIN_VELOCITY_THRESHOLD (10.0f)	// 最小速度のしきい値
#define DAMPINGFACTOR (0.9f)	// 減衰率	
#define STOPVELOCITY (0.03f)	// この値を下回ったら値を０にする 

class RigidBodyComponent :public Component
{
private:
	DirectX::SimpleMath::Vector3 m_velocity{};	 // 速度
	DirectX::SimpleMath::Vector3 m_acceleration{};// 加速度
	DirectX::SimpleMath::Vector3 m_totalForce{};              // 合力
	DirectX::SimpleMath::Vector3 m_LimitVelocity{};         // 速度制限
	float m_mass = 1.0f;				 // 質量
	//	float elapsedTime = 0.0f;		 // 落下中の時間、これで自由落下の計算をする
	bool m_fallFlag = false;		 	 // 落下のフラグ 
	bool m_gravityFlag = false;			 // 重力を有効にするかどうかのフラグ
	bool m_timeFlag = false;			 // 落下タイミングのフラグ
	bool m_beforeGravityFlag = false;	 // 重力を有効にするかどうか
	const float m_deltaTime = 0.016f;		 // 前回の時間からの経過時間
	float m_firstFallMagnification = 120.0f; // 初回の落下倍率
	float m_fallMagnification = 12.0f;	 // 落下倍率

	std::chrono::high_resolution_clock::time_point startTime;	// 計測開始時間
	std::chrono::high_resolution_clock::time_point lastTime;	// 最後の時間

public:
	RigidBodyComponent() = default;
	RigidBodyComponent(GameObject& obj);

	~RigidBodyComponent() = default;

	void Update()override;

	void UpdateVelocity();

	// 速度返す
	inline void SetVelocity(const DirectX::SimpleMath::Vector3& velocity) { m_velocity = velocity; };
	inline void SetLimitVelocity(const DirectX::SimpleMath::Vector3& velocity) { m_LimitVelocity = velocity; };
	inline void SetLimitVelocity_X(const float velocity) { m_LimitVelocity.x = velocity; };
	inline void SetLimitVelocity_Y(const float velocity) { m_LimitVelocity.y = velocity; };
	inline void SetLimitVelocity_Z(const float velocity) { m_LimitVelocity.z = velocity; };
	inline DirectX::SimpleMath::Vector3 GetVelocity()const { return m_velocity; };
	inline void AddVelocity(const DirectX::SimpleMath::Vector3& velocity) { m_velocity += velocity; };

	void ConstantVelocity(const DirectX::SimpleMath::Vector3& velocity);	// 等速運動
	void ConstantVelocity_X(const float velocity);	// 等速運動
	void ConstantVelocity_Y(const float velocity);	// 等速運動
	void ConstantVelocity_Z(const float velocity);	// 等速運動

	void ReduceVelocity(const DirectX::SimpleMath::Vector3& velocity = DirectX::SimpleMath::Vector3{ DAMPINGFACTOR,DAMPINGFACTOR,DAMPINGFACTOR });		// 速度減らし
	void ReduceVelocity_X(const float velocity = DAMPINGFACTOR);		// 速度減らし
	void ReduceVelocity_Y(const float velocity = DAMPINGFACTOR);		// 速度減らし
	void ReduceVelocity_Z(const float velocity = DAMPINGFACTOR);		// 速度減らし

	inline void ClearVelocity() { m_velocity = DirectX::SimpleMath::Vector3::Zero; };

	inline void SetMass(const float mass) { m_mass = mass; };
	inline float GetMass()const { return m_mass; };

	inline void SetFallFlag(const bool flag) { m_fallFlag = flag; };
	inline bool GetFallFlag() const { return m_fallFlag; };

	inline void SetTimeFlag(const bool flag) { m_timeFlag = flag; };

	inline void SetGravityFlag(const bool flag) { m_gravityFlag = flag; }	// 重力を有効にするかどうかのフラグを設定
	inline bool GetGravityFlag() const { return m_gravityFlag; }	// 重力を有効にするかどうかのフラグを取得

	inline void SetFirstFallMagnification(const float magnification) { m_firstFallMagnification = magnification; }	// 初回の落下倍率を設定
	inline float GetFirstFallMagnification() const { return m_firstFallMagnification; }	// 初回の落下倍率を取得

	inline void SetFallMagnification(const float magnification) { m_fallMagnification = magnification; }	// 落下倍率を設定
	inline float GetFallMagnification() const { return m_fallMagnification; }	// 落下倍率を取得

	//	DirectX::XMFLOAT3& AcceleratorPosition(DirectX::XMFLOAT3& pos);	// 加速度から速度、速度から位置の更新
	float UseGravity(const bool gravityFlag);		// 重力
	inline void AddForce(const DirectX::SimpleMath::Vector3& force) { m_totalForce += force; };
	inline void ClearForce() { m_totalForce = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f); };	// 合力をクリア
	inline DirectX::SimpleMath::Vector3 GetTotalForce() const { return m_totalForce; }	// 合力を返す

	inline void ClearAcceleration() { m_acceleration = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f); };	// 加速度をクリア

	void ApplyForce(const DirectX::SimpleMath::Vector3& force);	// 外力を加える、構造体かfloatか
	void ApplyFriction_X();	// 摩擦力
	void ApplyFriction_Y();	// 摩擦力
	void ApplyAirRessistance();	// 空気抵抗

	// ここはいったん考える
	void ApplyBounce_Y();		// 反発力（Ｙ軸）
	void ApplyBounce_X();		// 反発力（Ｘ軸）
	void ApplyCollision_X(float collision);
	void ApplyCollision_Y(float collision);

	void CheckStopVelocity();
	void CheckStopVelocity_X();
	void CheckStopVelocity_Y();
	void CheckStopVelocity_Z();

	// 自由落下で使う
	//void TimeStart();
	//void TimeStop();
	//float GetElapsedTime();
};