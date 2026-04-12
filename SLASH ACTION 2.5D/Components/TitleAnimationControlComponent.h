#pragma once
#include "Component.h"
#include "TimeLineComponent.h"
#include <array>

namespace {
	constexpr int EVENT_NUM = 9;
	constexpr int TRIANGLE_NUM = 4;
}

class TitleAnimationControlComponent : public Component
{
private:

	std::array<uint32_t, EVENT_NUM> m_Events{};
	GameObject* m_SlashUI1 = nullptr;
	GameObject* m_SlashUI2 = nullptr;
	std::array<GameObject*, TRIANGLE_NUM> m_Triangles{ nullptr };
	GameObject* m_TitleRogo = nullptr;
	GameObject* m_Sword3D = nullptr;
	GameObject* m_StartUI = nullptr;
	GameObject* m_EndUI = nullptr;
	GameObject* m_MiniSword3D = nullptr;
	bool m_IsMiniSwordUpDown = true;
	bool m_IsloopAnim = false;
	bool m_IsUseSkipAnim = true;

public:

	TitleAnimationControlComponent(GameObject& obj);
	void Update()override;

	void Slash1ActiveOn();
	void Slash2ActiveOn();
	void SlashesActiveOff();
	void TrianglesActiveOn();
	void SwordMoveStart();
	void SwordMove();
	void SwordSpecifiedPosition();
	void LogoRotating();
	void SwordDown();
	void SwordSheath();
	void GameStartWait();
	void SkipAnimation();
};