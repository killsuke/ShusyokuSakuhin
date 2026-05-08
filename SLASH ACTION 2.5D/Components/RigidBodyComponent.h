// -------------------------------------------
// リジットボディ
// （Ｚ軸をコメントアウトしているのは、
// 何かの手違いで裏面に描画されるのを防ぐため）
// -------------------------------------------

#pragma once
#include "TransformComponent.h"
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
	void SetVelocity(const DirectX::XMFLOAT3& velocity) { m_Velocity = velocity; };
	void SetLimitVelocity(const DirectX::XMFLOAT3& velocity) { m_LimitVelocity = velocity; };
	void SetLimitVelocity_X(const float velocity) { m_LimitVelocity.x = velocity; };
	void SetLimitVelocity_Y(const float velocity) { m_LimitVelocity.y = velocity; };
	void SetLimitVelocity_Z(const float velocity) { m_LimitVelocity.z = velocity; };
	DirectX::XMFLOAT3 GetVelocity()const { return m_Velocity; };
	void AddVelocity(const DirectX::XMFLOAT3& velocity) { m_Velocity += velocity; };
	void AddVelocity_X(const float velocity) { m_Velocity.x = velocity; };
	void AddVelocity_Y(const float velocity) { m_Velocity.y = velocity; };
	void AddVelocity_Z(const float velocity) { m_Velocity.z = velocity; };

	void ConstantVelocity(const DirectX::XMFLOAT3& velocity);	// 等速運動
	void ConstantVelocity_X(const float velocity);	// 等速運動
	void ConstantVelocity_Y(const float velocity);	// 等速運動
	void ConstantVelocity_Z(const float velocity);	// 等速運動

	void ReduceVelocity(const DirectX::XMFLOAT3& velocity = DirectX::XMFLOAT3{ DAMPINGFACTOR,DAMPINGFACTOR,DAMPINGFACTOR });		// 速度減らし
	void ReduceVelocity_X(const float velocity = DAMPINGFACTOR);		// 速度減らし
	void ReduceVelocity_Y(const float velocity = DAMPINGFACTOR);		// 速度減らし
	void ReduceVelocity_Z(const float velocity = DAMPINGFACTOR);		// 速度減らし

	void ClearVelocity() { m_Velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); };
	void ClearVelocity_X() { m_Velocity.x = 0.0f; };
	void ClearVelocity_Y() { m_Velocity.y = 0.0f; };
	void ClearVelocity_Z() { m_Velocity.z = 0.0f; };

	void SetMass(const float mass) { m_Mass = mass; };
	float GetMass()const { return m_Mass; };

	void SetFallFlag(const bool flag) { m_FallFlag = flag; };
	bool GetFallFlag() const { return m_FallFlag; };

	void SetGravityFlag(const bool flag) { m_GravityFlag = flag; }	// 重力を有効にするかどうかのフラグを設定
	bool GetGravityFlag() const { return m_GravityFlag; }	// 重力を有効にするかどうかのフラグを取得

	void SetFirstFallMagnification(const float magnification) { m_FirstFallMagnification = magnification; }	// 初回の落下倍率を設定
	float GetFirstFallMagnification() const { return m_FirstFallMagnification; }	// 初回の落下倍率を取得

	void SetFallMagnification(const float magnification) { m_FallMagnification = magnification; }	// 落下倍率を設定
	float GetFallMagnification() const { return m_FallMagnification; }	// 落下倍率を取得

	void SetStopGravity(const float stopGravity) { m_StopGravity = fabsf(stopGravity); }	// 重力固定パワーを設定

	float UseGravity(const bool gravityFlag);		// 重力
	void AddForce(const DirectX::XMFLOAT3& force) { m_TotalForce += force; };
	void AddForce_X(const float force) { m_TotalForce.x += force; };
	void AddForce_Y(const float force) { m_TotalForce.y += force; };
	void AddForce_Z(const float force) { m_TotalForce.z += force; };
	void ClearForce() { m_TotalForce = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); };	// 合力をクリア
	DirectX::XMFLOAT3 GetTotalForce() const { return m_TotalForce; }	// 合力を返す

	void ClearAcceleration() { m_Acceleration = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); };	// 加速度をクリア

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