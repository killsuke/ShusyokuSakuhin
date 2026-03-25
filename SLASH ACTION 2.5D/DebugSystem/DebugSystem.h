#pragma once
#include "GameObject/GameObject.h"
#include "Components/DebugCameraComponent.h"
#include <vector>
#include <array>
#include <string>

namespace {

	constexpr unsigned int UI_DISPLAY_COUNT = 4;
}

class DebugSystem final
{
private:

	static inline std::array<GameObject*, UI_DISPLAY_COUNT> m_DebugUIs = { nullptr };
	static inline std::array<std::string, UI_DISPLAY_COUNT> m_DebugUITextureNames = { "" };
	static inline GameObject* m_DebugCamera = nullptr;
	static inline bool m_IsDebugUI = true; // デバッグUIの表示非表示フラグ
	static inline bool m_ScreenStop = false; // 画面停止のフラグ
	static inline FieldOfView m_CurrentFieldOfView = FieldOfView::DEFAULT;

	DebugSystem() = default;
	~DebugSystem() = default;

	// コピー・ムーブも削除
	DebugSystem(const DebugSystem&) = delete;
	DebugSystem(DebugSystem&&) = delete;
	DebugSystem& operator=(const DebugSystem&) = delete;
	DebugSystem& operator=(DebugSystem&&) = delete;

	// デバッグ機能のUIのON/OFF
	static void DebugUI();

	// オブジェクトの更新停止のON/OFF
	static void ScreenStopped(const std::vector<GameObject*>& objs);

	// 停止中のコマ送り
	static void FrameAdvance(const std::vector<GameObject*>& objs);

	// ワイヤーフレームとソリッドの切り替え
	static void SwitchingFillMode();

public:

	static void Init();
	static void UnInit();

	static void Update();
};