#pragma once
#include <chrono>
#include <windows.h>

class TimeManager
{
private:
	static LARGE_INTEGER m_frequency;
	static float m_deltaTime;	// 時間の差分
	static double m_prevTime;	// 前回の時間

public:
	// コンストラクタ・デストラクタを削除
	TimeManager() = delete;
	~TimeManager() = delete;

	// コピー・ムーブも削除
	TimeManager(const TimeManager&) = delete;
	TimeManager(TimeManager&&) = delete;
	TimeManager& operator=(const TimeManager&) = delete;
	TimeManager& operator=(TimeManager&&) = delete;

	static void Init();
	static void UnInit();
	static void Update();

	static float GetDeltaTime() { return m_deltaTime; };
};
