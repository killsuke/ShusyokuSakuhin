#include "TimeLineComponent.h"
#include "Manager/TimeManager.h"

TimeLineComponent::TimeLineComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TIME_LINE"); // ソート番号を設定

	m_PointEvents.clear();	// イベントリストを初期化
}

TimeLineComponent::~TimeLineComponent() {
	m_PointEvents.clear();	// イベントリストをクリア
}

void TimeLineComponent::Update() {

	const float deltaTime = TimeManager::GetDeltaTime();
	m_CurrentTime += deltaTime;

	UpdatePointEvents();	// 一度きりのイベントの更新
	UpdateRangeEvents();	// 範囲イベントの更新
}

// 一度きりのイベントの更新
void TimeLineComponent::UpdatePointEvents() {

	// 次のイベントが存在し、かつ現在の時間がそのイベントのトリガー時間を超えている場合
	while (m_NextEventIndex < m_PointEvents.size()) {

		if (m_PointEvents[m_NextEventIndex].valid == false) {

			m_NextEventIndex++;
			continue; // 無効なイベントはスキップ
		}

		if (m_CurrentTime < m_PointEvents[m_NextEventIndex].triggerTime)
		{
			break; // 次のイベントの時間に達していない場合はループを抜ける
		}

		// イベントを実行
		m_PointEvents[m_NextEventIndex].eventAction();
		// 次のイベントへ進む
		m_NextEventIndex++;
	}
}

// 範囲イベントの更新
void TimeLineComponent::UpdateRangeEvents() {

	for (auto& e : m_RangeEvents) {

		// 開始処理
		if (e.started == false && m_CurrentTime >= e.startTime) {

			e.started = true;
			if (e.onStart) {
				e.onStart();
			}
		}

		// 中間処理
		if (e.started == true && e.ended == false) {

			float t = (m_CurrentTime - e.startTime) / (e.endTime - e.startTime);
			t = std::clamp(t, 0.0f, 1.0f);
			if (e.onUpdate) {
				e.onUpdate(t);
			}
		}

		// 終了処理
		if (e.ended == false && m_CurrentTime >= e.endTime) {
			e.ended = true;
			if (e.onEnd) {
				e.onEnd();
			}
		}
	}
}

// 一度きりのイベントを追加
uint32_t TimeLineComponent::AddPointEvent(const float time, Component* owner, std::function<void()> action) {
	
	TimePointEvent newEvent;
	newEvent.eventID = GenerateEventID();
	newEvent.triggerTime = time;
	newEvent.ownerComponent = owner;
	newEvent.eventAction = action;
	m_PointEvents.push_back(newEvent);	// イベントを追加

	const size_t index = m_PointEvents.size() - 1;	// 追加したイベントのインデックスを取得
	m_ComponentEventMap[owner].push_back(index); // コンポーネントごとのマップにインデックスを追加

	// イベントをトリガー時間でソート
	std::sort(m_PointEvents.begin(), m_PointEvents.end(),
		[](const TimePointEvent& a, const TimePointEvent& b) {
			return a.triggerTime < b.triggerTime;
		});

	return newEvent.eventID;
}

// 範囲イベントを追加
uint32_t TimeLineComponent::AddRangeEvent(
	const float startTime, const float endTime, Component* owner,
	std::function<void(float)> onUpdate,
	std::function<void()> onStart,
	std::function<void()> onEnd) {

	TimeRangeEvent newEvent;
	newEvent.eventID = GenerateEventID();
	newEvent.startTime = startTime;
	newEvent.endTime = endTime;
	newEvent.onUpdate = onUpdate;
	newEvent.onStart = onStart;
	newEvent.onEnd = onEnd;
	newEvent.ownerComponent = owner;
	m_RangeEvents.push_back(newEvent);	// イベントを追加

	const size_t index = m_RangeEvents.size() - 1;	// 追加したイベントのインデックスを取得
	m_ComponentEventMap[owner].push_back(index); // コンポーネントごとのマップにインデックスを追加

	return newEvent.eventID;
}

// 発行してから〇秒後に一度だけ実行されるイベントを追加
uint32_t TimeLineComponent::AddPointDelayEvent(const float delayTime, Component* owner, std::function<void()> action) {

	TimePointEvent newEvent;
	newEvent.eventID = GenerateEventID();
	newEvent.triggerTime = m_CurrentTime + delayTime;
	newEvent.ownerComponent = owner;
	newEvent.eventAction = action;
	m_PointEvents.push_back(newEvent);	// イベントを追加

	const size_t index = m_PointEvents.size() - 1;	// 追加したイベントのインデックスを取得
	m_ComponentEventMap[owner].push_back(index); // コンポーネントごとのマップにインデックスを追加

	// イベントをトリガー時間でソート
	std::sort(m_PointEvents.begin(), m_PointEvents.end(),
		[](const TimePointEvent& a, const TimePointEvent& b) {
			return a.triggerTime < b.triggerTime;
		});

	return newEvent.eventID;
}

uint32_t TimeLineComponent::AddRangeDelayEvent(const float startTime, const float endTime, const float delayTime, Component* owner,
	std::function<void(float)> onUpdate,
	std::function<void()> onStart,
	std::function<void()> onEnd) {

	TimeRangeEvent newEvent;
	newEvent.eventID = GenerateEventID();
	newEvent.startTime = startTime + delayTime;
	newEvent.endTime = endTime + delayTime;
	newEvent.onUpdate = onUpdate;
	newEvent.onStart = onStart;
	newEvent.onEnd = onEnd;
	newEvent.ownerComponent = owner;
	m_RangeEvents.push_back(newEvent);	// イベントを追加

	const size_t index = m_RangeEvents.size() - 1;	// 追加したイベントのインデックスを取得
	m_ComponentEventMap[owner].push_back(index); // コンポーネントごとのマップにインデックスを追加

	return newEvent.eventID;
}

// 指定したコンポーネントに関連するすべてのイベントを削除
void TimeLineComponent::RemoveEventsByComponent(Component* owner) {

	auto it = m_ComponentEventMap.find(owner);
	if (it == m_ComponentEventMap.end()) {
		return;
	}

	for (auto index : it->second) {
		m_PointEvents[index].valid = false; // 空の関数に置き換え
	}

	for(auto index : it->second) {
		m_RangeEvents[index].valid = false; // 空の関数に置き換え
	}

	m_ComponentEventMap.erase(it); // マップからエントリを削除
}

// 指定したイベントIDのイベントを停止
void TimeLineComponent::StopEvent(uint32_t eventID) {

	for (auto& e : m_PointEvents) {
		if (e.eventID == eventID) {
			e.valid = false; // イベントを無効化
			return;
		}
	}
	for (auto& e : m_RangeEvents) {
		if (e.eventID == eventID) {
			e.valid = false; // イベントを無効化
			return;
		}
	}
}