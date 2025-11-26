#include "SceneManager.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "HitStopManager.h"
#include "Application.h"

std::unique_ptr<Scene> SceneManager::m_pScene;	// ここで書くことでちゃんと定義できる
bool SceneManager::sceneChangeFg = false;	// シーンチェンジのフラグ
bool SceneManager::m_ActiveGame = true;
float SceneManager::waitTime = 0.0f;	// シーンチェンジの待ち時間
float SceneManager::waitTimeCounter = 0.0f;	// シーンチェンジの待ち時間

void SceneManager::Init() {
	sound.Init();	// サウンドの初期化
	GameObjectManager::Init();	// ゲームオブジェクトのマネージャーを初期化
	m_pScene = std::make_unique<TitleScene>();	// 初期シーンはタイトルシーン
	//m_pScene = std::make_unique<Stage1Scene>();	// 初期シーンはタイトルシーン
	//	Debug::DebugFirst();
}

void SceneManager::UnInit() {

	GameObjectManager::UnInit();	// ゲームオブジェクトのマネージャーを終了

	TextureManager::UnInit(); // テクスチャマネージャーの終了処理

	sound.Uninit();	// サウンドの終了処理
	//	Debug::DebugFin();

};

// 更新
void SceneManager::Update() {
	Input::Update();

#if _DEBUG

	if (Input::GetKeyTrigger(VK_INSERT)) {
		DirectXRender::SwitchingFillMode();
	}
#endif

	// 即席ポーズ画面実装
	if (Input::GetKeyTrigger(VK_CONTROL) == true) {
		m_ActiveGame = !m_ActiveGame;

		std::vector<GameObject*> objs = GameObjectManager::GameObjectFindAllTagsOtherThan("Camera");

		if (m_ActiveGame == false) {
			for (const auto& obj : objs) {
				obj->SetActiveState(ActiveState::UPDATE_STOP);
			}
		}
		else {
			for (const auto& obj : objs)
			{
				obj->SetActiveState(ActiveState::ACTIVE);
			}
		}
	}

	if (m_ActiveGame == true) {
		HitStopManager::Update();

		// ポインタ内に入ってるシーンの更新
		m_pScene->Update();
	}
	else {
		if (Input::GetKeyTrigger(VK_TAB) == true) {
			std::vector<GameObject*> objs = GameObjectManager::GameObjectFindAllTagsOtherThan("Camera");

			for (const auto& obj : objs) {

				obj->SetActiveState(ActiveState::ACTIVE);
				obj->Update();
				obj->SetActiveState(ActiveState::UPDATE_STOP);
			}

			HitStopManager::Update();

			// ポインタ内に入ってるシーンの更新
			m_pScene->Update();

		}
	}

	GameObjectManager::Update();
};

// 描画
void SceneManager::Draw() {

#if _DEBUG

	ImGui::Render();

#endif

	// 描画前処理
	DirectXRender::DrawBegin();

	GameObjectManager::Draw();

#if _DEBUG

	// ImGuiの描画
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

#endif

	// 描画後処理
	DirectXRender::DrawEnd();
};