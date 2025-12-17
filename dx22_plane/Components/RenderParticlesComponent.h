#pragma once
#include "Render.h"

class RenderParticlesComponent :  public RenderComponent
{
private:
	float m_VanishTime = 0.0f;	// Œv‘ª‚µ‚ÄÁ‚¦‚Ä—~‚µ‚¢ŠÔ
	float m_RecordTime = 0.0f;

public:

	RenderParticlesComponent(GameObject& obj);
	~RenderParticlesComponent() = default;
	void Update()override;
	void SetVanishTime(const float time) { m_VanishTime = fabsf(time); };
};