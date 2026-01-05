#include "TimeManager.h"
#include <iostream>

LARGE_INTEGER TimeManager::m_frequency = {};
float TimeManager::m_deltaTime = 0.0f;
double TimeManager::m_prevTime = 0.0;

void TimeManager::Init() {
	// 周波数を取得（秒に直すためのスケール）
	QueryPerformanceFrequency(&m_frequency);

	// 最初の時刻を取得して prev に記録
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	m_prevTime = static_cast<double>(counter.QuadPart) / m_frequency.QuadPart;
}

void TimeManager::UnInit() {

}

void TimeManager::Update() {
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	const double currentTime = static_cast<double>(counter.QuadPart) / m_frequency.QuadPart;
	m_deltaTime = static_cast<float>(currentTime - m_prevTime);
	m_prevTime = currentTime;
}
