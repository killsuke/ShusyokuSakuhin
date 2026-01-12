#pragma once
#include "Component.h"
#include <vector>
#include <DirectXMath.h>

class ChargePerformanceComponent : public Component
{
private:
	std::vector<GameObject*> m_ChargeParticles;
	float m_RecordTime = 0.0f;
	float m_OneParticleActiveTime = 0.0f;		// １つ辺りのパーティクルをアクティブにするまでの時間

public:
	ChargePerformanceComponent(GameObject& obj);
	~ChargePerformanceComponent() = default;

	void Update() override;

	void InitCreateParticles(const unsigned int num, const float radius, GameObject& parent);
	void SetOneParticleActiveTime(const float time) { m_OneParticleActiveTime = time; };
};