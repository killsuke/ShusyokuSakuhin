#pragma once
#include "Camera.h"

class DebugCameraComponent final :  public Camera
{
private:

	DirectX::XMFLOAT2 prevMouse = DirectX::XMFLOAT2(0.0f, 0.0f);	// マウス位置記録

public:
	DebugCameraComponent(GameObject& obj);
	~DebugCameraComponent() = default;

	void Update() override;

};
