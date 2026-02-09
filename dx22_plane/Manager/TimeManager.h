#pragma once
#include <chrono>
#include <windows.h>

class TimeManager final
{
private:
	static inline LARGE_INTEGER m_Frequency = {};	// タイマー値に使用する周波数
	static inline float m_DeltaTime = 0.0f;	// 時間の差分
	static inline double m_PrevTime = 0.0;	// 前回の時間

	// コンストラクタ・デストラクタを削除
	TimeManager() = delete;
	~TimeManager() = delete;

	// コピー・ムーブも削除
	TimeManager(const TimeManager&) = delete;
	TimeManager(TimeManager&&) = delete;
	TimeManager& operator=(const TimeManager&) = delete;
	TimeManager& operator=(TimeManager&&) = delete;

public:
	
	static void Init();
	static void UnInit();
	static void Update();
	static void Reset();

	static float GetDeltaTime() { return m_DeltaTime; };
};
