#pragma once
#include "Component.h"
#include <vector>
#include <functional>
#include <unordered_map>

// 一度しか発生しないイベント
struct TimePointEvent {
	uint32_t eventID = 0; // イベントのID
	float triggerTime = 0.0f; // イベントが発生する時間
	std::function<void()> eventAction; // イベントのアクション
	Component* ownerComponent = nullptr; // このイベントを所有するコンポーネントへのポインタ
	bool valid = true; // イベントが有効かどうかのフラグ
};

// 一定時間継続するイベント
struct TimeRangeEvent{
	uint32_t eventID = 0; // イベントのID
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

struct TimeContinuousEvent {

	uint32_t eventID = 0; // イベントのID
	float triggerTime = 0.0f; // イベントが発生する時間
	std::function<void()> eventAction; // イベントのアクション
	Component* ownerComponent = nullptr; // このイベントを所有するコンポーネントへのポインタ
	bool valid = true; // イベントが有効かどうかのフラグ
};

class TimeLineComponent final : public Component
{
private:
	uint32_t m_NextEventID = 1; // 次に割り当てるイベントID
	float m_CurrentTime = 0.0f; // 現在のタイムライン時間
	std::vector<TimePointEvent> m_PointEvents; // タイムラインイベントのリスト
	std::vector<TimeRangeEvent> m_RangeEvents; // 一定時間継続するイベントのリスト
	std::vector<TimeContinuousEvent> m_ContinuousEvents;	// 無期限で継続するイベントのリスト
	std::unordered_map<Component*, std::vector<uint32_t>> m_ComponentEventMap; // コンポーネントごとのイベントインデックスマップ

	void UpdatePointEvents();
	void UpdateRangeEvents();
	void UpdateContinuousEvents();

	void CleanUpPointEvents();
	void CleanUpRangeEvents();
	void CleanUpContinuousEvents();
	
	void AllStopEvents();

public:
	TimeLineComponent(GameObject& obj);
	~TimeLineComponent();
	void Update() override;
	
	uint32_t AddPointEvent(const float time, Component* owner, std::function<void()> action);
	uint32_t AddRangeEvent(
		const float startTime, const float endTime, Component* owner,
		std::function<void(float)> onUpdate = nullptr,
		std::function<void()> onStart = nullptr,
		std::function<void()> onEnd = nullptr);
	uint32_t AddContinuousEvent(const float startTime, Component* owner, std::function<void()> action);

	uint32_t AddPointDelayEvent(const float delayTime, Component* owner, std::function<void()> action);
	uint32_t AddRangeDelayEvent(const float startTime, const float endTime, const float delayTime, Component* owner,
		std::function<void(float)> onUpdate = nullptr,
		std::function<void()> onStart = nullptr,
		std::function<void()> onEnd = nullptr);
	uint32_t AddContinuousDelayEvent(const float delayTime, Component* owner, std::function<void()> action);
	void RemoveEventsByComponent(Component* owner);

	void StopEvent(uint32_t eventID);
	uint32_t GenerateEventID() { return m_NextEventID++; };
};
