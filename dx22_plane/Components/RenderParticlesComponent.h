#pragma once
#include "Render.h"
#include "ParticlesControlComponent.h"

class RenderParticlesComponent :  public RenderComponent
{
private:
	ParticlesControlComponent* m_ParticlesControl = nullptr;

public:

	RenderParticlesComponent(GameObject& obj);
	~RenderParticlesComponent() = default;
	void Update()override;
	void SetVanishTime(const float time) { m_ParticlesControl->SetVanishTime(fabsf(time)); };
};