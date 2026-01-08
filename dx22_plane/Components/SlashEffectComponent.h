#pragma once
#include "RenderLuminescenceBillboardComponent.h"
#include <DirectXMath.h>

class SlashEffectComponent : public Component
{
private:
	DirectX::XMFLOAT2 m_SizeChange = { 1.0f,1.0f }; // サイズ変更量
	float m_RecordTime = 0.0f; // サイズ変更時間記録用
	float m_RimitTime = 0.0f; // サイズ変更時間リミット
	float power = 0.2f;

public:
	SlashEffectComponent(GameObject& obj);
	~SlashEffectComponent() = default;
	void Update() override;

	void SetSizeChange(const DirectX::XMFLOAT2& size) { m_SizeChange = size; };
	void SetRimitTime(const float time) { m_RimitTime = time; };
};
