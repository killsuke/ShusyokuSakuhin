#pragma once
#include "Component.h"
#include "CameraPattern.h"

class CameraTargetComponent : public Component
{
private:
	CameraPattern cp;	// ƒJƒƒ‰‚É‚Ç‚¤‚¢‚¤“®‚«‚ğ‚³‚¹‚½‚¢‚©

public:
	CameraTargetComponent(GameObject& obj);
	~CameraTargetComponent() = default;

	void Update() override;

	void SetCameraPattern(const CameraPattern& _cp) { cp = _cp; };
	CameraPattern GetCameraPattern() const { return cp; };

};
