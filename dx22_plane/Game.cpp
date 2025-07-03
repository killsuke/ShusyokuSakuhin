#include "Game.h"
//#include "Renderer.h"
#include "Application.h"

Game* Game::m_Instance = nullptr;
SceneName Game::nowScene;
float Game::timeRecord = 0.0f;
Sound& Game::soundScene = Sound::GetInstance();
DirectXRender Game::d_rend;
// コンストラクタ
Game::Game()
{
	m_Input = std::make_unique<Input>(); //入力処理を作成
	m_Camera = std::make_unique<Camera>(); //カメラを作成
}

// デストラクタ
Game::~Game()
{

}

// 初期化
void Game::Init()
{
	// オブジェクト作成
	m_Instance = new Game;

	soundScene.Init();

	// 描画初期化
	d_rend.Init();

	// オブジェクト配列作成
	//m_Instance->m_Objects.emplace_back(new GolfBall);
	//m_Instance->m_Objects.emplace_back(new Ground);
	
	// カメラ初期化
	m_Instance->m_Camera->Init();

	m_Instance->m_Scene = new TitleScene; //メモリを確保

	// オブジェクト初期化
	// (※AddObject関数で実行するように変更）
	//for (auto& o : m_Instance->m_Objects)
	//{
	//	o->Init();
	//}
}

// 更新
void Game::Update()
{
	// シーン更新
	m_Instance->m_Scene->Update();

	// カメラ更新
	m_Instance->m_Camera->Update();

	// 入力処理更新
	m_Instance->m_Input->Update();

	// オブジェクト更新
	for (auto& o : m_Instance->m_Objects)
	{
		o->Update();
	}
}

// 描画
void Game::Draw()
{

	ImGui::Render();

	// 描画前処理
	d_rend.DrawBegin();

	// カメラ描画
	m_Instance->m_Camera->Draw();

	// オブジェクト描画
	for (auto& o : m_Instance->m_Objects)
	{
		o->Draw();
	}

	// ImGuiの描画
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// 描画後処理
	d_rend.DrawEnd();
}

// 終了処理
void Game::Uninit()
{
	delete m_Instance->m_Scene;
	m_Instance->m_Scene = nullptr;
	//オブジェクトを全て削除
	m_Instance->DeleteAllObject();

	// オブジェクト終了処理
 	// (※DeleteObject関数で実行するように変更）
	//for (auto& o : m_Instance->m_Objects)
	//{
	//	o->Uninit();
	//}

		// カメラ終了処理
	m_Instance->m_Camera->Uninit();

	// 描画終了処理
	RendererSystem::Uninit();

	soundScene.Uninit();

	delete m_Instance;
	m_Instance = nullptr;
}

// インスタンスを取得
Game* Game::GetInstance()
{
	return m_Instance;
}

// カメラを取得する
Camera& Game::GetCamera()
{
	return *(m_Instance->m_Camera);
}

// シーンを切り替える
void Game::ChangeScene(SceneName sName)
{
	nowScene = sName;
	// 読み込み済みのシーンがあれば削除
	int score = 0;
	if (m_Instance->m_Scene != nullptr) {
		// 消そうとしているシーンがStage1ならスコアを保存しておく
		/*if (Stage1Scene* sObj = dynamic_cast<Stage1Scene*>(m_Instance->m_Scene)) {
			score = sObj->GetScore();
		}*/

		delete m_Instance->m_Scene;
		m_Instance->m_Scene = nullptr;
	}

	switch (sName)
	{
	case TITLE:
		m_Instance->m_Scene = new TitleScene; // メモリを確保
		break;
	case STAGE1:
		m_Instance->m_Scene = new Stage1Scene; // メモリを確保
		break;
	case RESULT:
		m_Instance->m_Scene = new ResultScene; // メモリを確保
	//	dynamic_cast<ResultScene*>(m_Instance->m_Scene)->SetScore(score);	// スコアを設定
		break;
	}
}

// オブジェクトを削除する
void Game::DeleteObject(Object* pt)
{
	if (pt == NULL) return;

	pt->Uninit(); // 終了処理

	// 要素を削除
	m_Instance->m_Objects.erase(
		std::remove_if(
			m_Instance->m_Objects.begin(),
			m_Instance->m_Objects.end(),
			[pt](const std::unique_ptr<Object>& element) {return element.get() == pt; }),
		m_Instance->m_Objects.end());
	m_Instance->m_Objects.shrink_to_fit();
}

// オブジェクトをすべて削除する
void Game::DeleteAllObject()
{
	// オブジェクト終了処理
	for (auto& o : m_Instance->m_Objects)
	{
		o->Uninit();
	}

	m_Instance->m_Objects.clear(); //全て削除
	m_Instance->m_Objects.shrink_to_fit();
}

