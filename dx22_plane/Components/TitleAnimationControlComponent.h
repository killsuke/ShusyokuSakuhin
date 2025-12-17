#pragma once
#include "Component.h"

class TitleAnimationControlComponent : public Component
{
private:
	float m_RecordTime = 0.0f;
	GameObject* m_SlashUI1 = nullptr;
	GameObject* m_SlashUI2 = nullptr;

public:
	TitleAnimationControlComponent(GameObject& obj);
	~TitleAnimationControlComponent() = default;
	void Update()override;

};