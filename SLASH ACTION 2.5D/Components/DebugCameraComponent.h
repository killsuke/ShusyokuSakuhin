#pragma once
#include "CameraComponent.h"

class DebugCameraComponent final :  public CameraComponent
{
private:

	DirectX::XMFLOAT2 prevMouse = DirectX::XMFLOAT2(0.0f, 0.0f);	// マウス位置記録

public:
	DebugCameraComponent(GameObject& obj);
	~DebugCameraComponent() = default;

	void Update() override;

};
