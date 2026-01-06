#pragma once
#include "Render.h"
#include "MotionBlurControllerComponent.h"
#include <DirectXMath.h>

class RenderMotionBlurComponent : public RenderComponent
{
private:
	MotionBlurControllerComponent* m_MotionBlurController = nullptr;

public:
	RenderMotionBlurComponent(GameObject& obj);
	~RenderMotionBlurComponent() = default;

	void Update()override;
	void InitMotionBlurSettings(const int shellCount, const float blurStrength, const bool isUse) {
		if (m_MotionBlurController != nullptr) {
			m_MotionBlurController->SetShellCount(shellCount);
			m_MotionBlurController->SetBlurStrength(blurStrength);
			m_MotionBlurController->SetIsUseBlur(isUse);
		}
	};
};