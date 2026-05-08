#pragma once
#include "Component.h"

class StretchingComponent : public Component
{
private:
	float m_BaseScale = 1.0f;		// Šî–{‚Ì‘å‚«‚³
	float m_CurrentScale = 1.0f;	// Œ»İ‚Ì‘å‚«‚³
	float m_RecordTime = 0.0f;		// Œo‰ßŠÔ‚Ì‹L˜^
	float m_StretchSpeed = 1.0f;	// Lk‚Ìü”g”
	float m_StretchPower = 0.5f;	// Lk‚ÌU•
	float m_HolderTime = 0.0f;		// Lk‚Ì•ÛŠÔ

public:
	StretchingComponent(GameObject& obj);
	~StretchingComponent() = default;
	void Update() override;

	void SetBaseScale(const float scale) { m_BaseScale = scale; };
	void SetCurrentScale(const float scale) { m_CurrentScale = scale; };
	void SetStretchSpeed(const float speed) { m_StretchSpeed = speed; };
	void SetStretchPower(const float power) { m_StretchPower = power; };
	void SetHolderTime(const float time) { m_HolderTime = time; };
};