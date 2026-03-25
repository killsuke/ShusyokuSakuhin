#include "HitStopManager.h"
#include "GameObjectManager.h"
#include "TimeManager.h"
#include "GameObject/GameObject.h"
#include <vector>

std::unordered_set<std::string> HitStopManager::m_TargetTags;
float HitStopManager::m_HitStopTime = 0.0f;
float HitStopManager::m_RecordTime = 0.0f;
bool HitStopManager::m_IsHitStopActive = true;

void HitStopManager::Update()
{
	if(m_IsHitStopActive == false)
	{
		return;
	}

	if(m_HitStopTime > 0.0f)
	{
		if(m_RecordTime == 0.0f)
		{
			// 最初のフレームだけ、オブジェクトの更新を止める
			std::vector<GameObject*>objs = GameObjectManager::GameObjectFindAllTag(m_TargetTags);
			for (GameObject* obj : objs)
			{
				ActiveState ac = obj->GetActiveState();
				if (ac == ActiveState::ACTIVE) {
					obj->SetActiveState(ActiveState::UPDATE_STOP);
				}
			}
		}

		m_RecordTime += TimeManager::GetFixedDeltaTime();
		if(m_RecordTime >= m_HitStopTime)
		{
			// ヒットストップ終了
			m_HitStopTime = 0.0f;
			m_RecordTime = 0.0f;
			// オブジェクトの更新を再開する
			std::vector<GameObject*>objs = GameObjectManager::GameObjectFindAllTag(m_TargetTags);
			for (GameObject* obj : objs)
			{
				ActiveState ac = obj->GetActiveState();
				if (ac == ActiveState::UPDATE_STOP) {
					obj->SetActiveState(ActiveState::ACTIVE);
				}
			}
		}
	}
}