#pragma once
#include "Component.h"
class GoAroundComponent :  public Component
{
private:
	float m_deltaTime = 0.0f; // 更新間隔時間

	GameObject* m_CenterObject = nullptr; // 中心となるオブジェクト

public:
	GoAroundComponent(GameObject& obj);
	~GoAroundComponent() = default;

	void Update() override;

	inline void SetCenterObject(GameObject* centerObj) { m_CenterObject = centerObj; }
};