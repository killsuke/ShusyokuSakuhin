#pragma once
#include "Component.h"

class TitleAnimationControlComponent : public Component
{
private:
	GameObject* m_SlashUI1 = nullptr;
	GameObject* m_SlashUI2 = nullptr;
	GameObject* m_Triangle1 = nullptr;
	GameObject* m_Triangle2 = nullptr;
	GameObject* m_Triangle3 = nullptr;
	GameObject* m_Triangle4 = nullptr;
	GameObject* m_TitleRogo = nullptr;
	GameObject* m_Sword3D = nullptr;
	GameObject* m_StartUI = nullptr;
	GameObject* m_EndUI = nullptr;
	GameObject* m_MiniSword3D = nullptr;
	bool m_IsMiniSwordUpDown = true;

public:

	TitleAnimationControlComponent(GameObject& obj);
	~TitleAnimationControlComponent() = default;
	void Update()override;

	void Slash1ActiveOn();
	void Slash2ActiveOn();
	void SlashesActiveOff();
	void TrianglesActiveOn();
	void SwordMove();
	void SwordSpecifiedPosition();
	void LogoRotating();
	void SwordDown();
	void GameStartWait();
};