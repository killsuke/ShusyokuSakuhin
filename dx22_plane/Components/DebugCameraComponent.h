#pragma once
#include "Camera.h"

class DebugCameraComponent final :  public Camera
{
private:


public:
	DebugCameraComponent(GameObject& obj);
	~DebugCameraComponent() = default;

	void Update() override;

};
