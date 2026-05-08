#pragma once
#include "Component.h"

// 任意の最低値と最大値の間でサイン波で透明・不透明にしよう
class BlinkingUIComponent : public Component
{
private:
	bool m_IsBlinkingFlag = true; // 点滅フラグ
	float m_RecordTime = 0.0f; // 点滅している時間を記録する
	float m_BlinkingSpeed = 1.0f; // 点滅の速さ

public:
	BlinkingUIComponent(GameObject& obj);
	~BlinkingUIComponent() = default;
	void Update()override;
	void SetBlinkingFlag(const bool flag) { m_IsBlinkingFlag = flag; };
	bool GetBlinkingFlag() const { return m_IsBlinkingFlag; };

	void SetBlinkingSpeed(const float speed) { m_BlinkingSpeed = speed; };
};
