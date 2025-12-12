#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>

// 通知で誰と誰に影響させるかを入れて実行して対象者のみに影響させるさせる方法をやるとかは？

struct QueuedEvent{
	std::type_index type;
	void* eventData;
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

	static std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> m_Listeners;

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
			m_QueuedEventsA.push_back({ typeid(EventType), new EventType(event) });
		}
		else if (m_ActiveQueue == ActiveQueue::QUEUE_B) {
			m_QueuedEventsB.push_back({ typeid(EventType), new EventType(event) });
		}
	}

	// イベントリスナーを登録
	// ようするに、イベントが発生したときに呼び出される関数を登録する
	template<typename EventType2>
	static void Subscribe(std::function<void(const EventType2&)>func) {
		m_Listeners[typeid(EventType2)].push_back(
			[func](const void* ptr) {
				func(*static_cast<const EventType2*>(ptr));
			}
		);
	}

};

// テストコード、
// 例：ダメージイベントなら、構造体に自分と相手あてのIDとダメージ量を持たせるなど
//void DamageComponent::OnHit(const HitEvent& e) {
//	if (e.targetID != ownerID) return; // ← 自分宛じゃないなら無視
//	hp -= e.damage;
//}