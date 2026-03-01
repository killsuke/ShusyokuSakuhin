#pragma once
#include "Component.h"

class ChargeSlashComponent : public Component
{
private:
	float m_RecordTime = 0.0f;
	float m_RimitTime = 0.0f;

public:
	ChargeSlashComponent(GameObject& obj);
	~ChargeSlashComponent() = default;

	void Update() override;
	void SetRimitTime(const float time) { m_RimitTime = time; };
};