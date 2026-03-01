#pragma once
#include "TransformComponent.h"

class ChargeParticleMoveComponent : public Component
{
private:
	float m_MoveSpeed = 0.0f; // 移動速度
	float m_RimitDistance = 1.0f; // ターゲットに到達したと見なす距離
	GameObject* m_TargetObject = nullptr; // 移動先のターゲットオブジェクト
	DirectX::XMFLOAT3 m_StartPosition = { 0.0f,0.0f,0.0f }; // 開始位置
	DirectX::XMFLOAT3 m_InitScale = { 1.0f,1.0f,1.0f };

public:
	ChargeParticleMoveComponent(GameObject& obj);
	~ChargeParticleMoveComponent() = default;
	void Update() override;

	void SetTargetObject(GameObject& target) { m_TargetObject = &target; };
	void SetMoveSpeed(const float speed) { m_MoveSpeed = speed; };
	void SetRimitDistance(const float distance) { m_RimitDistance = distance; };
	void SetStartPosition(const DirectX::XMFLOAT3& pos) { m_StartPosition = pos; };
	void SetInitScale(const DirectX::XMFLOAT3& scale) { m_InitScale = scale; };
	void ResetProcess();
};