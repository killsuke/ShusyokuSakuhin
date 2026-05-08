#pragma once
#include "Component.h"
#include "Structs/CameraPattern.h"

class CameraTargetComponent : public Component
{
private:
	CameraPattern m_CameraPattern;	// ƒJƒƒ‰‚É‚Ç‚¤‚¢‚¤“®‚«‚ğ‚³‚¹‚½‚¢‚©
	float m_SpringKonstante = 20.0f; // ‚Î‚Ë’è”

public:
	CameraTargetComponent(GameObject& obj);
	~CameraTargetComponent() = default;

	void Update() override;

	void SetCameraPattern(const CameraPattern& _cp) { m_CameraPattern = _cp; };
	void SetSpringK(const float konstante) { m_SpringKonstante = konstante; };
	float GetSpringK() const { return m_SpringKonstante; };
	CameraPattern GetCameraPattern() const { return m_CameraPattern; };
};
