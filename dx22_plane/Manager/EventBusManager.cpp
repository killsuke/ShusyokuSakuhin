#include "EventBusManager.h"

std::vector<QueuedEvent> EventBusManager::queuedEvents;
std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> EventBusManager::m_Listeners;

void EventBusManager::Init()
{
	// 初期化処理
	queuedEvents.clear();
	m_Listeners.clear();
}

void EventBusManager::Update()
{
	if(queuedEvents.size() == 0 || m_Listeners.size() == 0){
		return; // 中身が空なら何もしない
	}
	
	// キューに溜まったイベントを処理
	for (const QueuedEvent& item : queuedEvents) {
		std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>>::iterator it =
			m_Listeners.find(item.type);

		if (it != m_Listeners.end()) {
			// 登録されているリスナーを呼び出す
			for (const std::function<void(const void*)>& listener : it->second) {
				listener(item.eventData);
			}
		}
		delete item.eventData;
	}
	queuedEvents.clear();
}

void EventBusManager::UnInit()
{
	// キューに残っているイベントデータを解放
	queuedEvents.clear();
	m_Listeners.clear();
}