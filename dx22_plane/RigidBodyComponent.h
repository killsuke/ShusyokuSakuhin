// -------------------------------------------
// リジットボディ
// （Ｚ軸をコメントアウトしているのは、
// 何かの手違いで裏面に描画されるのを防ぐため）
// -------------------------------------------

#pragma once
#include <DirectXMath.h> // DirextXの数学関連のヘッダーファイル
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
#define STOPVELOCITY (0.01f)	// この値を下回ったら値を０にする 

class RigidBodyComponent:public Component
{
private:
	DirectX::XMFLOAT3 m_velocity{};	 // 速度
	DirectX::XMFLOAT3 m_acceleration{};// 加速度
	float mass = 1.0f;				 // 質量
//	float elapsedTime = 0.0f;		 // 落下中の時間、これで自由落下の計算をする
	bool fallFlag = false;		 	 // 落下のフラグ 
	bool timeFlag = false;			 // 落下タイミングのフラグ
	const float m_deltaTime = 0.016f;		 // 前回の時間からの経過時間
	bool m_gravityFlag = false;	 // 重力を有効にするかどうか
	std::chrono::high_resolution_clock::time_point startTime;	// 計測開始時間
	std::chrono::high_resolution_clock::time_point lastTime;	// 最後の時間

public:
	RigidBodyComponent() = default;
	RigidBodyComponent(const GameObject& obj);

	~RigidBodyComponent() = default;

	void Update()override;

	// 速度返す
	inline void SetVelocity(const DirectX::XMFLOAT3& velocity) { this->m_velocity = velocity; };
	inline DirectX::XMFLOAT3 GetVelocity()const { return m_velocity; };
	inline void AddVelocity(const DirectX::XMFLOAT3& velocity) {
		this->m_velocity.x += velocity.x;
		this->m_velocity.y += velocity.y;
		this->m_velocity.z += velocity.z;
	};

	void ConstantVelocity(const DirectX::XMFLOAT3& velocity);	// 等速運動
	void ConstantVelocity_X(const float velocity);	// 等速運動
	void ConstantVelocity_Y(const float velocity);	// 等速運動
	void ConstantVelocity_Z(const float velocity);	// 等速運動

	void ReduceVelocity(const DirectX::XMFLOAT3& velocity = DirectX::XMFLOAT3{ DAMPINGFACTOR,DAMPINGFACTOR,DAMPINGFACTOR });		// 速度減らし
	void ReduceVelocity_X(const float velocity = DAMPINGFACTOR);		// 速度減らし
	void ReduceVelocity_Y(const float velocity = DAMPINGFACTOR);		// 速度減らし
	void ReduceVelocity_Z(const float velocity = DAMPINGFACTOR);		// 速度減らし

	inline void SetMass(float mass) { this->mass = mass; };
	inline float GetMass() { return mass; };

	inline void SetFallFlag(const bool flag) { this->fallFlag = flag; };
	inline bool GetFallFlag() const { return fallFlag; };

	inline void SetTimeFlag(const bool flag) { this->timeFlag = flag; };

	DirectX::XMFLOAT3& AcceleratorPosition(DirectX::XMFLOAT3& pos);	// 加速度から速度、速度から位置の更新
	float UseGravity(DirectX::XMFLOAT3& pos, const bool gravityFlag, const float firstFallMagnification, const float fallMagnification);		// 重力
	inline void AddForce(const DirectX::XMFLOAT3& force) { this->m_acceleration = force; };
	void ApplyForce(const DirectX::XMFLOAT3& force);	// 外力を加える、構造体かfloatか
	void ApplyFriction_X();	// 摩擦力
	void ApplyFriction_Y();	// 摩擦力
	void ApplyAirRessistance();	// 空気抵抗

	// ここはいったん考える
	void ApplyBounce_Y();		// 反発力（Ｙ軸）
	void ApplyBounce_X();		// 反発力（Ｘ軸）
	void ApplyCollision_X(float collision);
	void ApplyCollision_Y(float collision);

	// 自由落下で使う
	//void TimeStart();
	//void TimeStop();
	//float GetElapsedTime();
};