// ============================================================
// EventBusManager.cpp
// バケツの水を入れ替えるように、イベントキューを
// 二つ用意して交互に処理する仕様なので、パフォーマンスに
// 影響が出る可能性があることに注意。
// ============================================================

#include "EventBusManager.h"

std::vector<QueuedEvent> EventBusManager::m_QueuedEventsA;
std::vector<QueuedEvent> EventBusManager::m_QueuedEventsB;
std::unordered_map<std::type_index, std::vector<ListenerEntry>> EventBusManager::m_Listeners;
uint64_t EventBusManager::m_NextListenerID = 1;

void EventBusManager::Init()
{
	// 初期化処理
	m_QueuedEventsA.clear();
	m_QueuedEventsB.clear();
	m_ActiveQueue = ActiveQueue::QUEUE_A; // デフォルトでQUEUE_Aをアクティブに設定
	m_Listeners.clear();
}

void EventBusManager::Update()
{
	if(m_Listeners.size() == 0){
		return; // 中身が空なら何もしない
	}
	
	int loopCount = 0;

	// イベントキューの切り替え
	// どちらも空になるまでループ
	while (!m_QueuedEventsA.empty() || !m_QueuedEventsB.empty()) {

		// 無限ループ防止
		if (++loopCount > MAX_LOOP) {
			break;
		}

		// 処理するキューを選択
		std::vector<QueuedEvent>& currentQueue =
			(m_ActiveQueue == ActiveQueue::QUEUE_A) ? m_QueuedEventsA : m_QueuedEventsB;
	/*	std::vector<QueuedEvent>& nextQueue =
			(m_ActiveQueue == ActiveQueue::QUEUE_A) ? m_QueuedEventsB : m_QueuedEventsA;*/

		// 次は反対のキューをアクティブにする
		m_ActiveQueue = (m_ActiveQueue == ActiveQueue::QUEUE_A) ? ActiveQueue::QUEUE_B : ActiveQueue::QUEUE_A;

		// キューに溜まったイベントを処理
		for (const QueuedEvent& item : currentQueue) {
			std::unordered_map<std::type_index, std::vector<ListenerEntry>>::iterator it =
				m_Listeners.find(item.type);
			if (it != m_Listeners.end()) {
				// 登録されているリスナーを呼び出す
				for (const ListenerEntry& listener : it->second) {
					listener.func(item.eventData);
				}
			}
		}
		currentQueue.clear();
	}

	m_QueuedEventsA.clear();
	m_QueuedEventsB.clear();
}

void EventBusManager::UnInit()
{
	// キューに残っているイベントデータを解放
	m_QueuedEventsA.clear();
	m_QueuedEventsB.clear();
	m_Listeners.clear();
}