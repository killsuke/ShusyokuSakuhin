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

	const float deltaTime = TimeManager::GetFixedDeltaTime();

	if (deltaTime <= 0.0f) {
		return;
	}

	m_CurrentTime += deltaTime;

	UpdatePointEvents();	// 一度きりのイベントの更新
	UpdateRangeEvents();	// 範囲イベントの更新
	UpdateContinuousEvents();	// 動き続けるイベントの更新
}

// イベント更新系の関数 =============================================

// 一度きりのイベントの更新
void TimeLineComponent::UpdatePointEvents() {

	const float now = m_CurrentTime;

	// 次のイベントが存在し、かつ現在の時間がそのイベントのトリガー時間を超えている場合
	while (m_NextEventIndex < m_PointEvents.size()) {

		TimePointEvent e = m_PointEvents[m_NextEventIndex];

		if (e.valid == false) {

			m_NextEventIndex++;
			continue; // 無効なイベントはスキップ
		}

		if (now >= e.triggerTime) {
			// イベントを実行
			e.eventAction();
			// 次のイベントへ進む
			m_NextEventIndex++;
		}

		if (m_CurrentTime < e.triggerTime)
		{
			break; // 次のイベントの時間に達していない場合はループを抜ける
		}
	}
}

// 範囲イベントの更新
void TimeLineComponent::UpdateRangeEvents() {

	const float now = m_CurrentTime;

	for (TimeRangeEvent& e : m_RangeEvents) {

		if (e.valid == false) {
			continue; // 無効なイベントはスキップ
		}

		// 開始処理
		if (e.started == false && now >= e.startTime) {

			e.started = true;
			if (e.onStart) {
				e.onStart();
			}
		}

		// 中間処理
		if (e.started == true && e.ended == false) {

			float t = (now - e.startTime) / (e.endTime - e.startTime);
			t = std::clamp(t, 0.0f, 1.0f);
			if (e.onUpdate) {

				e.onUpdate(t);
			}
		}

		// 終了処理
		if (e.ended == false && now >= e.endTime) {
			e.ended = true;
			if (e.onEnd) {
				e.onEnd();
			}
		}
	}
}

// 永続的に行うイベント
void TimeLineComponent::UpdateContinuousEvents() {

	const float now = m_CurrentTime;

	for (TimeContinuousEvent& e : m_ContinuousEvents) {

		if (e.valid == false) {
			continue;
		}

		if (now >= e.triggerTime) {
			e.eventAction();
		}
	}
}

// ===========================================================================

// イベント登録系の関数 ======================================================

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

uint32_t TimeLineComponent::AddContinuousEvent(const float startTime, Component* owner, std::function<void()> action) {

	TimeContinuousEvent newEvent;
	newEvent.eventID = GenerateEventID();
	newEvent.triggerTime = startTime;
	newEvent.eventAction = action;
	newEvent.ownerComponent = owner;
	m_ContinuousEvents.push_back(newEvent);	// イベントを追加

	const size_t index = m_ContinuousEvents.size() - 1;	// 追加したイベントのインデックスを取得
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
	newEvent.startTime = m_CurrentTime + startTime + delayTime;
	newEvent.endTime = m_CurrentTime + endTime + delayTime;
	newEvent.onUpdate = onUpdate;
	newEvent.onStart = onStart;
	newEvent.onEnd = onEnd;
	newEvent.ownerComponent = owner;
	m_RangeEvents.push_back(newEvent);	// イベントを追加

	const size_t index = m_RangeEvents.size() - 1;	// 追加したイベントのインデックスを取得
	m_ComponentEventMap[owner].push_back(index); // コンポーネントごとのマップにインデックスを追加

	return newEvent.eventID;
}

uint32_t TimeLineComponent::AddContinuousDelayEvent(const float delayTime, Component* owner, std::function<void()> action) {

	TimeContinuousEvent newEvent;
	newEvent.eventID = GenerateEventID();
	newEvent.triggerTime = m_CurrentTime + delayTime;
	newEvent.eventAction = action;
	newEvent.ownerComponent = owner;
	m_ContinuousEvents.push_back(newEvent);

	const size_t index = m_ContinuousEvents.size() - 1;	// 追加したイベントのインデックスを取得
	m_ComponentEventMap[owner].push_back(index); // コンポーネントごとのマップにインデックスを追加

	return newEvent.eventID;
}

// =======================================================================================

// 指定したコンポーネントに関連するすべてのイベントを削除
void TimeLineComponent::RemoveEventsByComponent(Component* owner) {

    std::unordered_map<Component*, std::vector<size_t>>::iterator it = m_ComponentEventMap.find(owner);
    if (it == m_ComponentEventMap.end()) {
        return;
    }

    for (size_t index : it->second) {
        m_PointEvents[index].valid = false; // 空の関数に置き換え
    }

    for (size_t index : it->second) {
        m_RangeEvents[index].valid = false; // 空の関数に置き換え
    }

    m_ComponentEventMap.erase(it); // マップからエントリを削除
}

// 指定したイベントIDのイベントを停止
void TimeLineComponent::StopEvent(uint32_t eventID) {

	for (TimePointEvent& e : m_PointEvents) {
		if (e.eventID == eventID) {
			e.valid = false; // イベントを無効化
			return;
		}
	}
	for (TimeRangeEvent& e : m_RangeEvents) {
		if (e.eventID == eventID) {
			e.valid = false; // イベントを無効化
			return;
		}
	}
	for (TimeContinuousEvent& e : m_ContinuousEvents) {
		if (e.eventID == eventID) {
			e.valid = false; // イベントを無効化
			return;
		}
	}
}