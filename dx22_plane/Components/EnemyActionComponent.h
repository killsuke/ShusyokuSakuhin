#pragma once
#include "Component.h"
#include "FighterComponent.h"

class EnemyActionComponent : public Component
{
protected:
	const float m_deltaTime = 0.016f;
	float m_recordTime = 0.0f;
	bool m_IsRightLeft = true; // 左右の移動フラグ、初期は右向き(true:右、false:左)

	EnemyActionComponent(GameObject& obj);
	~EnemyActionComponent() = default;

public:
	
	virtual void Update() = 0;

	void CreateDamageEffect();
	bool GetRightLeft()const { return m_IsRightLeft; };
};