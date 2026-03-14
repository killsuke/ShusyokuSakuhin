#include "DebugSystem.h"
#include "Manager/SceneManager.h"
#include "Manager/GameObjectManager.h"
#include "Manager/HitStopManager.h"
#include "Components/TransformComponent.h"
#include "Components/Render3DComponent.h"
#include "Mesh/SquareMesh.h"
#include "System/DirectXRender.h"
#include "Input/Input.h"
#include <iostream>

using namespace DirectX;

namespace {

	const XMFLOAT3 DebugUIScale = XMFLOAT3(60.0f, 30.0f, 1.0f);
	const XMFLOAT3 StartUIPosition = XMFLOAT3(-300.0f, 330.0f, -1.0f);
	const XMFLOAT4 WeakRedColor = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
	const XMFLOAT4 WhiteColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	constexpr float ShiftUI = 120.0f;
	constexpr unsigned int UI_DisplayCount = 4;
}

void DebugSystem::Init() {
#if _DEBUG

	m_DebugUIs.resize(UI_DisplayCount, nullptr);

	m_DebugUITextureNames.resize(UI_DisplayCount);
	m_DebugUITextureNames[0] = "debugUI.png";
	m_DebugUITextureNames[1] = "debugUI_F2.png";
	m_DebugUITextureNames[2] = "debugUI_F3.png";
	m_DebugUITextureNames[3] = "debugUI_F4.png";

	XMFLOAT3 uiPosition = StartUIPosition;
	std::string debugUIName = "DebugUIObject";

	// デバッグUIの生成
	for (int i = 0; i < UI_DisplayCount; ++i) {
		m_DebugUIs[i] = nullptr;

		debugUIName += std::to_string(i + 1);
		m_DebugUIs[i] = GameObjectManager::AddUI(debugUIName, "DebugUI");
		m_DebugUIs[i]->SetCarryOverFlag(true);
		TransformComponent* dbTransform = m_DebugUIs[i]->AddComponent<TransformComponent>();
		dbTransform->SetPosition(uiPosition);
		dbTransform->SetScale(DebugUIScale);

		Render3DComponent* dbRender = m_DebugUIs[i]->AddComponent<Render3DComponent>();
		dbRender->CreateMesh<SquareMesh>();
		dbRender->SetShader("unlitTextureVS2D.hlsl", "unlitTexturePS.hlsl");
		dbRender->ChangeTexture(m_DebugUITextureNames[i]);

		uiPosition.x += ShiftUI;
	}

	m_DebugCamera = GameObjectManager::AddObject("DebugCamera", "DebugCamera");
	TransformComponent* camTransform = m_DebugCamera->AddComponent<TransformComponent>();
	camTransform->SetPosition(XMFLOAT3(0.0f, 0.0f, -100.0f));
	DebugCameraComponent* debugCamera = m_DebugCamera->AddComponent<DebugCameraComponent>();
	debugCamera->SetTarget(XMFLOAT3(0.0f, 0.0f, 0.0f));
	debugCamera->SetFieldOfView(FieldOfView::DEFAULT);
	m_DebugCamera->SetCarryOverFlag(true);
	m_DebugCamera->SetActiveState(ActiveState::ALL_STOP);

#endif
}

void DebugSystem::UnInit() {

	// デバッグUIの破棄
	m_DebugUIs.clear();
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

	// 新しいオブジェクトが出来た時にちゃんと止められるか確認
	const std::vector<GameObject*> objs = GameObjectManager::GameObjectFindAllTagsOtherThan("DebugCamera", "DebugUI");

	// デバッグ機能の処理
	DebugUI();
	ScreenStopped(objs);
	FrameAdvance(objs);
	SwitchingFillMode();

#endif //  _DEBUG
}

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

void DebugSystem::ScreenStopped(const std::vector<GameObject*>& objs) {

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

	if (vk_F2_Trigger) {
		m_ScreenStop = !m_ScreenStop;

		const XMFLOAT3 sceneCamPos = sceneCamTrans->GetPosition();
		const XMFLOAT3 sceneCamTarget = sceneCamComp->GetTarget();

		// 位置を入れる
		debugCamTrans->SetPosition(sceneCamPos);
		debugCamComp->SetTarget(sceneCamTarget);
		// 画面停止解除時の処理
		if (m_ScreenStop == false) {

			sceneCamera->SetActiveState(ActiveState::ACTIVE);
			m_DebugCamera->SetActiveState(ActiveState::ALL_STOP);


			sceneCameraRend->SetActiveFlag(false);

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

	XMFLOAT4 debugColor = WhiteColor;

	// 画面停止中の処理
	if (m_ScreenStop == true) {

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

		debugColor = WeakRedColor;
	}

	// デバッグUIの色変更
	Render3DComponent* dbRender = m_DebugUIs[1]->GetComponent<Render3DComponent>();
	dbRender->SetColor(debugColor);
}

void DebugSystem::FrameAdvance(const std::vector<GameObject*>& objs) {

	const bool vk_F3_Trigger = Input::GetKeyTrigger(VK_F3);

	// コマ送り
	if (vk_F3_Trigger && m_ScreenStop == true) {
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
		scene->Update();
	}
}

void DebugSystem::SwitchingFillMode() {

	const bool vk_F4_Trigger = Input::GetKeyTrigger(VK_F4);

	if (vk_F4_Trigger) {
		DirectXRender::SwitchingFillMode();
	}
}

// 割り当てるキー
// F1キーでデバッグ機能のUIのON/OFF
// F2キーでオブジェクトの更新停止のON/OFF
// F3キーで停止中のコマ送り
// F4キーでワイヤーフレームとソリッドの切り替え