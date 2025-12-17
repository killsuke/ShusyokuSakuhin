#pragma once
#include "Component.h"
#include <string>

class TimeSpawnParticlesComponent : public Component
{
private:
	std::string m_TextureName = "";
	float m_SpawnInterval = 0.0f;
	float m_InitInterval = 0.0f;

public:
	~TimeSpawnParticlesComponent() = default;
	TimeSpawnParticlesComponent(GameObject& obj);
	void Update();
	void SetSpawnInterval(const float interval) { 
		m_SpawnInterval = fabsf(interval);
		m_InitInterval = m_SpawnInterval;
	}
};