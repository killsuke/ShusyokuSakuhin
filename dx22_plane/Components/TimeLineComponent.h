#pragma once
#include "Component.h"
#include <vector>
#include <functional>
#include <unordered_map>

// 一度しか発生しないイベント
struct TimePointEvent {
	float triggerTime = 0.0f; // イベントが発生する時間
	std::function<void()> eventAction; // イベントのアクション
	Component* ownerComponent = nullptr; // このイベントを所有するコンポーネントへのポインタ
	bool valid = true; // イベントが有効かどうかのフラグ
};

// 一定時間継続するイベント
struct TimeRangeEvent{
	float startTime = 0.0f; // イベントが開始する時間
	float endTime = 0.0f;   // イベントが終了する時間

	std::function<void(float)> onUpdate; // 0.0 ～ 1.0 正規化時間
	std::function<void()> onStart; // イベント開始時のアクション
	std::function<void()> onEnd;   // イベント終了時のアクション
	Component* ownerComponent = nullptr; // このイベントを所有するコンポーネントへのポインタ

	bool started = false; // イベントが開始されたかどうかのフラグ
	bool ended = false;   // イベントが終了したかどうかのフラグ
	bool valid = true;  // イベントが有効かどうかのフラグ
};

class TimeLineComponent : public Component
{
private:
	float m_CurrentTime = 0.0f; // 現在のタイムライン時間
	size_t m_NextEventIndex = 0; // 次に発生するイベントのインデックス
	std::vector<TimePointEvent> m_PointEvents; // タイムラインイベントのリスト
	std::vector<TimeRangeEvent> m_RangeEvents; // 一定時間継続するイベントのリスト
	std::unordered_map<Component*, std::vector<size_t>> m_ComponentEventMap; // コンポーネントごとのイベントインデックスマップ

	void UpdatePointEvents();
	void UpdateRangeEvents();

public:
	TimeLineComponent(GameObject& obj);
	~TimeLineComponent();
	void Update() override;
	
	void AddPointEvent(const float time, Component* owner, std::function<void()> action);
	void AddRangeEvent(
		const float startTime, const float endTime, Component* owner,
		std::function<void(float)> onUpdate,
		std::function<void()> onStart,
		std::function<void()> onEnd);

	void AddPointDelayEvent(const float delayTime, Component* owner, std::function<void()> action);
	void AddRangeDelayEvent(const float startTime, const float endTime, const float delayTime, Component* owner,
		std::function<void(float)> onUpdate,
		std::function<void()> onStart,
		std::function<void()> onEnd);
	void RemoveEventsByComponent(Component* owner);
};
