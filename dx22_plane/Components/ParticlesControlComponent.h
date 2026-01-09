#pragma once
#include "Component.h"

class ParticlesControlComponent : public Component
{
private:
	float m_VanishTime = 0.0f;	// Œv‘ª‚µ‚ÄÁ‚¦‚Ä—~‚µ‚¢ŠÔ
	float m_RecordTime = 0.0f;


public:
	ParticlesControlComponent(GameObject& obj);
	~ParticlesControlComponent() = default;
	void Update() override;
	void SetVanishTime(const float time) { m_VanishTime = fabsf(time); };
};
