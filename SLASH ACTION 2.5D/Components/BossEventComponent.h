#pragma once
#include "TimeLineComponent.h"

class BossEventComponent : public Component
{
private:
	float m_RecordTime = 0.0f;
	bool m_CreateCompletionFlag = false;
	bool m_IsBossDied = false;
	GameObject* m_Boss = nullptr;
	TimeLineComponent* m_TimeLine = nullptr;

public:
	BossEventComponent(GameObject& obj);
	~BossEventComponent() = default;

	void Update()override;

	void CreateBossWalls();
	void CreateBossObj();

	void PlayerControlStop();
	void PlayerControlRestart();
	void PlayerMoveControl();

	void TransferResultScene();
	void DeadCameraShakeAndScreenStop();
	void ScreenReStart();
};