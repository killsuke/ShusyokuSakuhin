#include "SceneManager.h"
#include "GameObjectManager.h"
#include "HitStopManager.h"
#include "System/DirectXRender.h"
#include "System/Application.h"
#include "DebugSystem/DebugSystem.h"
#include "ShadowManager.h"

// 初期化
void SceneManager::Init() {
	
	m_Scene = std::make_unique<TitleScene>();	// 初期シーンはタイトルシーン
	//m_Scene = std::make_unique<Stage1Scene>();	// テスト用シーン
}

// 終了
void SceneManager::UnInit() {

};

// 更新
void SceneManager::Update() {

	DebugSystem::Update();

	if (m_Scene != nullptr) {

		if (m_Scene->GetSceneActive() == true)
		{
			HitStopManager::Update();

			// ポインタ内に入ってるシーンの更新
			m_Scene->Update();
		}
	}
	else {
		MessageBoxW(nullptr, L"シーンが存在しません。", L"Error", MB_OK | MB_ICONWARNING);
	}

	GameObjectManager::Update();

	ShadowManager::Update();
};

// 描画
void SceneManager::Draw() {

	// 描画前処理
	DirectXRender::DrawBegin();

	GameObjectManager::Draw();

	// 描画後処理
	DirectXRender::DrawEnd();
};