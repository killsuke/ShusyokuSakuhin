#include "SceneManager.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "Application.h"

std::unique_ptr<Scene> SceneManager::m_pScene;	// ここで書くことでちゃんと定義できる
bool SceneManager::sceneChangeFg = false;	// シーンチェンジのフラグ
float SceneManager::waitTime = 0.0f;	// シーンチェンジの待ち時間
float SceneManager::waitTimeCounter = 0.0f;	// シーンチェンジの待ち時間

void SceneManager::Init() {
	sound.Init();	// サウンドの初期化
	m_pScene = std::make_unique<TitleScene>();	// 初期シーンはタイトルシーン
	GameObjectManager::Init();	// ゲームオブジェクトのマネージャーを初期化
	//	Debug::DebugFirst();

}

void SceneManager::UnInit() {
	//TextureManager::ReleaseAllTextures();	// 各シーンのテクスチャーをまとめて破棄する

	GameObjectManager::UnInit();	// ゲームオブジェクトのマネージャーを終了

	sound.Uninit();	// サウンドの終了処理
//	Debug::DebugFin();

};

// 更新
void SceneManager::Update() {
	Input::Update();

	//if (waitTime != 0.0f) {
	//	waitTimeCounter += 0.016f;
	//}

	// ポインタ内に入ってるシーンの更新
	m_pScene->Update();

	GameObjectManager::Update();
};

// 描画
void SceneManager::Draw() {
	ImGui::Render();

	// 描画前処理
	DirectXRender::DrawBegin();
		
	GameObjectManager::Draw();

	// ImGuiの描画
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// 描画後処理
	DirectXRender::DrawEnd();
};