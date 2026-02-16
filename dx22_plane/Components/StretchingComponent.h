#pragma once
#include "Component.h"

class StretchingComponent : public Component
{
private:
	float m_BaseScale = 1.0f;	// Šî–{‚Ì‘å‚«‚³
	float m_CurrentScale = 1.0f;	// Œ»İ‚Ì‘å‚«‚³

public:
	StretchingComponent(GameObject& obj);
	~StretchingComponent() = default;
	void Update() override;

	void SetBaseScale(const float scale) { m_BaseScale = scale; };
	void SetCurrentScale(const float scale) { m_CurrentScale = scale; };
};