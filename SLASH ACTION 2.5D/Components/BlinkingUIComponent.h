#pragma once
#include "Component.h"

// 任意の最低値と最大値の間でサイン波で透明・不透明にしよう
class BlinkingUIComponent : public Component
{
private:
	bool m_blinkingFlag = true; // 点滅フラグ
	float m_recordTime = 0.0f; // 点滅している時間を記録する
	float m_blinkingSpeed = 1.0f; // 点滅の速さ

public:
	BlinkingUIComponent(GameObject& obj);
	~BlinkingUIComponent() = default;
	void Update()override;
	void SetBlinkingFlag(const bool flag) { m_blinkingFlag = flag; };
	bool GetBlinkingFlag() const { return m_blinkingFlag; };

	void SetBlinkingSpeed(const float speed) { m_blinkingSpeed = speed; };
};
