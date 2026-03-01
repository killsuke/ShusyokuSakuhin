#pragma once
#include "RenderComponent.h"

class RenderCharacterComponent final : public RenderComponent
{
private:
	float m_BaseScale = 1.0f;	// Šî–{‚Ì‘å‚«‚³
	float m_CurrentScale = 0.0f;	// Œ»İ‚Ì‘å‚«‚³

public:

	RenderCharacterComponent(GameObject& obj);
	~RenderCharacterComponent() = default;
	void Update() override;

	void SetBaseScale(const float scale) { m_BaseScale = scale; };
	void SetCurrentScale(const float scale) { m_CurrentScale = scale; };
};