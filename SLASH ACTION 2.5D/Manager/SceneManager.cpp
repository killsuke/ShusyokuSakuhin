#include "SceneManager.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "HitStopManager.h"
#include "System/Application.h"
#include "DebugSystem/DebugSystem.h"
#include "SoundManager.h"
#include "ShadowManager.h"

void SceneManager::Init() {
	
	SoundManager::Init();	// サウンドの初期化
	TextureManager::Init(); // テクスチャマネージャーの初期化処理
	GameObjectManager::Init();	// ゲームオブジェクトのマネージャーを初期化
	m_Scene = std::make_unique<TitleScene>();	// 初期シーンはタイトルシーン
	//m_Scene = std::make_unique<Stage1Scene>();	// 初期シーンはタイトルシーン
	//	Debug::DebugFirst();
}

void SceneManager::UnInit() {

	GameObjectManager::UnInit();	// ゲームオブジェクトのマネージャーを終了

	TextureManager::UnInit(); // テクスチャマネージャーの終了処理

	SoundManager::UnInit();	// サウンドの終了処理
	//	Debug::DebugFin();

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