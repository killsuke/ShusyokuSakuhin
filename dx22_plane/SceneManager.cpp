#include "SceneManager.h"
#include "TextureManager.h"
#include "TitleScene.h"
#include "Stage1Scene.h"
#include "ResultScene.h"

Scene* SceneManager::m_pScene = nullptr;	// ここで書くことでちゃんと定義できる
bool SceneManager::sceneChangeFg = false;	// シーンチェンジのフラグ
SCENES SceneManager::nowScene{};	// シーンチェンジのフラグ
// コンストラクタ
SceneManager::SceneManager() {
//	sound.Init();	// サウンドの初期化

	m_pScene = new TitleScene();	// ここで最初に読み込むシーン生成
	nowScene = TITLE;
//	Debug::DebugFirst();
}

// 片付け
SceneManager::~SceneManager() {
	delete m_pScene;
	m_pScene = nullptr;
	TextureManager::ReleaseAllTextures();	// 各シーンのテクスチャーをまとめて破棄する
	//sound.Uninit();	// サウンドの終了処理
//	Debug::DebugFin();
}

void SceneManager::SceneChange(SCENES scene) {

	// シーンチェンジする際に現在のシーンを消して、新たなシーンを読み込むための準備をする
	if (m_pScene != nullptr) {

		sceneChangeFg = true;	// シーンが切り替わったことを知らせるフラグ
		delete m_pScene;
		m_pScene = nullptr;
	}

	// 読み込みたいシーンをnewしてくる
	switch (scene)
	{
	case SCENES::TITLE:
		m_pScene = new TitleScene();
		break;
	case SCENES::GAME:
		m_pScene = new Stage1Scene();
		break;
	case SCENES::RESULT:
		m_pScene = new ResultScene();
		break;
	default:
		break;
	}
	nowScene = scene;	// 現在のシーンが何かを判断するために更新
};

// 更新
void SceneManager::Update() {
	//Input::Update();
	//// ポインタ内に入ってるシーンの更新
	//m_pScene->Update();
};

// 描画
void SceneManager::Draw() {
	//D3D_StartRender();  //描画開始
	//m_pScene->Draw();	// ポインタ内に入ってるシーンの描画
	//D3D_FinishRender(); //描画終了
};