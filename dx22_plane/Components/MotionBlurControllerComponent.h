#pragma once
#include "Transform.h"

enum class EBlurPattern {
	NONE = 0,
	LINEAR,
	CIRCULAR,
	MAX_BLURPATTERN
};

class MotionBlurControllerComponent : public Component
{
private:
	DirectX::XMMATRIX m_PrevMtx = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_PrevMtx2 = DirectX::XMMatrixIdentity();
	EBlurPattern m_BlurPattern = EBlurPattern::NONE; // ブラーのパターン
	int m_ShellCount = 0;	// シェルの数（多いほど滑らか）
	float m_BlurStrength = 0.0f; // ブラーの強さ
	bool m_IsUseBlur = false; // モーションブラーを使うかどうか

public:
	MotionBlurControllerComponent(GameObject& obj);
	~MotionBlurControllerComponent() = default;

	void Update()override;

	void SetShellCount(const int count) { m_ShellCount = count; };
	void SetIsUseBlur(const bool flag) {
		m_IsUseBlur = flag;
		if (flag == false) {
			m_PrevMtx = DirectX::XMMatrixIdentity();
			m_PrevMtx2 = DirectX::XMMatrixIdentity();
		}
	};
	void SetBlurStrength(const float strength) { m_BlurStrength = strength; };
	void SetBlurPattern(const EBlurPattern& pattern) { m_BlurPattern = pattern; };

	DirectX::XMMATRIX GetPrevMatrix()const { return m_PrevMtx; };
	int GetShellCount()const { return m_ShellCount; };
	float GetBlurStrength()const { return m_BlurStrength; };
	bool GetIsUseBlur()const { return m_IsUseBlur; };
	EBlurPattern GetBlurPattern()const { return m_BlurPattern; };
};