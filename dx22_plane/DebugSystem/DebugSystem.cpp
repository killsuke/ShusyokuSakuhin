#include "DebugSystem.h"
#include "Manager/SceneManager.h"
#include "Manager/GameObjectManager.h"
#include "HitStopManager.h"
#include "Components/Transform.h"
#include "Components/Render3D.h"
#include "Mesh/SquareMesh.h"
#include "DirectXRender.h"
#include "input.h"
#include <iostream>
#include <string>

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace {

	Vector3 DebugUIScale = Vector3(60.0f, 30.0f, 1.0f);
}

void DebugSystem::Init() {
#if _DEBUG

	// デバッグUIの生成
	{
		m_DebugUI_F1 = GameObjectManager::AddUI("DebugUIObject", "DebugUI");
		m_DebugUI_F1->SetCarryOverFlag(true);
		TransformComponent* dbTransform = m_DebugUI_F1->AddComponent<TransformComponent>();
		dbTransform->SetPosition(Vector3(-300.0f, 330.0f, -1.0f));
		dbTransform->SetScale(DebugUIScale);

		Render3DComponent* dbRender = m_DebugUI_F1->AddComponent<Render3DComponent>();
		dbRender->CreateMesh<SquareMesh>();
		dbRender->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
		dbRender->ChangeTexture("assets/texture/debugUI.png");
	}

	{
		m_DebugUI_F2 = GameObjectManager::AddUI("DebugUIObject2", "DebugUI");
		m_DebugUI_F2->SetCarryOverFlag(true);
		TransformComponent* dbTransform = m_DebugUI_F2->AddComponent<TransformComponent>();
		dbTransform->SetPosition(Vector3(-180.0f, 330.0f, -1.0f));
		dbTransform->SetScale(DebugUIScale);

		Render3DComponent* dbRender = m_DebugUI_F2->AddComponent<Render3DComponent>();
		dbRender->CreateMesh<SquareMesh>();
		dbRender->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
		dbRender->ChangeTexture("assets/texture/debugUI_F2.png");
	}

	{
		m_DebugUI_F3 = GameObjectManager::AddUI("DebugUIObject3", "DebugUI");
		m_DebugUI_F3->SetCarryOverFlag(true);
		TransformComponent* dbTransform = m_DebugUI_F3->AddComponent<TransformComponent>();
		dbTransform->SetPosition(Vector3(-60.0f, 330.0f, -1.0f));
		dbTransform->SetScale(DebugUIScale);

		Render3DComponent* dbRender = m_DebugUI_F3->AddComponent<Render3DComponent>();
		dbRender->CreateMesh<SquareMesh>();
		dbRender->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
		dbRender->ChangeTexture("assets/texture/debugUI_F3.png");
	}

	{
		m_DebugUI_F4 = GameObjectManager::AddUI("DebugUIObject4", "DebugUI");
		m_DebugUI_F4->SetCarryOverFlag(true);
		TransformComponent* dbTransform = m_DebugUI_F4->AddComponent<TransformComponent>();
		dbTransform->SetPosition(Vector3(60.0f, 330.0f, -1.0f));
		dbTransform->SetScale(DebugUIScale);

		Render3DComponent* dbRender = m_DebugUI_F4->AddComponent<Render3DComponent>();
		dbRender->CreateMesh<SquareMesh>();
		dbRender->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
		dbRender->ChangeTexture("assets/texture/debugUI_F4.png");
	}


#endif
}

void DebugSystem::UnInit() {

	// デバッグUIの破棄
	if (m_DebugUI_F1 != nullptr) {
		m_DebugUI_F1 = nullptr;
	}

	if (m_DebugUI_F2 != nullptr) {
		m_DebugUI_F2 = nullptr;
	}

	if (m_DebugUI_F3 != nullptr) {
		m_DebugUI_F3 = nullptr;
	}

	if (m_DebugUI_F4 != nullptr) {
		m_DebugUI_F4 = nullptr;
	}
}

void DebugSystem::Update() {
	if (m_DebugUI_F1 == nullptr || m_DebugUI_F2 == nullptr || m_DebugUI_F3 == nullptr || m_DebugUI_F4 == nullptr) {
		return;
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
		if (m_IsDebugUI == false) {
			m_DebugUI_F1->SetActiveState(ActiveState::DRAW_STOP);
			m_DebugUI_F2->SetActiveState(ActiveState::DRAW_STOP);
			m_DebugUI_F3->SetActiveState(ActiveState::DRAW_STOP);
			m_DebugUI_F4->SetActiveState(ActiveState::DRAW_STOP);
		}
		else {
			m_DebugUI_F1->SetActiveState(ActiveState::ACTIVE);
			m_DebugUI_F2->SetActiveState(ActiveState::ACTIVE);
			m_DebugUI_F3->SetActiveState(ActiveState::ACTIVE);
			m_DebugUI_F4->SetActiveState(ActiveState::ACTIVE);
		}
	}
}

void DebugSystem::ScreenStopped(const std::vector<GameObject*>& objs) {
	if (Input::GetKeyTrigger(VK_F2)) {
		m_ScreenStop = !m_ScreenStop;

		if (m_ScreenStop == false) {
			for (const auto& obj : objs) {
				obj->SetActiveState(ActiveState::ACTIVE);
			}

			Scene* scene = SceneManager::GetScene();
			if (scene != nullptr) {
				scene->SetSceneActive(true);
			}
			return;
		}
	}

	if (m_ScreenStop == true) {
		for (const auto& obj : objs) {
			obj->SetActiveState(ActiveState::UPDATE_STOP);
		}
		Scene* scene = SceneManager::GetScene();
		if (scene != nullptr) {
			scene->SetSceneActive(false);
		}

		// デバッグUIの色変更
		Render3DComponent* dbRender = m_DebugUI_F2->GetComponent<Render3DComponent>();
		dbRender->SetColor(XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f));
	}
	else {
		// デバッグUIの色変更
		Render3DComponent* dbRender = m_DebugUI_F2->GetComponent<Render3DComponent>();
		dbRender->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void DebugSystem::FrameAdvance(const std::vector<GameObject*>& objs) {

	// コマ送り
	if (Input::GetKeyTrigger(VK_F3) && m_ScreenStop == true) {
		for (const auto& obj : objs) {

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