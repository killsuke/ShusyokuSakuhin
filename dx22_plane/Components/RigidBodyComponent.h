// -------------------------------------------
// リジットボディ
// （Ｚ軸をコメントアウトしているのは、
// 何かの手違いで裏面に描画されるのを防ぐため）
// -------------------------------------------

#pragma once
#include "Transform.h"
#include "Component.h"
#include <cmath>

class RigidBodyComponent final : public Component
{
private:
	DirectX::XMFLOAT3 m_Velocity{};			// 速度
	DirectX::XMFLOAT3 m_Acceleration{};		// 加速度
	DirectX::XMFLOAT3 m_TotalForce{};       // 合力
	DirectX::XMFLOAT3 m_LimitVelocity{};    // 速度制限
	bool m_FallFlag = false;		 		// 落下のフラグ 
	bool m_GravityFlag = false;				// 重力を有効にするかどうかのフラグ
	bool m_BeforeGravityFlag = false;		// 重力を有効にするかどうか
	float m_Mass = 1.0f;					// 質量
	float m_FirstFallMagnification = 0.0f;  // 初回の落下倍率
	float m_FallMagnification = 0.0f;		// 落下倍率
	float m_StopGravity = 0.0f;			    // 重力固定パワー
	static inline float DAMPINGFACTOR = 0.9f;	// 減衰率	

public:
	RigidBodyComponent(GameObject& obj);
	~RigidBodyComponent() = default;

	void Update()override;

	void UpdateVelocity();

	// 速度返す
	inline void SetVelocity(const DirectX::XMFLOAT3& velocity) { m_Velocity = velocity; };
	inline void SetLimitVelocity(const DirectX::XMFLOAT3& velocity) { m_LimitVelocity = velocity; };
	inline void SetLimitVelocity_X(const float velocity) { m_LimitVelocity.x = velocity; };
	inline void SetLimitVelocity_Y(const float velocity) { m_LimitVelocity.y = velocity; };
	inline void SetLimitVelocity_Z(const float velocity) { m_LimitVelocity.z = velocity; };
	inline DirectX::XMFLOAT3 GetVelocity()const { return m_Velocity; };
	inline void AddVelocity(const DirectX::XMFLOAT3& velocity) { m_Velocity += velocity; };

	void ConstantVelocity(const DirectX::XMFLOAT3& velocity);	// 等速運動
	void ConstantVelocity_X(const float velocity);	// 等速運動
	void ConstantVelocity_Y(const float velocity);	// 等速運動
	void ConstantVelocity_Z(const float velocity);	// 等速運動

	void ReduceVelocity(const DirectX::XMFLOAT3& velocity = DirectX::XMFLOAT3{ DAMPINGFACTOR,DAMPINGFACTOR,DAMPINGFACTOR });		// 速度減らし
	void ReduceVelocity_X(const float velocity = DAMPINGFACTOR);		// 速度減らし
	void ReduceVelocity_Y(const float velocity = DAMPINGFACTOR);		// 速度減らし
	void ReduceVelocity_Z(const float velocity = DAMPINGFACTOR);		// 速度減らし

	inline void ClearVelocity() { m_Velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); };
	inline void ClearVelocity_X() { m_Velocity.x = 0.0f; };
	inline void ClearVelocity_Y() { m_Velocity.y = 0.0f; };
	inline void ClearVelocity_Z() { m_Velocity.z = 0.0f; };

	inline void SetMass(const float mass) { m_Mass = mass; };
	inline float GetMass()const { return m_Mass; };

	inline void SetFallFlag(const bool flag) { m_FallFlag = flag; };
	inline bool GetFallFlag() const { return m_FallFlag; };

	inline void SetGravityFlag(const bool flag) { m_GravityFlag = flag; }	// 重力を有効にするかどうかのフラグを設定
	inline bool GetGravityFlag() const { return m_GravityFlag; }	// 重力を有効にするかどうかのフラグを取得

	inline void SetFirstFallMagnification(const float magnification) { m_FirstFallMagnification = magnification; }	// 初回の落下倍率を設定
	inline float GetFirstFallMagnification() const { return m_FirstFallMagnification; }	// 初回の落下倍率を取得

	inline void SetFallMagnification(const float magnification) { m_FallMagnification = magnification; }	// 落下倍率を設定
	inline float GetFallMagnification() const { return m_FallMagnification; }	// 落下倍率を取得

	inline void SetStopGravity(const float stopGravity) { m_StopGravity = fabsf(stopGravity); }	// 重力固定パワーを設定

	float UseGravity(const bool gravityFlag);		// 重力
	inline void AddForce(const DirectX::XMFLOAT3& force) { m_TotalForce += force; };
	inline void ClearForce() { m_TotalForce = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); };	// 合力をクリア
	inline DirectX::XMFLOAT3 GetTotalForce() const { return m_TotalForce; }	// 合力を返す

	inline void ClearAcceleration() { m_Acceleration = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); };	// 加速度をクリア

	void ApplyForce(const DirectX::XMFLOAT3& force);	// 外力を加える、構造体かfloatか
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
};