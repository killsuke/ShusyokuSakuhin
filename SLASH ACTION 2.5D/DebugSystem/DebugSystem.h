#pragma once
#include "GameObject/GameObject.h"
#include "Components/DebugCameraComponent.h"
#include <vector>
#include <array>
#include <string>

namespace {

	constexpr unsigned int UI_DISPLAY_COUNT = 5;
}

class DebugSystem final
{
private:

	static inline std::array<GameObject*, UI_DISPLAY_COUNT> m_DebugUIs = { nullptr };			// デバッグUIのGameObjectの配列
	static inline std::array<std::string, UI_DISPLAY_COUNT> m_DebugUITextureNames = { "" };		// デバッグUIのテクスチャ名の配列
	static inline GameObject* m_DebugCamera = nullptr;											// デバッグカメラのGameObject
	static inline bool m_IsDebugUI = true;														// デバッグUIの表示非表示フラグ
	static inline bool m_IsScreenStop = false;													// 画面停止のフラグ
	static inline bool m_IsVisualization = true;												// コライダーの可視化フラグ
	static inline FieldOfView m_CurrentFieldOfView = FieldOfView::DEFAULT;						// 現在の視野角
	static inline std::vector<uint32_t> m_ColliderObjects = {};									// コライダーオブジェクトのID
	static inline std::vector<uint32_t> m_CameraTarget = {};									// カメラターゲットのID

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

	// コライダーの可視化のON/OFF
	static void SwitchingVisualization();

	// デバッグ用カメラの視野角の切り替え
	static void ChangeFieldOfView();

	static void SettingFieldOfView();

public:

	static void Init();
	static void UnInit();

	static void Update();

	static void AddColliderObjects(const uint32_t& obj);
	static void RemoveColliderObjects(const uint32_t& obj);
	static void ClearColliderObjects();

	static void AddCameraTarget(const uint32_t& obj);
	static void RemoveCameraTarget(const uint32_t& obj);
	static void ClearCameraTarget();
};