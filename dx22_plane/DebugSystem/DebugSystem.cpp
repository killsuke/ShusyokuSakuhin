#include "DebugSystem.h"
#include "Manager/SceneManager.h"
#include "Manager/GameObjectManager.h"
#include "Manager/HitStopManager.h"
#include "Components/Transform.h"
#include "Components/Render3D.h"
#include "Mesh/SquareMesh.h"
#include "DirectXRender.h"
#include "input.h"
#include <iostream>

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace {

	const Vector3 DebugUIScale = Vector3(60.0f, 30.0f, 1.0f);
	const Vector3 StartUIPosition = Vector3(-300.0f, 330.0f, -1.0f);
	const XMFLOAT4 WeakRedColor = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
	const XMFLOAT4 WhiteColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	constexpr float ShiftUI = 120.0f;
	constexpr unsigned int UI_DisplayCount = 4;
}

void DebugSystem::Init() {
#if _DEBUG

	m_DebugUIs.resize(UI_DisplayCount, nullptr);

	m_DebugUITextureNames.resize(UI_DisplayCount);
	m_DebugUITextureNames[0] = "assets/texture/debugUI.png";
	m_DebugUITextureNames[1] = "assets/texture/debugUI_F2.png";
	m_DebugUITextureNames[2] = "assets/texture/debugUI_F3.png";
	m_DebugUITextureNames[3] = "assets/texture/debugUI_F4.png";

	Vector3 uiPosition = StartUIPosition;
	std::string debugUIName = "DebugUIObject";

	// デバッグUIの生成
	for(int i = 0; i < UI_DisplayCount; ++i) {
		m_DebugUIs[i] = nullptr;

		debugUIName += std::to_string(i + 1);
		m_DebugUIs[i] = GameObjectManager::AddUI(debugUIName, "DebugUI");
		m_DebugUIs[i]->SetCarryOverFlag(true);
		TransformComponent* dbTransform = m_DebugUIs[i]->AddComponent<TransformComponent>();
		dbTransform->SetPosition(uiPosition);
		dbTransform->SetScale(DebugUIScale);

		Render3DComponent* dbRender = m_DebugUIs[i]->AddComponent<Render3DComponent>();
		dbRender->CreateMesh<SquareMesh>();
		dbRender->SetShader("shader/unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
		dbRender->ChangeTexture(m_DebugUITextureNames[i]);

		uiPosition.x += ShiftUI;
	}

#endif
}

void DebugSystem::UnInit() {

	// デバッグUIの破棄
	m_DebugUIs.clear();
}

void DebugSystem::Update() {
	
	// デバッグUIのオブジェクトが生成されているか確認
	for (const GameObject* debugUI : m_DebugUIs) {
		if (debugUI == nullptr) {
			return;
		}
	}

	// 新しいオブジェクトが出来た時にちゃんと止められるか確認
	const std::vector<GameObject*> objs = GameObjectManager::GameObjectFindAllTagsOtherThan("Camera", "DebugUI");

	// デバッグ機能の処理
	DebugUI();
	ScreenStopped(objs);
	FrameAdvance(objs);
	SwitchingFillMode();
}

void DebugSystem::DebugUI() {
	if (Input::GetKeyTrigger(VK_F1)) {
		m_IsDebugUI = !m_IsDebugUI;

		// デバッグUIの表示非表示
		ActiveState debugUIState = m_IsDebugUI ? ActiveState::ACTIVE : ActiveState::DRAW_STOP;
	
		for(GameObject* debugUI : m_DebugUIs) {
			debugUI->SetActiveState(debugUIState);
		}
	}
}

void DebugSystem::ScreenStopped(const std::vector<GameObject*>& objs) {
	if (Input::GetKeyTrigger(VK_F2)) {
		m_ScreenStop = !m_ScreenStop;

		if (m_ScreenStop == false) {
			for (const auto& obj : objs) {
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

	XMFLOAT4 debugColor;
	debugColor = WhiteColor;

	// 画面停止中の処理
	if (m_ScreenStop == true) {
		for (const auto& obj : objs) {
			// 更新停止中か全停止中のオブジェクトはスルー
			const ActiveState currentState = obj->GetActiveState();
			if(currentState != ActiveState::ACTIVE) {
				continue;
			}

			obj->SetActiveState(ActiveState::UPDATE_STOP);
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

	// コマ送り
	if (Input::GetKeyTrigger(VK_F3) && m_ScreenStop == true) {
		for (const auto& obj : objs) {
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

	if (Input::GetKeyTrigger(VK_F4)) {
		DirectXRender::SwitchingFillMode();
	}
}

// 割り当てるキー
// F1キーでデバッグ機能のUIのON/OFF
// F2キーでオブジェクトの更新停止のON/OFF
// F3キーで停止中のコマ送り
// F4キーでワイヤーフレームとソリッドの切り替え