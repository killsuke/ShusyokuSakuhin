#pragma once
#include "Render.h"
#include "FighterComponent.h"
#include <SimpleMath.h>

class HitFlashComponent :  public RenderComponent
{
private:
	DirectX::SimpleMath::Vector3 m_HitFlashColor = DirectX::SimpleMath::Vector3::Zero;
	float m_HitFlashPower = 0.0f;
	bool m_IsFlash = false;
	uint64_t m_listenerID_HitEvent = 0; // ヒットイベントのリスナーID

public:
	HitFlashComponent(GameObject& obj);
	~HitFlashComponent();

	void Update() override;

	void SetHitFlashColor(const DirectX::SimpleMath::Vector3& color) { m_HitFlashColor = color; };
	void SetHitFlashPower(const float power) { m_HitFlashPower = power; };
	void OnHitFlash(const HitEvent& event);
};