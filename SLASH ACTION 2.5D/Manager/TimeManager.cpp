#include "TimeManager.h"
#include "System/DirectXRender.h"
#include <iostream>

using namespace DirectX;

namespace {
	constexpr double FIXED_DT = (1.0 / 60.0);			// 固定更新のデルタタイム（60FPS）
	constexpr double MAX_DELTA_TIME = 0.05;				// 巨大フレームを防止するための最大デルタタイム（50ms）
	constexpr double RESTRICTION_DELTA_TIME = 0.032;	// 更新時間の制限（32ms、約30FPS）
}

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
	double frameTime = currentTime - m_PrevTime;
	m_PrevTime = currentTime;

	// タイマーストップ中はフレームタイムをゼロにする
	if (m_IsActive == false) {
		frameTime = 0.0;
	}

	// 巨大フレーム防止
	if (frameTime > MAX_DELTA_TIME) {
		frameTime = MAX_DELTA_TIME;
	}

	m_DeltaTime = static_cast<float>(frameTime);

	m_Accumulator += frameTime;

	// 更新時間を制御するために溢れた分は削る
	if (m_Accumulator > RESTRICTION_DELTA_TIME) {
		m_Accumulator = RESTRICTION_DELTA_TIME;
	}
}

void TimeManager::Reset() {
	// 最初の時刻を取得して prev に記録
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	m_PrevTime = static_cast<double>(counter.QuadPart) / m_Frequency.QuadPart;
	m_DeltaTime = 0.0f;
}

// 固定更新が必要かどうかを判定
bool TimeManager::ShouldFixedUpdate() {

	ID3D11Buffer* timeBuffer = DirectXRender::GetTimeBuffer();
	ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();
	TimeBuffer timeData = {};
	bool updated = false;

	// アキュムレータが固定更新の時間を超えているかどうかをチェック
	// おさまっている時間の分だけ実行、余った分によるズレを防止

	const float deltaTime = static_cast<float>(FIXED_DT);
	if (m_Accumulator >= FIXED_DT)
	{
		m_Accumulator -= FIXED_DT;
		m_TotalTime += FIXED_DT;

		// 固定更新の時間をシェーダーに渡す
		timeData.deltaTime = deltaTime;
		timeData.totalTime = static_cast<float>(m_TotalTime);
		deviceContext->UpdateSubresource(timeBuffer, 0, NULL, &timeData, 0, 0);

		updated = true;
	}

	return updated;
}

// 固定更新のデルタタイムを取得
float TimeManager::GetFixedDeltaTime()
{
	const float fixedDeltaTime = static_cast<float>(FIXED_DT);
	return fixedDeltaTime;
}
