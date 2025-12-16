#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <any>

// 通知で誰と誰に影響させるかを入れて実行して対象者のみに影響させるさせる方法をやるとかは？

// リスナーエントリ構造体
struct ListenerEntry{
	uint64_t id;
	std::function<void(const std::any&)> func;
};

struct QueuedEvent{
	std::type_index type;
	std::any  eventData;
};

enum class ActiveQueue {
	QUEUE_NONE = -1,
	QUEUE_A,
	QUEUE_B
};

class EventBusManager
{
private:

	static std::vector<QueuedEvent> m_QueuedEventsA;
	static std::vector<QueuedEvent> m_QueuedEventsB;
	static inline ActiveQueue m_ActiveQueue = ActiveQueue::QUEUE_A;

	static constexpr int MAX_LOOP = 1000;

	static std::unordered_map<std::type_index, std::vector<ListenerEntry>> m_Listeners;
	static uint64_t m_NextListenerID;

	EventBusManager() = default;
	~EventBusManager() = default;

public:

	static void Init();
	static void Update();
	static void UnInit();

	// イベントをキューに追加
	// ようするに、イベントを発生させる通知処理
	template<typename EventType>
	static void Push(const EventType& event) {

		if (m_ActiveQueue == ActiveQueue::QUEUE_A) {
			m_QueuedEventsA.push_back({ typeid(EventType), event });
		}
		else if (m_ActiveQueue == ActiveQueue::QUEUE_B) {
			m_QueuedEventsB.push_back({ typeid(EventType), event });
		}
	}

	// イベントリスナーを登録
	// ようするに、イベントが発生したときに呼び出される関数を登録する
	template<typename EventType2>
	static uint64_t Subscribe(std::function<void(const EventType2&)>func) {
		
		uint64_t listenerID = m_NextListenerID++;
		
		m_Listeners[typeid(EventType2)].push_back({
			listenerID,
			[func](const std::any& eventData) {
				func(std::any_cast<const EventType2&>(eventData));
			}
		});

		return listenerID;
	}

	// イベントリスナーの登録解除
	static void Unsubscribe(uint64_t& id) {
		for (auto& [type, listeners] : m_Listeners) {
			listeners.erase(
				std::remove_if(
					listeners.begin(),
					listeners.end(),
					[id](const ListenerEntry& entry) {
						return entry.id == id;
					}
				),
				listeners.end()
			);
		}
	}

};