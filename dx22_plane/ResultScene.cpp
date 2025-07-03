#include "ResultScene.h"
#include "Game.h"

// コンストラクタ
ResultScene::ResultScene()
{
	Init();
}

// デストラクタ
ResultScene::~ResultScene()
{
	Uninit();
}

// 初期化
void ResultScene::Init()
{
	// 背景画像オブジェクトを作成
	Texture2D* pt = Game::GetInstance()->AddObject<Texture2D>();
	pt->SetTexture("assets/texture/goal.png");	// 画像を指定
	pt->SetScale(1280.0f, 720.0f, 0.0f);	// 大きさを指定
	m_MySceneObjects.emplace_back(pt);

	// リザルト文字列オブジェクトを作成
	//Texture2D* pt2 = Game::GetInstance()->AddObject<Texture2D>();
	//pt2->SetTexture("assets/texture/resultString.png");	// 画像を指定
	//pt2->SetPosition(300.0f, 0.0f, 0.0f);	// 位置を指定
	//pt2->SetScale(700.0f, 100.0f, 0.0f);	// 大きさを指定
	//pt2->SetUV(1, 1, 1, 13);	// ＵＶを指定
	//m_MySceneObjects.emplace_back(pt2);

	// タイヤオブジェクトを作成
	Texture2D* pt3 = Game::GetInstance()->AddObject<Texture2D>();
	pt3->SetTexture("assets/texture/ranks.png");	// 画像を指定
	pt3->SetPosition(1000.0f, 0.0f, 0.0f);	// 位置を指定
	pt3->SetScale(361.0, 400.0f, 0.0f);	// 大きさを指定
	pt3->SetUV(1, 1, 3, 1);	// 大きさを指定
	m_MySceneObjects.emplace_back(pt3);
	texRank = pt3;

	Texture2D* pt7 = Game::GetInstance()->AddObject<Texture2D>();
	pt7->SetTexture("assets/texture/number.png");	// 画像を指定
	pt7->SetPosition(-10.0f, 0.0f, 0.0f);	// 位置を指定
	pt7->SetScale(80.0f, 80.0f, 0.0f);	// 大きさを指定
	pt7->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt7);
	texTime1 = pt7;

	Texture2D* pt8 = Game::GetInstance()->AddObject<Texture2D>();
	pt8->SetTexture("assets/texture/number.png");	// 画像を指定
	pt8->SetPosition(-80.0f, 0.0f, 0.0f);	// 位置を指定
	pt8->SetScale(80.0f, 80.0f, 0.0f);	// 大きさを指定
	pt8->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt8);
	texTime2 = pt8;

	Texture2D* colon1 = Game::GetInstance()->AddObject<Texture2D>();
	colon1->SetTexture("assets/texture/koron.png");	// 画像を指定
	colon1->SetPosition(-150.0f, 0.0f, 0.0f);	// 位置を指定
	colon1->SetScale(90.0f, 70.0f, 0.0f);	// 大きさを指定
	colon1->SetUV(1, 1, 1, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(colon1);

	Texture2D* pt9 = Game::GetInstance()->AddObject<Texture2D>();
	pt9->SetTexture("assets/texture/number.png");	// 画像を指定
	pt9->SetPosition(-220.0f, 0.0f, 0.0f);	// 位置を指定
	pt9->SetScale(80.0f, 80.0f, 0.0f);	// 大きさを指定
	pt9->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt9);
	texTime3 = pt9;

	Texture2D* pt10 = Game::GetInstance()->AddObject<Texture2D>();
	pt10->SetTexture("assets/texture/number.png");	// 画像を指定
	pt10->SetPosition(-290.0f, 0.0f, 0.0f);	// 位置を指定
	pt10->SetScale(80.0f, 80.0f, 0.0f);	// 大きさを指定
	pt10->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt10);
	texTime4 = pt10;

	Texture2D* colon2 = Game::GetInstance()->AddObject<Texture2D>();
	colon2->SetTexture("assets/texture/koron.png");	// 画像を指定
	colon2->SetPosition(-340.0f, 0.0f, 0.0f);	// 位置を指定
	colon2->SetScale(90.0f, 70.0f, 0.0f);	// 大きさを指定
	colon2->SetUV(1, 1, 1, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(colon2);

	Texture2D* pt11 = Game::GetInstance()->AddObject<Texture2D>();
	pt11->SetTexture("assets/texture/number.png");	// 画像を指定
	pt11->SetPosition(-390.0f, 0.0f, 0.0f);	// 位置を指定
	pt11->SetScale(80.0f, 80.0f, 0.0f);	// 大きさを指定
	pt11->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt11);
	texTime5 = pt11;

	Texture2D* pt12 = Game::GetInstance()->AddObject<Texture2D>();
	pt12->SetTexture("assets/texture/number.png");	// 画像を指定
	pt12->SetPosition(-460.0f, 0.0f, 0.0f);	// 位置を指定
	pt12->SetScale(80.0f, 80.0f, 0.0f);	// 大きさを指定
	pt12->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt12);
	texTime6 = pt12;

	startTime = std::chrono::high_resolution_clock::now();
	soundResult.Play(SOUND_LABEL_BGM002);
	soundResult.Stop(SOUND_LABEL_SE000);
}

// 更新
void ResultScene::Update()
{
	float recordTime = Game::GetTimeRecord();

	if (Game::GetNowScene() == RESULT) {
		//resultCount++;
		//if (resultCount == 1) {
		//	soundResult.Play(SOUND_LABEL_SE004);
		//}
		//if (resultCount < 100) {
		//	
		//	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		//	// スタートから現在までの経過時間を計算 
		//	std::chrono::duration<float> elapsed = now - startTime;

		//	Texture2D* timesDecimal[2];
		//	timesDecimal[0] = texTime1;	// 現在時刻の数値
		//	timesDecimal[1] = texTime2;	// 現在時刻の数値

		//	Texture2D* timesInteger[2];
		//	timesInteger[0] = texTime3;	// 現在時刻の数値
		//	timesInteger[1] = texTime4;	// 現在時刻の数値

		//	Texture2D* timeMinutes[2];
		//	timeMinutes[0] = texTime5;	// 現在時刻の数値
		//	timeMinutes[1] = texTime6;	// 現在時刻の数値

		//	int time1 = 0;
		//	int time2 = 0;
		//	int time3 = 0;

		//	time1 = int(elapsed.count());	// 整数
		//	time2 = int((elapsed.count() - time1) * 100);	// 整数部分
		//	time3 = int((elapsed.count() - time1) * 10);	// 整数部分
		//	time3 = time2 - time3;
		//	
		//	// ドラムロールっぽいのをやる
		//	// 各桁を後ろから取得していく
		//	// 小数
		//	for (int i = 0; i < 1; ++i) {
		//		int cnt = time3 % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
		//		timesDecimal[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
		//	}
		//	for (int i = 0; i < 1; ++i) {
		//		int cnt = time3 % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
		//		i++;
		//		timesDecimal[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
		//	}

		//	// 秒
		//	for (int i = 0; i < 1; ++i) {
		//		int cnt = time3 % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
		//		timesInteger[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
		//	}
		//	for (int i = 0; i < 1; ++i) {
		//		int cnt = time3 % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
		//		i++;
		//		timesInteger[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
		//	}

		//	// 分
		//	for (int i = 0; i < 1; ++i) {
		//		int cnt = time3 % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
		//		timeMinutes[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
		//	}
		//	for (int i = 0; i < 1; ++i) {
		//		int cnt = time3 % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
		//		i++;
		//		timeMinutes[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
		//	}
		//}
		//else {
		//	if (resultCount == 101) {
		//		soundResult.Stop(SOUND_LABEL_SE004);
		//		soundResult.Play(SOUND_LABEL_SE005);
		//	}
		//	int time1 = 0;
		//	int time2 = 0;
		//	int time3 = 0;
		//	int time4 = 0;

		//	Texture2D* timesDecimal[2];
		//	timesDecimal[0] = texTime1;	// 現在時刻の数値
		//	timesDecimal[1] = texTime2;	// 現在時刻の数値

		//	Texture2D* timesInteger[2];
		//	timesInteger[0] = texTime3;	// 現在時刻の数値
		//	timesInteger[1] = texTime4;	// 現在時刻の数値

		//	Texture2D* timeMinutes[2];
		//	timeMinutes[0] = texTime5;	// 現在時刻の数値
		//	timeMinutes[1] = texTime6;	// 現在時刻の数値


		//	time1 = int(recordTime);	// 整数
		//	time2 = int((recordTime - time1) * 100);	// 整数部分
		//	time3 = time1 / 60;
		//	time4 = time1 % 60;	// ここで秒計算

		//	// 各桁を後ろから取得していく
		//	// 小数
		//	for (int i = 0; i < 2; ++i) {
		//		time2 -= 1;
		//		int cnt = time2 % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
		//		timesDecimal[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
		//	}

		//	// 秒
		//	for (int i = 0; i < 2; ++i) {
		//		int cnt = time4 % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
		//		timesInteger[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
		//	}

		//	// 分
		//	for (int i = 0; i < 2; ++i) {
		//		int cnt = time3 % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
		//		timeMinutes[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
		//	}

		//	auto pos = texRank->GetPosition();
		//	if (resultCount > 101 && pos.x > 390.0f) {
		//		pos.x -= 100.0f;
		//		texRank->SetPosition(pos.x, pos.y, pos.z);

		//		// ランク付け
		//		if (recordTime < 90.0f) {
		//			texRank->SetUV(3,1,3,1);
		//		}
		//		else if (recordTime > 90.0f && recordTime < 150.0f) {
		//			texRank->SetUV(1, 1, 3, 1);
		//		}
		//		if (recordTime > 150.0f) {
		//			texRank->SetUV(2, 1, 3, 1);
		//		}
		//	}

		//	if (pos.x > 400 && pos.x < 500) {
		//		soundResult.Play(SOUND_LABEL_SE006);
		//	}

		//}
	}


	// エンターキーを押してタイトルへ
	if (Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_A))
	{
		Game::GetInstance()->ChangeScene(TITLE);
	}
}

// 終了処理
void ResultScene::Uninit()
{
	// このシーンのオブジェクトを削除する
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
	soundResult.Stop(SOUND_LABEL_BGM002);
}

// スコアを設定
void ResultScene::SetScore(int c) {
	// リザルト文字列オブジェクト
	Texture2D* stringObj = dynamic_cast<Texture2D*>(m_MySceneObjects[1]);

	switch (c) {

	case -4:
		stringObj->SetUV(1, 2, 1, 13);	// -4 コンドル
		break;
	case -3:
		stringObj->SetUV(1, 3, 1, 13);	// -3 アルバトロス
		break;
	case -2:
		stringObj->SetUV(1, 4, 1, 13);	// -2 イーグル
		break;
	case -1:
		stringObj->SetUV(1, 5, 1, 13);	// -1 バーディ
		break;
	case 0:
		stringObj->SetUV(1, 6, 1, 13);	// パー
		break;
	case 1:
		stringObj->SetUV(1, 7, 1, 13);	// +1 ボギー
		break;
	case 2:
		stringObj->SetUV(1, 8, 1, 13);	// +2 ダブルボギー
		break;
	case 3:
		stringObj->SetUV(1, 9, 1, 13);	// +3 トリプルボギー
		break;
	case 4:
		stringObj->SetUV(1, 10, 1, 13);	// +4
		break;
	case 5:
		stringObj->SetUV(1, 11, 1, 13);	// +5
		break;
	case 6:
		stringObj->SetUV(1, 12, 1, 13);	// +6
		break;
	default:
		stringObj->SetUV(1, 13, 1, 13);	// +7以上
		break;
	}
}