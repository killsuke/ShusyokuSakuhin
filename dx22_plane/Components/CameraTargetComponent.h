#pragma once
#include "Component.h"
#include "Structs/CameraPattern.h"

class CameraTargetComponent : public Component
{
private:
	CameraPattern cp;	// ƒJƒƒ‰‚É‚Ç‚¤‚¢‚¤“®‚«‚ğ‚³‚¹‚½‚¢‚©
	float m_springK = 20.0f; // ‚Î‚Ë’è”

public:
	CameraTargetComponent(GameObject& obj);
	~CameraTargetComponent() = default;

	void Update() override;

	void SetCameraPattern(const CameraPattern& _cp) { cp = _cp; };
	inline void SetSpringK(float k) { m_springK = k; };
	inline float GetSpringK() const { return m_springK; };
	CameraPattern GetCameraPattern() const { return cp; };

};
