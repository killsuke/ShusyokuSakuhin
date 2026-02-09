#include "TimeManager.h"
#include <iostream>

void TimeManager::Init() {
	// 周波数を取得（秒に直すためのスケール）
	QueryPerformanceFrequency(&m_Frequency);

	// 最初の時刻を取得して prev に記録
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	m_PrevTime = static_cast<double>(counter.QuadPart) / m_Frequency.QuadPart;
}

void TimeManager::UnInit() {

}

void TimeManager::Update() {
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	// 時刻を取得して更新
	const double currentTime = static_cast<double>(counter.QuadPart) / m_Frequency.QuadPart;
	m_DeltaTime = static_cast<float>(currentTime - m_PrevTime);
	m_PrevTime = currentTime;
}

void TimeManager::Reset() {
	// 最初の時刻を取得して prev に記録
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	m_PrevTime = static_cast<double>(counter.QuadPart) / m_Frequency.QuadPart;
	m_DeltaTime = 0.0f;
}