#pragma once
#include "RenderComponent.h"
#include "FighterComponent.h"
#include <DirectXMath.h>

class HitFlashComponent :  public RenderComponent
{
private:
	DirectX::XMFLOAT3 m_HitFlashColor = DirectX::XMFLOAT3();
	float m_HitFlashPower = 0.0f;
	bool m_IsFlash = false;
	uint64_t m_ListenerID_HitEvent = 0; // ヒットイベントのリスナーID

public:
	HitFlashComponent(GameObject& obj);
	~HitFlashComponent();

	void Update() override;

	void SetHitFlashColor(const DirectX::XMFLOAT3& color) { m_HitFlashColor = color; };
	void SetHitFlashPower(const float power) { m_HitFlashPower = power; };
	void OnHitFlash(const HitEvent& event);
};