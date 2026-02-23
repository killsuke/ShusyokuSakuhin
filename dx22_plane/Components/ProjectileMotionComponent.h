#pragma once
#include "Component.h"
#include <DirectXMath.h>

class ProjectileMotionComponent final : public Component
{
private:
	DirectX::XMVECTOR m_ProjectileDirection = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_ProjectileRotation = DirectX::XMFLOAT3(0.0f,0.0f,0.0f);
	float m_ProjectilePower = 0.0f;

public:

	ProjectileMotionComponent(GameObject& obj);
	~ProjectileMotionComponent() = default;
	void Update() override;

	// 投射運動の向き
	void SetProjectileDirection(const DirectX::XMFLOAT3& dir) {
		m_ProjectileDirection = DirectX::XMLoadFloat3(&dir);
		m_ProjectileDirection = DirectX::XMVector3Normalize(m_ProjectileDirection);
	};

	// 足し込む回転の値
	void SetProjectileRotation(const DirectX::XMFLOAT3& rot) {
		m_ProjectileRotation = rot;
	}

	// 投射運動のパワー
	void SetProjectilePower(const float power) {
		m_ProjectilePower = std::fabsf(power); 
	};

	// 投射運動の初期化処理
	// →投射運動の開始時のみに使う
	void InitProjectile(const DirectX::XMFLOAT3& dir, const DirectX::XMFLOAT3& rot, const float power) {
		SetProjectileDirection(dir);
		SetProjectileRotation(rot);
		SetProjectilePower(power);
	};

	// 値のリセット
	void ResetProjectile() {
		m_ProjectileDirection = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_ProjectileRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_ProjectilePower = 0.0f;
	}
};

