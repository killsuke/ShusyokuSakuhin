#pragma once
#include "Component.h"
#include <vector>
#include <DirectXMath.h>

class ChargePerformanceComponent : public Component
{
private:
	std::vector<GameObject*> m_ChargeParticles;
	GameObject* m_ChargeCompleteParticle = nullptr;
	float m_RecordTime = 0.0f;
	float m_RecordTime2 = 0.0f;
	float m_ScaleChangePower = 0.0f;
	float m_ScaleChangeSpeed = 0.0f;
	float m_OneParticleActiveTime = 0.0f;		// １つ辺りのパーティクルをアクティブにするまでの時間
	
	bool m_IsCompleteCharge = false;	// チャージ完了フラグ

	DirectX::XMFLOAT3 m_InitScale = { 1.0f,1.0f,1.0f };

public:
	ChargePerformanceComponent(GameObject& obj);
	~ChargePerformanceComponent() = default;

	void Update() override;

	void InitCreateParticles(const unsigned int num, const float radius, GameObject& parent,const float bigRadius,const float power, const float speed);	// パーティクルを生成
	void SetOneParticleActiveTime(const float time) { m_OneParticleActiveTime = time; };
	void ResetAllParticles(); // 全てのパーティクルを非アクティブに戻す
	void SetChargeCompleteFlag(const bool flag) { m_IsCompleteCharge = flag; };
};