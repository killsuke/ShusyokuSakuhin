#include "DebugSystem.h"
#include "Manager/SceneManager.h"
#include "Manager/GameObjectManager.h"
#include "Manager/HitStopManager.h"
#include "Components/TransformComponent.h"
#include "Components/Render3DComponent.h"
#include "Components/Render3DColliderAABBComponent.h"
#include "Components/Render3DColliderOBBComponent.h"
#include "Mesh/SquareMesh.h"
#include "System/DirectXRender.h"
#include "Input/Input.h"
#include <iostream>

using namespace DirectX;

namespace {

	const XMFLOAT3 DEBUG_UI_SCALE = XMFLOAT3(60.0f, 30.0f, 1.0f);
	const XMFLOAT3 START_UI_POSITION = XMFLOAT3(-300.0f, 330.0f, -1.0f);
	const XMFLOAT3 DEBUG_CAMERA_POSITION = XMFLOAT3(0.0f, 0.0f, -100.0f);
	const XMFLOAT3 DEBUG_CAMERA_TARGET = XMFLOAT3(0.0f, 0.0f, 0.0f);
	const XMFLOAT4 WEAK_REDCOLOR = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
	const XMFLOAT4 WHITECOLOR = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	constexpr float SHIFT_UI = 120.0f;
}

// 初期化
void DebugSystem::Init() {
#if _DEBUG

	m_DebugUITextureNames[0] = "debugUI_F1.png";
	m_DebugUITextureNames[1] = "debugUI_F2.png";
	m_DebugUITextureNames[2] = "debugUI_F3.png";
	m_DebugUITextureNames[3] = "debugUI_F4.png";
	m_DebugUITextureNames[4] = "debugUI_F5.png";

	XMFLOAT3 uiPosition = START_UI_POSITION;
	std::string debugUIName = "DebugUIObject";

	// デバッグUIの生成
	for (int i = 0; i < UI_DISPLAY_COUNT; ++i) {
		m_DebugUIs[i] = nullptr;

		// デバッグUIの名前を生成
		debugUIName += std::to_string(i + 1);
		m_DebugUIs[i] = GameObjectManager::AddUI(debugUIName, "DebugUI");

		// デバッグUIはシーンを跨いで持ち越す
		m_DebugUIs[i]->SetCarryOverFlag(true);

		TransformComponent* dbTransform = m_DebugUIs[i]->AddComponent<TransformComponent>();
		dbTransform->SetPosition(uiPosition);
		dbTransform->SetScale(DEBUG_UI_SCALE);

		Render3DComponent* dbRender = m_DebugUIs[i]->AddComponent<Render3DComponent>();
		dbRender->CreateMesh<SquareMesh>();
		dbRender->SetShader("unlitTextureVS2D.hlsl", "unlitTexturePS.hlsl");
		dbRender->ChangeTexture(m_DebugUITextureNames[i]);

		uiPosition.x += SHIFT_UI;
	}

	// デバッグカメラの生成
	m_DebugCamera = GameObjectManager::AddObject("DebugCamera", "DebugCamera");
	TransformComponent* camTransform = m_DebugCamera->AddComponent<TransformComponent>();
	camTransform->SetPosition(DEBUG_CAMERA_POSITION);
	DebugCameraComponent* debugCamera = m_DebugCamera->AddComponent<DebugCameraComponent>();
	debugCamera->SetTarget(DEBUG_CAMERA_TARGET);
	debugCamera->SetFieldOfView(FieldOfView::DEFAULT);
	m_DebugCamera->SetCarryOverFlag(true);
	m_DebugCamera->SetActiveState(ActiveState::ALL_STOP);

#endif
}

// 終了処理
void DebugSystem::UnInit() {

	// デバッグUIの破棄
	for (GameObject*& ui : m_DebugUIs) {
		ui = nullptr;
	}

	m_DebugCamera = nullptr;
}

void DebugSystem::Update() {

#ifdef  _DEBUG

	// デバッグUIのオブジェクトが生成されているか確認
	for (const GameObject* debugUI : m_DebugUIs) {
		if (debugUI == nullptr) {
			return;
		}
	}

	// デバッグ用のカメラ以外のオブジェクトを取得
	const std::vector<GameObject*> objs = GameObjectManager::GameObjectFindAllTagsOtherThan("DebugCamera", "DebugUI");

	// デバッグ機能の処理
	DebugUI();
	ScreenStopped(objs);
	FrameAdvance(objs);
	SwitchingFillMode();
	SwitchingVisualization();
	SettingFieldOfView();

#endif //  _DEBUG
}

// デバッグUIの表示非表示の切り替え
void DebugSystem::DebugUI() {

	const bool vk_F1_Trigger = Input::GetKeyTrigger(VK_F1);

	if (vk_F1_Trigger) {
		m_IsDebugUI = !m_IsDebugUI;

		// デバッグUIの表示非表示
		ActiveState debugUIState = m_IsDebugUI ? ActiveState::ACTIVE : ActiveState::DRAW_STOP;

		for (GameObject* debugUI : m_DebugUIs) {

			debugUI->SetActiveState(debugUIState);
		}
	}
}

// 画面停止の切り替え
void DebugSystem::ScreenStopped(const std::vector<GameObject*>& objs) {

	if (objs.empty()) {
		return;
	}

	GameObject* sceneCamera = GameObjectManager::GameObjectFindName("camera");

	if (sceneCamera == nullptr || m_DebugCamera == nullptr) {
		return;
	}

	CameraComponent* sceneCamComp = sceneCamera->GetComponent<CameraComponent>();
	DebugCameraComponent* debugCamComp = m_DebugCamera->GetComponent<DebugCameraComponent>();
	TransformComponent* sceneCamTrans = sceneCamera->GetComponent<TransformComponent>();
	TransformComponent* debugCamTrans = m_DebugCamera->GetComponent<TransformComponent>();
	Render3DComponent* sceneCameraRend = sceneCamera->GetComponent<Render3DComponent>();

	if (sceneCamComp == nullptr || debugCamComp == nullptr || sceneCamTrans == nullptr || debugCamTrans == nullptr || sceneCameraRend == nullptr) {
		return;
	}

	const bool vk_F2_Trigger = Input::GetKeyTrigger(VK_F2);

	// 停止と起動を入れ替える
	if (vk_F2_Trigger) {

		m_IsScreenStop = !m_IsScreenStop;

		const XMFLOAT3 sceneCamPos = sceneCamTrans->GetPosition();
		const XMFLOAT3 sceneCamTarget = sceneCamComp->GetTarget();

		// シーン内のカメラの位置を入れる
		debugCamTrans->SetPosition(sceneCamPos);
		debugCamComp->SetTarget(sceneCamTarget);

		// 画面停止解除時の処理
		if (m_IsScreenStop == false) {

			sceneCamera->SetActiveState(ActiveState::ACTIVE);
			m_DebugCamera->SetActiveState(ActiveState::ALL_STOP);
			sceneCameraRend->SetActiveFlag(false);

			// 描画処理停止と全て停止以外のオブジェクトをアクティブにする
			for (GameObject* obj : objs) {

				// 描画停止中か全停止中のオブジェクトはスルー
				const ActiveState currentState = obj->GetActiveState();
				if (currentState == ActiveState::DRAW_STOP || currentState == ActiveState::ALL_STOP) {
					continue;
				}

				obj->SetActiveState(ActiveState::ACTIVE);
			}

			Scene* scene = SceneManager::GetScene();

			if (scene != nullptr) {

				scene->SetSceneActive(true);
			}
			return;
		}

	}

	XMFLOAT4 debugColor = WHITECOLOR;

	// 画面停止中の処理
	if (m_IsScreenStop == true) {

		sceneCamera->SetActiveState(ActiveState::ALL_STOP);
		m_DebugCamera->SetActiveState(ActiveState::ACTIVE);
		sceneCameraRend->SetActiveFlag(true);

		for (GameObject* obj : objs) {

			// 更新停止中か全停止中のオブジェクトはスルー
			const ActiveState currentState = obj->GetActiveState();
			if (currentState == ActiveState::ACTIVE) {
				obj->SetActiveState(ActiveState::UPDATE_STOP);
			}
		}

		Scene* scene = SceneManager::GetScene();
		if (scene != nullptr) {
			scene->SetSceneActive(false);
		}

		debugColor = WEAK_REDCOLOR;
	}

	// デバッグUIの色変更
	Render3DComponent* dbRender = m_DebugUIs[1]->GetComponent<Render3DComponent>();

	if (dbRender != nullptr) {
		dbRender->SetColor(debugColor);
	}
}

// コマ送りの処理
void DebugSystem::FrameAdvance(const std::vector<GameObject*>& objs) {

	if (objs.empty()) {
		return;
	}

	const bool vk_F3_Trigger = Input::GetKeyTrigger(VK_F3);

	// コマ送り
	if (vk_F3_Trigger && m_IsScreenStop == true) {

		for (GameObject* obj : objs) {

			// 描画停止中か全停止中のオブジェクトはスルー
			const ActiveState currentState = obj->GetActiveState();
			if (currentState == ActiveState::DRAW_STOP || currentState == ActiveState::ALL_STOP) {
				continue;
			}

			obj->SetActiveState(ActiveState::ACTIVE);
			obj->Update();
			obj->SetActiveState(ActiveState::UPDATE_STOP);
		}

		HitStopManager::Update();

		Scene* scene = SceneManager::GetScene();
		if (scene != nullptr) {
			scene->Update();
		}
	}
}

// ワイヤーフレームとソリッドの切り替え
void DebugSystem::SwitchingFillMode() {

	const bool vk_F4_Trigger = Input::GetKeyTrigger(VK_F4);

	if (vk_F4_Trigger) {
		DirectXRender::SwitchingFillMode();
	}
}

// コライダーの可視化の切り替え
void DebugSystem::SwitchingVisualization() {

	XMFLOAT4 debugColor = WHITECOLOR;
	const bool vk_F5_Trigger = Input::GetKeyTrigger(VK_F5);

	if (vk_F5_Trigger) {

		// カメラのターゲットにしているオブジェクトを取得
		for (const uint32_t& objID : m_ColliderObjects) {
			GameObject* obj = GameObjectManager::GameObjectFindInstanceID(objID);
			if (obj != nullptr) {

				if (m_IsVisualization == true) {
					Render3DColliderOBBComponent* renderComp = obj->GetComponent<Render3DColliderOBBComponent>();
					if (renderComp != nullptr) {
						renderComp->SetActiveFlag(false);
					}
					Render3DColliderAABBComponent* renderComp2 = obj->GetComponent<Render3DColliderAABBComponent>();
					if (renderComp2 != nullptr) {
						renderComp2->SetActiveFlag(false);
					}

					debugColor = WEAK_REDCOLOR;
				}
				else {
					Render3DColliderOBBComponent* renderComp = obj->GetComponent<Render3DColliderOBBComponent>();
					if (renderComp != nullptr) {
						renderComp->SetActiveFlag(true);
					}
					Render3DColliderAABBComponent* renderComp2 = obj->GetComponent<Render3DColliderAABBComponent>();
					if (renderComp2 != nullptr) {
						renderComp2->SetActiveFlag(true);
					}
				}
			}
		}

		// カメラのターゲットのオブジェクトも可視化の対象にする
		for (const uint32_t& objID : m_CameraTarget) {
			GameObject* obj = GameObjectManager::GameObjectFindInstanceID(objID);

			if (obj != nullptr) {

				if (m_IsVisualization == true) {
					Render3DComponent* renderComp = obj->GetComponent<Render3DComponent>();
					if (renderComp != nullptr) {
						renderComp->SetActiveFlag(false);
					}
				}
				else {
					Render3DComponent* renderComp = obj->GetComponent<Render3DComponent>();
					if (renderComp != nullptr) {
						renderComp->SetActiveFlag(true);
					}
				}
			}
		}

		m_IsVisualization = !m_IsVisualization;

		// デバッグUIの色変更
		Render3DComponent* dbRender = m_DebugUIs[4]->GetComponent<Render3DComponent>();

		if (dbRender != nullptr) {
			dbRender->SetColor(debugColor);
		}
	}
}

// デバッグ用カメラの視野角の切り替え
void DebugSystem::ChangeFieldOfView() {

	const bool vk_F6_Trigger = Input::GetKeyTrigger(VK_F6);

	if (vk_F6_Trigger && m_DebugCamera != nullptr) {
		DebugCameraComponent* debugCamComp = m_DebugCamera->GetComponent<DebugCameraComponent>();
		if (debugCamComp != nullptr) {

			const FieldOfView currentFOV = debugCamComp->GetFieldOfView();

			switch (currentFOV)
			{
			case FieldOfView::DEFAULT:
				debugCamComp->SetFieldOfView(FieldOfView::WIDE);
				break;
			case FieldOfView::WIDE:
				debugCamComp->SetFieldOfView(FieldOfView::TELEPHOTO);
				break;
			case FieldOfView::TELEPHOTO:
				debugCamComp->SetFieldOfView(FieldOfView::DEFAULT);
				break;
			default:
				break;
			}
		}
	}
}

// デバッグ用カメラの視野角の設定
void DebugSystem::SettingFieldOfView() {

	if (m_DebugCamera == nullptr) {
		return;
	}

	DebugCameraComponent* debugCamComp = m_DebugCamera->GetComponent<DebugCameraComponent>();

	if (debugCamComp == nullptr) {

		return;
	}

	if (m_IsScreenStop == false) {

		debugCamComp->SetFieldOfView(FieldOfView::DEFAULT);
	}
	else {

		ChangeFieldOfView();
	}
}

// コライダーオブジェクトの追加
void DebugSystem::AddColliderObjects(const uint32_t& obj) {

	// すでに登録されているオブジェクトはスルー
	std::vector<uint32_t>::iterator it = std::find(m_ColliderObjects.begin(), m_ColliderObjects.end(), obj);
	if (it != m_ColliderObjects.end()) {
		return;
	}
	m_ColliderObjects.push_back(obj);
}

// コライダーオブジェクトの削除
void DebugSystem::RemoveColliderObjects(const uint32_t& obj) {

	std::vector<uint32_t>::iterator it = std::find(m_ColliderObjects.begin(), m_ColliderObjects.end(), obj);
	if (it != m_ColliderObjects.end()) {
		m_ColliderObjects.erase(it);
	}
}

// コライダーオブジェクトのクリア
void DebugSystem::ClearColliderObjects() {
	m_ColliderObjects.clear();
}

// カメラターゲットの追加
void DebugSystem::AddCameraTarget(const uint32_t& obj) {

	// すでに登録されているオブジェクトはスルー
	std::vector<uint32_t>::iterator it = std::find(m_CameraTarget.begin(), m_CameraTarget.end(), obj);
	if (it != m_CameraTarget.end()) {
		return;
	}
	m_CameraTarget.push_back(obj);
}

// カメラターゲットの削除
void DebugSystem::RemoveCameraTarget(const uint32_t& obj) {

	std::vector<uint32_t>::iterator it = std::find(m_CameraTarget.begin(), m_CameraTarget.end(), obj);
	if (it != m_CameraTarget.end()) {
		m_CameraTarget.erase(it);
	}
}

// カメラターゲットのクリア
void DebugSystem::ClearCameraTarget() {
	m_CameraTarget.clear();
}

// 割り当てるキー
// F1キーでデバッグ機能のUIのON/OFF
// F2キーでオブジェクトの更新停止のON/OFF
// F3キーで停止中のコマ送り
// F4キーでワイヤーフレームとソリッドの切り替え