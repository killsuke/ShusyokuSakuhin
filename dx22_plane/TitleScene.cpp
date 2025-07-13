#include "TitleScene.h"
#include "Game.h"
#include "Texture2D.h"
#include "TestCube.h"
#include "EntityManager.h"
#include "Singleton.h"
#include "TestBoard.h"
#include "Test_Transparent.h"
#include "SkyDome.h"

// コンストラクタ
TitleScene::TitleScene()
{
	Init();
}

// デストラクタ
TitleScene::~TitleScene()
{
	Uninit();
}

// 初期化
void TitleScene::Init()
{
	TestCube* cube = Game::GetInstance()->AddObject<TestCube>();
	TestBoard* board = Game::GetInstance()->AddObject<TestBoard>();
	//SkyDome* dome = Game::GetInstance()->AddObject<SkyDome>();
	Test_Transparent* transparent = Game::GetInstance()->AddObject<Test_Transparent>();

	//TestBoard* board = Game::GetInstance()->AddObject<TestBoard>();
	//Test_Transparent* transparent = Game::GetInstance()->AddObject<Test_Transparent>();
	/*Test_Transparent* transparent2 = Game::GetInstance()->AddObject<Test_Transparent>();

	transparent2->SetPosition({ 50.0f, 100.0f, 0.0f });*/

//	EntityManager* entityMn = mozc::Singleton<EntityManager>::GetInstance();


	// 背景画像オブジェクトを作成
	//Texture2D* pt = Game::GetInstance()->AddObject<Texture2D>();
	//pt->SetTexture("assets/texture/title_car.png");	// 画像を指定
	//pt->SetPosition(0.0f, 0.0f, 0.0f);	// 位置を指定
	//pt->SetRotation(0.0f, 0.0f, 0.0f);	// 角度を指定
	//pt->SetScale(1280.0f, 720.0f, 0.0f);// 大きさを指定
	//m_MySceneObjects.emplace_back(pt);

	// 背景画像オブジェクトを作成
	//Texture2D* pt2 = Game::GetInstance()->AddObject<Texture2D>();
	//pt2->SetTexture("assets/texture/titlerogo.png");	// 画像を指定
	//pt2->SetPosition(0.0f, 0.0f, 0.0f);	// 位置を指定
	//pt2->SetRotation(0.0f, 0.0f, 0.0f);	// 角度を指定
	//pt2->SetScale(780.0f, 520.0f, 0.0f);// 大きさを指定
	//m_MySceneObjects.emplace_back(pt2);

	//soundTitle.Play(SOUND_LABEL_BGM001);
}

// 更新
void TitleScene::Update()
{
	// エンターキーを押してステージ1へ
	//if ((Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_A)) && setumeiFg == true)
	//{
	//	Game::GetInstance()->ChangeScene(STAGE1);
	//}

	//else if ((Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_A)) && setumeiFg == false)
	//{
	//	Texture2D* pt = Game::GetInstance()->AddObject<Texture2D>();
	//	pt->SetTexture("assets/texture/setumei.png");	// 画像を指定
	//	pt->SetPosition(0.0f, 0.0f, 0.0f);	// 位置を指定
	//	pt->SetRotation(0.0f, 0.0f, 0.0f);	// 角度を指定
	//	pt->SetScale(640.0f, 600.0f, 0.0f);// 大きさを指定
	//	m_MySceneObjects.emplace_back(pt);

	//	setumeiFg = true;
	//}

	
}

// 終了処理
void TitleScene::Uninit()
{
	// このシーンのオブジェクトを削除する
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
	//soundTitle.Stop(SOUND_LABEL_BGM001);
}
