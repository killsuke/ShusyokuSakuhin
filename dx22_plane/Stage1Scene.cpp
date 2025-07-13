#include "Stage1Scene.h"
#include "Game.h"
#include "TestCube.h"

using namespace DirectX::SimpleMath;
int Stage1Scene::countDown = 0;

// コンストラクタ
Stage1Scene::Stage1Scene()
{
	Init();
}

// デストラクタ
Stage1Scene::~Stage1Scene()
{
	Uninit();
}

// 初期化
void Stage1Scene::Init()
{
	m_Par = 4;	// パー（標準打数）を設定
	m_StrokeCount = 0;	// 現在打数を初期化
	countDown = 0;

	m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<TestCube>());

	// ＵＩ（背景）
	Texture2D* pt1 = Game::GetInstance()->AddObject<Texture2D>();
	pt1->SetTexture("assets/texture/ui_back.png");	// 画像を指定
	pt1->SetPosition(-475.0f, -300.0f, 0.0f);	// 位置を指定
	pt1->SetScale(270.0f, 75.0f, 0.0f);	// 大きさを指定
	m_MySceneObjects.emplace_back(pt1);

	// ＵＩ（現在打数の数値　一の位）
	Texture2D* pt5 = Game::GetInstance()->AddObject<Texture2D>();
	pt5->SetTexture("assets/texture/number.png");	// 画像を指定
	pt5->SetPosition(-485.0f, -300.0f, 0.0f);	// 位置を指定
	pt5->SetScale(95.0f, 72.0f, 0.0f);	// 大きさを指定
	pt5->SetUV(2, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt5);
	texOne = pt5;

	// ＵＩ（背景）
	Texture2D* pt6 = Game::GetInstance()->AddObject<Texture2D>();
	pt6->SetTexture("assets/texture/number.png");	// 画像を指定
	pt6->SetPosition(-556.0f, -300.0f, 0.0f);	// 位置を指定
	pt6->SetScale(95.0f, 72.0f, 0.0f);	// 大きさを指定
	pt6->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt6);
	texTen = pt6;

	Texture2D* pt7 = Game::GetInstance()->AddObject<Texture2D>();
	pt7->SetTexture("assets/texture/number.png");	// 画像を指定
	pt7->SetPosition(570.0f, 300.0f, 0.0f);	// 位置を指定
	pt7->SetScale(80.0f, 72.0f, 0.0f);	// 大きさを指定
	pt7->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt7);
	texTime1 = pt7;

	Texture2D* pt8 = Game::GetInstance()->AddObject<Texture2D>();
	pt8->SetTexture("assets/texture/number.png");	// 画像を指定
	pt8->SetPosition(500.0f, 300.0f, 0.0f);	// 位置を指定
	pt8->SetScale(80.0f, 72.0f, 0.0f);	// 大きさを指定
	pt8->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt8);
	texTime2 = pt8;

	Texture2D* colon1 = Game::GetInstance()->AddObject<Texture2D>();
	colon1->SetTexture("assets/texture/koron.png");	// 画像を指定
	colon1->SetPosition(450.0f, 300.0f, 0.0f);	// 位置を指定
	colon1->SetScale(90.0f, 70.0f, 0.0f);	// 大きさを指定
	colon1->SetUV(1, 1, 1, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(colon1);

	Texture2D* pt9 = Game::GetInstance()->AddObject<Texture2D>();
	pt9->SetTexture("assets/texture/number.png");	// 画像を指定
	pt9->SetPosition(400.0f, 300.0f, 0.0f);	// 位置を指定
	pt9->SetScale(80.0f, 72.0f, 0.0f);	// 大きさを指定
	pt9->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt9);
	texTime3 = pt9;

	Texture2D* pt10 = Game::GetInstance()->AddObject<Texture2D>();
	pt10->SetTexture("assets/texture/number.png");	// 画像を指定
	pt10->SetPosition(330.0f, 300.0f, 0.0f);	// 位置を指定
	pt10->SetScale(80.0f, 72.0f, 0.0f);	// 大きさを指定
	pt10->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt10);
	texTime4 = pt10;

	Texture2D* colon2 = Game::GetInstance()->AddObject<Texture2D>();
	colon2->SetTexture("assets/texture/koron.png");	// 画像を指定
	colon2->SetPosition(280.0f, 300.0f, 0.0f);	// 位置を指定
	colon2->SetScale(90.0f, 70.0f, 0.0f);	// 大きさを指定
	colon2->SetUV(1, 1, 1, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(colon2);

	Texture2D* pt11 = Game::GetInstance()->AddObject<Texture2D>();
	pt11->SetTexture("assets/texture/number.png");	// 画像を指定
	pt11->SetPosition(230.0f, 300.0f, 0.0f);	// 位置を指定
	pt11->SetScale(80.0f, 72.0f, 0.0f);	// 大きさを指定
	pt11->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt11);
	texTime5 = pt11;

	Texture2D* pt12 = Game::GetInstance()->AddObject<Texture2D>();
	pt12->SetTexture("assets/texture/number.png");	// 画像を指定
	pt12->SetPosition(160.0f, 300.0f, 0.0f);	// 位置を指定
	pt12->SetScale(80.0f, 72.0f, 0.0f);	// 大きさを指定
	pt12->SetUV(1, 1, 10, 1);	// ＵＶを指定
	m_MySceneObjects.emplace_back(pt12);
	texTime6 = pt12;

	Texture2D* counts = Game::GetInstance()->AddObject<Texture2D>();
	counts->SetTexture("assets/texture/3count.png");	// 画像を指定
	counts->SetPosition(0.0f, 0.0f, 0.0f);	// 位置を指定
	counts->SetScale(80.0f, 80.0f, 0.0f);	// 大きさを指定
	counts->SetUV(1, 1, 2, 2);	// ＵＶを指定
	m_MySceneObjects.emplace_back(counts);
	texCounts = counts;

}

//更新
void Stage1Scene::Update()
{
	int nowLap = 0;
	int time1 = 0;	// 取り敢えず仮置き
	int time2 = 0;	// 取り敢えず仮置き
	int time3 = 0;	// 取り敢えず仮置き
	int time4 = 0;	// 取り敢えず仮置き
	//GolfBall* ball = dynamic_cast<GolfBall*>(m_MySceneObjects[1]);	// ゴルフボール
	//Arrow* arrow = dynamic_cast<Arrow*>(m_MySceneObjects[2]);	// 矢印
	//CarModel* carM = dynamic_cast<CarModel*>(car);	// 矢印
	
	//Texture2D* tex1 = dynamic_cast<Texture2D*>(m_MySceneObjects[2]);
	m_State = 0;
	// 状態ごとに処理
	//switch (m_State) {

	//	// ボール移動中
	//case 0:
	if (Game::GetNowScene() == STAGE1) {
		if (countDown == 180) {
			startTime = std::chrono::high_resolution_clock::now();
			//soundStage.Play(SOUND_LABEL_SE003);
		}
		else if (countDown > 181) {
			std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
			// スタートから現在までの経過時間を計算 
			std::chrono::duration<float> elapsed = now - startTime;
			Game::SetTimeRecord(elapsed.count());	// ここで毎フレームタイムを記録
			// ボールが静止したら
			//if (ball->GetState() == 1) {
			//	m_State = 1;
				//arrow->SetState(m_State);

				// 打数を更新
			Texture2D* count[3];
			count[0] = texOne;	// 現在打数の数値　１の位
			count[1] = texTen;	// 現在打数の数値	１０の位

			Texture2D* timesDecimal[2];
			timesDecimal[0] = texTime1;	// 現在時刻の数値
			timesDecimal[1] = texTime2;	// 現在時刻の数値

			Texture2D* timesInteger[2];
			timesInteger[0] = texTime3;	// 現在時刻の数値
			timesInteger[1] = texTime4;	// 現在時刻の数値

			Texture2D* timeMinutes[2];
			timeMinutes[0] = texTime5;	// 現在時刻の数値
			timeMinutes[1] = texTime6;	// 現在時刻の数値

			//nowLap = carM->GetNowLap();

			//m_StrokeCount++;	// 現在打数をカウントアップ

			// 各桁を後ろから取得していく
			//for (int i = 0; i < 2; ++i) {
			//	int cnt = nowLap % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
			//	count[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
			//}

			if (nowLap == 4) {
				//soundStage.Stop(SOUND_LABEL_BGM000);
				Game::GetInstance()->ChangeScene(RESULT);
				return;
			}

			time1 = int(elapsed.count());	// 整数
			time2 = int((elapsed.count() - time1) * 100);	// 整数部分
			time3 = time1 / 60;
			time4 = time1 % 60;	// ここで秒計算

			// 各桁を後ろから取得していく
			// 小数
			//for (int i = 0; i < 2; ++i) {
			//	int cnt = time2 % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
			//	timesDecimal[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
			//}

			// 秒
			//for (int i = 0; i < 2; ++i) {
			//	int cnt = time4 % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
			//	timesInteger[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
			//}

			// 分
			//for (int i = 0; i < 2; ++i) {
			//	int cnt = time3 % (int)pow(10, i + 1) / (int)pow(10, i);	// １桁取り出す
			//	timeMinutes[i]->SetUV(cnt + 1, 1, 10, 1);	// ＵＶを設定
			//}
		}

		// カウントダウンの処理
	// カウントダウン切り替え
		if (countDown >= 0 && countDown <= 59) {
			texCounts->SetUV(1, 1, 2, 2);
		}
		else if (countDown >= 60 && countDown <= 119) {
			texCounts->SetUV(2, 1, 2, 2);
		}
		else if (countDown >= 120 && countDown <= 179) {
			texCounts->SetUV(1, 2, 2, 2);
		}
		else if (countDown >= 180 && countDown <= 239) {
			texCounts->SetUV(2, 2, 2, 2);
		}
		else if (countDown == 240) {
			texCounts->SetScale(0.0f, 0.0f, 0.0f);
			texCounts->SetPosition(800.0f, 0.0f, 0.0f);
			//soundStage.Play(SOUND_LABEL_BGM000);
		//	soundStage.SetVolume(SOUND_LABEL_BGM000,0.5f);
		}

		if (countDown < 240) {
			// ここで小さくしていく
			auto texSize = texCounts->GetScale();
			if (texSize.x > 0.0f && (onceStop > 0 && onceStop < 19 || onceStop > 39) ) {
				texSize.x -= 20.0f;
				texSize.y -= 20.0f;
				texAngleCount += 20.0f;
			}
			texCounts->SetScale(texSize.x, texSize.y, texSize.z);
			//	auto anglee = texCounts->GetRotation();
			texCounts->SetRotation(0.0f, 0.0f, texAngleCount);
		}

		// ここで大きくする処理を入れる
		if ((countDown % 60) == 0 && countDown < 240) {
			texCounts->SetScale(1000.0f, 1000.0f, 0.0f);
			texCounts->SetRotation(0.0f, 0.0f, 0.0f);
			texAngleCount = 0.0f;
			onceStop = 0;
			if (countDown < 180) {
		//		soundStage.Play(SOUND_LABEL_SE002);
			}
		}

		// カウントのストップ
		if (countDown > 241) {
			countDown = 241;
			onceStop = 241;
		}
		countDown++;
		onceStop++;
	}
	//	}
	//	break;
		// ボールがカップインしたらリザルトへ
		/*
if (ball->GetState() == 2) {
			Game::GetInstance()->ChangeScene(RESULT);
		}*/
		// 方向選択中
	//case 1:
	//	// スペースキーでパワー選択へ
	//	if (Input::GetKeyTrigger(VK_SPACE)) {
	//		m_State = 2;
	//	//	arrow->SetState(m_State);
	//	}
	//	break;

	//	// パワー選択中
	//case 2:
	//	// スペースキーでショット
	//	if (Input::GetKeyTrigger(VK_SPACE)) {
	//		m_State = 0;
	//		/*ball->SetState(m_State);
	//		arrow->SetState(m_State);

	//		Vector3 v = arrow->GetVector();
	//		ball->Shot(v);*/
	//	}
	//	break;

	//default:
	//	break;
	//}



	// エンターキーを押してリザルトへ
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Game::GetInstance()->ChangeScene(RESULT);
	}
}

// 終了処理
void Stage1Scene::Uninit()
{
	// このシーンのオブジェクトを削除する
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
}

// スコアを取得
int Stage1Scene::GetScore() {
	// 現在打数から標準打数を引いた数値をreturn
	return(m_StrokeCount - m_Par);
}

//void Stage1Scene::Roads() {
//	// 道路をまとめるベクター
//	std::vector<Object*> roads;
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize1 = roads[0]->GetScale();
//	auto roadPos1 = roads[0]->GetPosition();
//	roads[0]->SetScale(Vector3(roadSize1.x, roadSize1.y, roadSize1.z + 600.0f));
//	roads[0]->SetPosition(Vector3(roadPos1.x, roadPos1.y, roadPos1.z - 300.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize2 = roads[1]->GetScale();
//	auto roadAngle2 = roads[1]->GetRotation();
//	auto roadPos2 = roads[1]->GetPosition();
//	roads[1]->SetScale(Vector3(roadSize2.x, roadSize2.y, roadSize2.z + 300.0f));
//	roads[1]->SetRotation(Vector3(roadAngle2.x, roadAngle2.y + 45.0f, roadAngle2.z));
//	roads[1]->SetPosition(Vector3(roadPos2.x - 130.0f, roadPos2.y, roadPos2.z - 780.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize3 = roads[2]->GetScale();
//	auto roadAngle3 = roads[2]->GetRotation();
//	auto roadPos3 = roads[2]->GetPosition();
//	roads[2]->SetScale(Vector3(roadSize3.x, roadSize3.y, roadSize3.z + 300.0f));
//	roads[2]->SetRotation(Vector3(roadAngle3.x, roadAngle3.y + 60.0f, roadAngle3.z));
//	roads[2]->SetPosition(Vector3(roadPos3.x - 450.0f, roadPos3.y, roadPos3.z - 1020.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize4 = roads[3]->GetScale();
//	auto roadAngle4 = roads[3]->GetRotation();
//	auto roadPos4 = roads[3]->GetPosition();
//	roads[3]->SetScale(Vector3(roadSize4.x, roadSize4.y, roadSize4.z + 300.0f));
//	roads[3]->SetRotation(Vector3(roadAngle4.x, roadAngle4.y + 120.0f, roadAngle4.z));
//	roads[3]->SetPosition(Vector3(roadPos4.x - 450.0f, roadPos4.y, roadPos4.z - 1200.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize5 = roads[4]->GetScale();
//	auto roadAngle5 = roads[4]->GetRotation();
//	auto roadPos5 = roads[4]->GetPosition();
//	roads[4]->SetScale(Vector3(roadSize5.x, roadSize5.y, roadSize5.z + 300.0f));
//	roads[4]->SetRotation(Vector3(roadAngle5.x, roadAngle5.y + 90.0f, roadAngle5.z));
//	roads[4]->SetPosition(Vector3(roadPos5.x - 100.0f, roadPos5.y, roadPos5.z - 1300.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize6 = roads[5]->GetScale();
//	auto roadAngle6 = roads[5]->GetRotation();
//	auto roadPos6 = roads[5]->GetPosition();
//	roads[5]->SetScale(Vector3(roadSize6.x, roadSize6.y, roadSize6.z + 300.0f));
//	roads[5]->SetRotation(Vector3(roadAngle6.x, roadAngle6.y + 60.0f, roadAngle6.z));
//	roads[5]->SetPosition(Vector3(roadPos6.x + 300.0f, roadPos6.y, roadPos6.z - 1200.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize7 = roads[6]->GetScale();
//	auto roadAngle7 = roads[6]->GetRotation();
//	auto roadPos7 = roads[6]->GetPosition();
//	roads[6]->SetScale(Vector3(roadSize7.x, roadSize7.y, roadSize7.z + 300.0f));
//	roads[6]->SetRotation(Vector3(roadAngle7.x, roadAngle7.y + 60.0f, roadAngle7.z));
//	roads[6]->SetPosition(Vector3(roadPos7.x + 800.0f, roadPos7.y, roadPos7.z - 900.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize8 = roads[7]->GetScale();
//	auto roadAngle8 = roads[7]->GetRotation();
//	auto roadPos8 = roads[7]->GetPosition();
//	roads[7]->SetScale(Vector3(roadSize8.x, roadSize8.y, roadSize8.z + 300.0f));
//	roads[7]->SetRotation(Vector3(roadAngle8.x, roadAngle8.y + 60.0f, roadAngle8.z));
//	roads[7]->SetPosition(Vector3(roadPos8.x + 1300.0f, roadPos8.y, roadPos8.z - 600.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize9 = roads[8]->GetScale();
//	auto roadAngle9 = roads[8]->GetRotation();
//	auto roadPos9 = roads[8]->GetPosition();
//	roads[8]->SetScale(Vector3(roadSize9.x, roadSize9.y, roadSize9.z + 300.0f));
//	roads[8]->SetRotation(Vector3(roadAngle9.x, roadAngle9.y + 120.0f, roadAngle9.z));
//	roads[8]->SetPosition(Vector3(roadPos9.x + 1600.0f, roadPos9.y, roadPos9.z - 580.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize10 = roads[9]->GetScale();
//	auto roadAngle10 = roads[9]->GetRotation();
//	auto roadPos10 = roads[9]->GetPosition();
//	roads[9]->SetScale(Vector3(roadSize10.x, roadSize10.y, roadSize10.z + 300.0f));
//	roads[9]->SetRotation(Vector3(roadAngle10.x, roadAngle10.y + 70.0f, roadAngle10.z));
//	roads[9]->SetPosition(Vector3(roadPos10.x + 1950.0f, roadPos10.y, roadPos10.z - 600.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize11 = roads[10]->GetScale();
//	auto roadAngle11 = roads[10]->GetRotation();
//	auto roadPos11 = roads[10]->GetPosition();
//	roads[10]->SetScale(Vector3(roadSize11.x, roadSize11.y, roadSize11.z + 900.0f));
//	roads[10]->SetRotation(Vector3(roadAngle11.x, roadAngle11.y + 70.0f, roadAngle11.z));
//	roads[10]->SetPosition(Vector3(roadPos11.x + 2610.0f, roadPos11.y, roadPos11.z - 360.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize12 = roads[11]->GetScale();
//	auto roadAngle12 = roads[11]->GetRotation();
//	auto roadPos12 = roads[11]->GetPosition();
//	roads[11]->SetScale(Vector3(roadSize12.x, roadSize12.y, roadSize12.z + 300.0f));
//	roads[11]->SetRotation(Vector3(roadAngle12.x, roadAngle12.y - 30.0f, roadAngle12.z));
//	roads[11]->SetPosition(Vector3(roadPos12.x + 3000.0f, roadPos12.y, roadPos12.z));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize13 = roads[12]->GetScale();
//	auto roadAngle13 = roads[12]->GetRotation();
//	auto roadPos13 = roads[12]->GetPosition();
//	roads[12]->SetScale(Vector3(roadSize13.x, roadSize13.y, roadSize13.z + 300.0f));
//	roads[12]->SetRotation(Vector3(roadAngle13.x, roadAngle13.y - 60.0f, roadAngle13.z));
//	roads[12]->SetPosition(Vector3(roadPos13.x + 2700.0f, roadPos13.y, roadPos13.z + 280.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize14 = roads[13]->GetScale();
//	auto roadAngle14 = roads[13]->GetRotation();
//	auto roadPos14 = roads[13]->GetPosition();
//	roads[13]->SetScale(Vector3(roadSize14.x, roadSize14.y, roadSize14.z + 300.0f));
//	roads[13]->SetRotation(Vector3(roadAngle14.x, roadAngle14.y - 90.0f, roadAngle14.z));
//	roads[13]->SetPosition(Vector3(roadPos14.x + 2350.0f, roadPos14.y, roadPos14.z + 370.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize15 = roads[14]->GetScale();
//	auto roadAngle15 = roads[14]->GetRotation();
//	auto roadPos15 = roads[14]->GetPosition();
//	roads[14]->SetScale(Vector3(roadSize15.x, roadSize15.y, roadSize15.z + 1600.0f));
//	roads[14]->SetRotation(Vector3(roadAngle15.x, roadAngle15.y - 90.0f, roadAngle15.z));
//	roads[14]->SetPosition(Vector3(roadPos15.x + 1200.0f, roadPos15.y - 10.0f, roadPos15.z + 370.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize16 = roads[15]->GetScale();
//	auto roadAngle16 = roads[15]->GetRotation();
//	auto roadPos16 = roads[15]->GetPosition();
//	roads[15]->SetScale(Vector3(roadSize16.x - 40.0f, roadSize16.y, roadSize16.z + 300.0f));
//	roads[15]->SetRotation(Vector3(roadAngle16.x, roadAngle16.y - 90.0f, roadAngle16.z));
//	roads[15]->SetPosition(Vector3(roadPos16.x + 1700.0f, roadPos16.y + 20.0f, roadPos16.z + 370.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize17 = roads[16]->GetScale();
//	auto roadAngle17 = roads[16]->GetRotation();
//	auto roadPos17 = roads[16]->GetPosition();
//	roads[16]->SetScale(Vector3(roadSize17.x - 40.0f, roadSize17.y, roadSize17.z + 900.0f));
//	roads[16]->SetRotation(Vector3(roadAngle17.x, roadAngle17.y - 90.0f, roadAngle17.z));
//	roads[16]->SetPosition(Vector3(roadPos17.x + 1000.0f, roadPos17.y + 20.0f, roadPos17.z + 370.0f));
//
//	roads.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Road>()));
//	auto roadSize18 = roads[17]->GetScale();
//	auto roadAngle18 = roads[17]->GetRotation();
//	auto roadPos18 = roads[17]->GetPosition();
//	roads[17]->SetScale(Vector3(roadSize18.x + 30.0f, roadSize18.y, roadSize17.z + 300.0f));
//	roads[17]->SetRotation(Vector3(roadAngle18.x, roadAngle18.y + 20.0f, roadAngle17.z));
//	roads[17]->SetPosition(Vector3(roadPos18.x + 60.0f, roadPos18.y, roadPos17.z + 240.0f));
//}
//
//void Stage1Scene::Slopes() {
//	std::vector<Object*> slopes;
//	slopes.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Slope>()));
//	auto slopeSize1 = slopes[0]->GetScale();
//	auto slopeAngle1 = slopes[0]->GetRotation();
//	auto slopePos1 = slopes[0]->GetPosition();
//	slopes[0]->SetScale(Vector3(slopeSize1.x, slopeSize1.y, slopeSize1.z));
//	slopes[0]->SetRotation(Vector3(slopeAngle1.x, slopeAngle1.y + 240.0f, slopeAngle1.z));
//	slopes[0]->SetPosition(Vector3(slopePos1.x + 380.0f, slopePos1.y, slopePos1.z - 1000.0f));
//
//	slopes.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Slope>()));
//	auto slopeSize2 = slopes[1]->GetScale();
//	auto slopeAngle2 = slopes[1]->GetRotation();
//	auto slopePos2 = slopes[1]->GetPosition();
//	slopes[1]->SetScale(Vector3(slopeSize2.x, slopeSize2.y, slopeSize2.z));
//	slopes[1]->SetRotation(Vector3(slopeAngle2.x, slopeAngle2.y + 240.0f, slopeAngle2.z));
//	slopes[1]->SetPosition(Vector3(slopePos2.x + 380.0f, slopePos2.y, slopePos2.z - 1100.0f));
//
//	slopes.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Slope>()));
//	auto slopeSize3 = slopes[2]->GetScale();
//	auto slopeAngle3 = slopes[2]->GetRotation();
//	auto slopePos3 = slopes[2]->GetPosition();
//	slopes[2]->SetScale(Vector3(slopeSize3.x, slopeSize3.y, slopeSize3.z));
//	slopes[2]->SetRotation(Vector3(slopeAngle3.x, slopeAngle3.y + 240.0f, slopeAngle3.z));
//	slopes[2]->SetPosition(Vector3(slopePos3.x + 950.0f, slopePos3.y, slopePos3.z - 720.0f));
//
//	slopes.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Slope>()));
//	auto slopeSize4 = slopes[3]->GetScale();
//	auto slopeAngle4 = slopes[3]->GetRotation();
//	auto slopePos4 = slopes[3]->GetPosition();
//	slopes[3]->SetScale(Vector3(slopeSize4.x + 30.0f, slopeSize4.y, slopeSize4.z));
//	slopes[3]->SetRotation(Vector3(slopeAngle4.x, slopeAngle4.y + 90.0f, slopeAngle4.z));
//	slopes[3]->SetPosition(Vector3(slopePos4.x + 2150.0f, slopePos4.y, slopePos4.z + 470.0f));
//
//	slopes.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Slope>()));
//	auto slopeSize5 = slopes[4]->GetScale();
//	auto slopeAngle5 = slopes[4]->GetRotation();
//	auto slopePos5 = slopes[4]->GetPosition();
//	slopes[4]->SetScale(Vector3(slopeSize5.x + 30.0f, slopeSize5.y, slopeSize5.z));
//	slopes[4]->SetRotation(Vector3(slopeAngle5.x, slopeAngle5.y + 90.0f, slopeAngle5.z));
//	slopes[4]->SetPosition(Vector3(slopePos5.x + 480.0f, slopePos5.y - 10.0f, slopePos5.z + 470.0f));
//
//	slopes.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Slope>()));
//	auto slopeSize6 = slopes[5]->GetScale();
//	auto slopeAngle6 = slopes[5]->GetRotation();
//	auto slopePos6 = slopes[5]->GetPosition();
//	slopes[5]->SetScale(Vector3(slopeSize6.x + 30.0f, slopeSize6.y, slopeSize6.z));
//	slopes[5]->SetRotation(Vector3(slopeAngle6.x, slopeAngle6.y + 90.0f, slopeAngle6.z));
//	slopes[5]->SetPosition(Vector3(slopePos6.x + 470.0f, slopePos6.y + 20.0f, slopePos6.z + 470.0f));
//}
//
//void Stage1Scene::Walls() {
//	std::vector<Object*> walls;
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize1 = walls[0]->GetScale();
//	auto wallPos1 = walls[0]->GetPosition();
//	walls[0]->SetScale(Vector3(wallSize1.x, wallSize1.y, wallSize1.z));
//	walls[0]->SetPosition(Vector3(wallPos1.x + 60.0f, wallPos1.y, wallPos1.z - 100.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize2 = walls[1]->GetScale();
//	auto wallPos2 = walls[1]->GetPosition();
//	walls[1]->SetScale(Vector3(wallSize2.x, wallSize2.y, wallSize2.z + 40.0f));
//	walls[1]->SetPosition(Vector3(wallPos2.x - 60.0f, wallPos2.y, wallPos2.z - 80.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize3 = walls[2]->GetScale();
//	auto wallPos3 = walls[2]->GetPosition();
//	walls[2]->SetScale(Vector3(wallSize3.x, wallSize3.y, wallSize3.z + 140.0f));
//	walls[2]->SetPosition(Vector3(wallPos3.x + 60.0f, wallPos3.y, wallPos3.z - 470.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize4 = walls[3]->GetScale();
//	auto wallPos4 = walls[3]->GetPosition();
//	walls[3]->SetScale(Vector3(wallSize4.x, wallSize4.y, wallSize4.z + 80.0f));
//	walls[3]->SetPosition(Vector3(wallPos4.x - 60.0f, wallPos4.y, wallPos4.z - 440.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize5 = walls[4]->GetScale();
//	auto wallAngle5 = walls[4]->GetRotation();
//	auto wallPos5 = walls[4]->GetPosition();
//	walls[4]->SetScale(Vector3(wallSize5.x, wallSize5.y + 30.0f, wallSize5.z));
//	walls[4]->SetRotation(Vector3(wallAngle5.x, wallAngle5.y + 89.0f, wallAngle5.z));
//	walls[4]->SetPosition(Vector3(wallPos5.x + 1800.0f, wallPos5.y + 35.0f, wallPos5.z + 410.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize6 = walls[5]->GetScale();
//	auto wallAngle6 = walls[5]->GetRotation();
//	auto wallPos6 = walls[5]->GetPosition();
//	walls[5]->SetScale(Vector3(wallSize6.x, wallSize6.y + 30.0f, wallSize6.z));
//	walls[5]->SetRotation(Vector3(wallAngle6.x, wallAngle6.y - 89.0f, wallAngle6.z));
//	walls[5]->SetPosition(Vector3(wallPos6.x + 1800.0f, wallPos6.y + 35.0f, wallPos6.z + 330.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize7 = walls[6]->GetScale();
//	auto wallAngle7 = walls[6]->GetRotation();
//	auto wallPos7 = walls[6]->GetPosition();
//	walls[6]->SetScale(Vector3(wallSize7.x, wallSize7.y + 30.0f, wallSize7.z));
//	walls[6]->SetRotation(Vector3(wallAngle7.x, wallAngle7.y + 89.0f, wallAngle7.z));
//	walls[6]->SetPosition(Vector3(wallPos7.x + 1500.0f, wallPos7.y + 35.0f, wallPos7.z + 400.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize8 = walls[7]->GetScale();
//	auto wallAngle8 = walls[7]->GetRotation();
//	auto wallPos8 = walls[7]->GetPosition();
//	walls[7]->SetScale(Vector3(wallSize8.x, wallSize8.y + 30.0f, wallSize8.z));
//	walls[7]->SetRotation(Vector3(wallAngle8.x, wallAngle8.y - 89.0f, wallAngle8.z));
//	walls[7]->SetPosition(Vector3(wallPos8.x + 1500.0f, wallPos8.y + 35.0f, wallPos8.z + 340.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize9 = walls[8]->GetScale();
//	auto wallAngle9 = walls[8]->GetRotation();
//	auto wallPos9 = walls[8]->GetPosition();
//	walls[8]->SetScale(Vector3(wallSize9.x, wallSize9.y + 30.0f, wallSize9.z));
//	walls[8]->SetRotation(Vector3(wallAngle9.x, wallAngle9.y + 89.0f, wallAngle9.z));
//	walls[8]->SetPosition(Vector3(wallPos9.x + 1200.0f, wallPos9.y + 35.0f, wallPos9.z + 390.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize10 = walls[9]->GetScale();
//	auto wallAngle10 = walls[9]->GetRotation();
//	auto wallPos10 = walls[9]->GetPosition();
//	walls[9]->SetScale(Vector3(wallSize10.x, wallSize10.y + 30.0f, wallSize10.z));
//	walls[9]->SetRotation(Vector3(wallAngle10.x, wallAngle10.y - 89.0f, wallAngle10.z));
//	walls[9]->SetPosition(Vector3(wallPos10.x + 1200.0f, wallPos10.y + 35.0f, wallPos10.z + 350.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize11 = walls[10]->GetScale();
//	auto wallPos11 = walls[10]->GetPosition();
//	auto wallAngle11 = walls[10]->GetRotation();
//	walls[10]->SetScale(Vector3(wallSize11.x, wallSize11.y, wallSize11.z + 150.0f));
//	walls[10]->SetRotation(Vector3(wallAngle11.x, wallAngle11.y + 45.0f, wallAngle11.z));
//	walls[10]->SetPosition(Vector3(wallPos11.x - 100.0f, wallPos11.y, wallPos11.z - 830.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize12 = walls[11]->GetScale();
//	auto wallAngle12 = walls[11]->GetRotation();
//	auto wallPos12 = walls[11]->GetPosition();
//	walls[11]->SetScale(Vector3(wallSize12.x, wallSize12.y, wallSize12.z + 40.0f));
//	walls[11]->SetRotation(Vector3(wallAngle12.x, wallAngle12.y + 45.0f, wallAngle12.z));
//	walls[11]->SetPosition(Vector3(wallPos12.x - 185.0f, wallPos12.y, wallPos12.z - 745.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize13 = walls[12]->GetScale();
//	auto wallAngle13 = walls[12]->GetRotation();
//	auto wallPos13 = walls[12]->GetPosition();
//	walls[12]->SetScale(Vector3(wallSize13.x, wallSize13.y, wallSize13.z - 10.0f));
//	walls[12]->SetRotation(Vector3(wallAngle13.x, wallAngle13.y + 60.0f, wallAngle13.z));
//	walls[12]->SetPosition(Vector3(wallPos13.x - 360.0f, wallPos13.y, wallPos13.z - 1035.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize14 = walls[13]->GetScale();
//	auto wallAngle14 = walls[13]->GetRotation();
//	auto wallPos14 = walls[13]->GetPosition();
//	walls[13]->SetScale(Vector3(wallSize14.x, wallSize14.y, wallSize14.z + 60.0f));
//	walls[13]->SetRotation(Vector3(wallAngle14.x, wallAngle14.y + 60.0f, wallAngle14.z));
//	walls[13]->SetPosition(Vector3(wallPos14.x - 460.0f, wallPos14.y, wallPos14.z - 955.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize15 = walls[14]->GetScale();
//	auto wallAngle15 = walls[14]->GetRotation();
//	auto wallPos15 = walls[14]->GetPosition();
//	walls[14]->SetScale(Vector3(wallSize15.x, wallSize15.y, wallSize15.z - 30.0f));
//	walls[14]->SetRotation(Vector3(wallAngle15.x, wallAngle15.y + 2.0f, wallAngle15.z));
//	walls[14]->SetPosition(Vector3(wallPos15.x - 600.0f, wallPos15.y, wallPos15.z - 1100.0f));
//
//	walls.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Wall>()));
//	auto wallSize16 = walls[15]->GetScale();
//	auto wallAngle16 = walls[15]->GetRotation();
//	auto wallPos16 = walls[15]->GetPosition();
//	walls[15]->SetScale(Vector3(wallSize16.x, wallSize16.y, wallSize16.z + 50.0f));
//	walls[15]->SetRotation(Vector3(wallAngle16.x, wallAngle16.y - 60.0f, wallAngle16.z));
//	walls[15]->SetPosition(Vector3(wallPos16.x - 470.0f, wallPos16.y, wallPos16.z - 1260.0f));
//}
//
//void Stage1Scene::Bouncers() {
//	std::vector<Object*> bouncers;
//	bouncers.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Bouncer>()));
//	auto bouncePos1 = bouncers[0]->GetPosition();
//	bouncers[0]->SetPosition(Vector3(bouncePos1.x - 320.0f, bouncePos1.y, bouncePos1.z - 1150.0f));
//
//	bouncers.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Bouncer>()));
//	auto bouncePos2 = bouncers[1]->GetPosition();
//	bouncers[1]->SetPosition(Vector3(bouncePos2.x - 200.0f, bouncePos2.y, bouncePos2.z - 1250.0f));
//
//	bouncers.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Bouncer>()));
//	auto bouncePos3 = bouncers[2]->GetPosition();
//	bouncers[2]->SetPosition(Vector3(bouncePos3.x - 100.0f, bouncePos3.y, bouncePos3.z - 1180.0f));
//
//	bouncers.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Bouncer>()));
//	auto bouncePos4 = bouncers[3]->GetPosition();
//	bouncers[3]->SetPosition(Vector3(bouncePos4.x + 60.0f, bouncePos4.y, bouncePos4.z - 1240.0f));
//
//	bouncers.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Bouncer>()));
//	auto bouncePos5 = bouncers[4]->GetPosition();
//	bouncers[4]->SetPosition(Vector3(bouncePos5.x + 120.0f, bouncePos5.y, bouncePos5.z - 1180.0f));
//
//	bouncers.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Bouncer>()));
//	auto bouncePos6 = bouncers[5]->GetPosition();
//	bouncers[5]->SetPosition(Vector3(bouncePos6.x + 880.0f, bouncePos6.y, bouncePos6.z - 760.0f));
//
//	bouncers.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Bouncer>()));
//	auto bouncePos7 = bouncers[6]->GetPosition();
//	bouncers[6]->SetPosition(Vector3(bouncePos7.x + 2200.0f, bouncePos7.y, bouncePos7.z - 410.0f));
//
//	bouncers.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Bouncer>()));
//	auto bouncePos8 = bouncers[7]->GetPosition();
//	bouncers[7]->SetPosition(Vector3(bouncePos8.x + 2300.0f, bouncePos8.y, bouncePos8.z - 400.0f));
//
//	bouncers.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Bouncer>()));
//	auto bouncePos9 = bouncers[8]->GetPosition();
//	bouncers[8]->SetPosition(Vector3(bouncePos9.x + 2400.0f, bouncePos9.y, bouncePos9.z - 300.0f));
//
//	bouncers.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<Bouncer>()));
//	auto bouncePos10 = bouncers[9]->GetPosition();
//	bouncers[9]->SetPosition(Vector3(bouncePos10.x + 2500.0f, bouncePos10.y, bouncePos10.z - 280.0f));
//
//}
//
//void Stage1Scene::AccelerationFloors() {
//	std::vector<Object*> acFloors;
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle1 = acFloors[0]->GetRotation();
//	auto acFloorsPos1 = acFloors[0]->GetPosition();
//	acFloors[0]->SetRotation(Vector3(acFloorsAngle1.x, acFloorsAngle1.y + 30.0f, acFloorsAngle1.z));
//	acFloors[0]->SetPosition(Vector3(acFloorsPos1.x + 1470.0f, acFloorsPos1.y, acFloorsPos1.z - 460.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle2 = acFloors[1]->GetRotation();
//	auto acFloorsPos2 = acFloors[1]->GetPosition();
//	acFloors[1]->SetRotation(Vector3(acFloorsAngle2.x, acFloorsAngle2.y - 30.0f, acFloorsAngle2.z));
//	acFloors[1]->SetPosition(Vector3(acFloorsPos2.x + 1800.0f, acFloorsPos2.y, acFloorsPos2.z - 700.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle3 = acFloors[2]->GetRotation();
//	auto acFloorsPos3 = acFloors[2]->GetPosition();
//	acFloors[2]->SetRotation(Vector3(acFloorsAngle3.x, acFloorsAngle3.y - 30.0f, acFloorsAngle3.z));
//	acFloors[2]->SetPosition(Vector3(acFloorsPos3.x + 1900.0f, acFloorsPos3.y, acFloorsPos3.z - 630.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle4 = acFloors[3]->GetRotation();
//	auto acFloorsPos4 = acFloors[3]->GetPosition();
//	acFloors[3]->SetRotation(Vector3(acFloorsAngle4.x, acFloorsAngle4.y - 30.0f, acFloorsAngle4.z));
//	acFloors[3]->SetPosition(Vector3(acFloorsPos4.x + 1950.0f, acFloorsPos4.y, acFloorsPos4.z - 600.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle5 = acFloors[4]->GetRotation();
//	auto acFloorsPos5 = acFloors[4]->GetPosition();
//	acFloors[4]->SetRotation(Vector3(acFloorsAngle5.x, acFloorsAngle5.y - 30.0f, acFloorsAngle5.z));
//	acFloors[4]->SetPosition(Vector3(acFloorsPos5.x + 2000.0f, acFloorsPos5.y, acFloorsPos5.z - 570.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle6 = acFloors[5]->GetRotation();
//	auto acFloorsPos6 = acFloors[5]->GetPosition();
//	acFloors[5]->SetRotation(Vector3(acFloorsAngle6.x, acFloorsAngle6.y - 30.0f, acFloorsAngle6.z));
//	acFloors[5]->SetPosition(Vector3(acFloorsPos6.x + 2050.0f, acFloorsPos6.y, acFloorsPos6.z - 540.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle7 = acFloors[6]->GetRotation();
//	auto acFloorsPos7 = acFloors[6]->GetPosition();
//	acFloors[6]->SetRotation(Vector3(acFloorsAngle7.x, acFloorsAngle7.y - 30.0f, acFloorsAngle7.z));
//	acFloors[6]->SetPosition(Vector3(acFloorsPos7.x + 2100.0f, acFloorsPos7.y, acFloorsPos7.z - 510.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle8 = acFloors[7]->GetRotation();
//	auto acFloorsPos8 = acFloors[7]->GetPosition();
//	acFloors[7]->SetRotation(Vector3(acFloorsAngle8.x, acFloorsAngle8.y - 15.0f, acFloorsAngle8.z));
//	acFloors[7]->SetPosition(Vector3(acFloorsPos8.x + 2300.0f, acFloorsPos8.y, acFloorsPos8.z - 430.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle9 = acFloors[8]->GetRotation();
//	auto acFloorsPos9 = acFloors[8]->GetPosition();
//	acFloors[8]->SetRotation(Vector3(acFloorsAngle9.x, acFloorsAngle9.y - 15.0f, acFloorsAngle9.z));
//	acFloors[8]->SetPosition(Vector3(acFloorsPos9.x + 2350.0f, acFloorsPos9.y, acFloorsPos9.z - 430.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle10 = acFloors[9]->GetRotation();
//	auto acFloorsPos10 = acFloors[9]->GetPosition();
//	acFloors[9]->SetRotation(Vector3(acFloorsAngle10.x, acFloorsAngle10.y - 15.0f, acFloorsAngle10.z));
//	acFloors[9]->SetPosition(Vector3(acFloorsPos10.x + 2400.0f, acFloorsPos10.y, acFloorsPos10.z - 430.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle11 = acFloors[10]->GetRotation();
//	auto acFloorsPos11 = acFloors[10]->GetPosition();
//	acFloors[10]->SetRotation(Vector3(acFloorsAngle11.x, acFloorsAngle11.y - 15.0f, acFloorsAngle11.z));
//	acFloors[10]->SetPosition(Vector3(acFloorsPos11.x + 2450.0f, acFloorsPos11.y, acFloorsPos11.z - 430.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle12 = acFloors[11]->GetRotation();
//	auto acFloorsPos12 = acFloors[11]->GetPosition();
//	acFloors[11]->SetRotation(Vector3(acFloorsAngle12.x, acFloorsAngle12.y - 15.0f, acFloorsAngle12.z));
//	acFloors[11]->SetPosition(Vector3(acFloorsPos12.x + 2500.0f, acFloorsPos12.y, acFloorsPos12.z - 430.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle13 = acFloors[12]->GetRotation();
//	auto acFloorsPos13 = acFloors[12]->GetPosition();
//	acFloors[12]->SetRotation(Vector3(acFloorsAngle13.x, acFloorsAngle13.y + 180.0f, acFloorsAngle13.z));
//	acFloors[12]->SetPosition(Vector3(acFloorsPos13.x + 2000.0f, acFloorsPos13.y - 10.0f, acFloorsPos13.z + 370.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle14 = acFloors[13]->GetRotation();
//	auto acFloorsPos14 = acFloors[13]->GetPosition();
//	acFloors[13]->SetRotation(Vector3(acFloorsAngle14.x, acFloorsAngle14.y + 180.0f, acFloorsAngle14.z));
//	acFloors[13]->SetPosition(Vector3(acFloorsPos14.x + 1800.0f, acFloorsPos14.y - 10.0f, acFloorsPos14.z + 370.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle15 = acFloors[14]->GetRotation();
//	auto acFloorsPos15 = acFloors[14]->GetPosition();
//	acFloors[14]->SetRotation(Vector3(acFloorsAngle15.x, acFloorsAngle15.y + 180.0f, acFloorsAngle15.z));
//	acFloors[14]->SetPosition(Vector3(acFloorsPos15.x + 1600.0f, acFloorsPos15.y - 10.0f, acFloorsPos15.z + 370.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle16 = acFloors[15]->GetRotation();
//	auto acFloorsPos16 = acFloors[15]->GetPosition();
//	acFloors[15]->SetRotation(Vector3(acFloorsAngle16.x, acFloorsAngle16.y + 180.0f, acFloorsAngle16.z));
//	acFloors[15]->SetPosition(Vector3(acFloorsPos16.x + 1400.0f, acFloorsPos16.y - 10.0f, acFloorsPos16.z + 370.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle17 = acFloors[16]->GetRotation();
//	auto acFloorsPos17 = acFloors[16]->GetPosition();
//	acFloors[16]->SetRotation(Vector3(acFloorsAngle17.x, acFloorsAngle17.y + 180.0f, acFloorsAngle17.z));
//	acFloors[16]->SetPosition(Vector3(acFloorsPos17.x + 1200.0f, acFloorsPos17.y - 10.0f, acFloorsPos17.z + 370.0f));
//
//	acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	auto acFloorsAngle18 = acFloors[17]->GetRotation();
//	auto acFloorsPos18 = acFloors[17]->GetPosition();
//	acFloors[17]->SetRotation(Vector3(acFloorsAngle18.x, acFloorsAngle18.y + 180.0f, acFloorsAngle18.z));
//	acFloors[17]->SetPosition(Vector3(acFloorsPos18.x + 1000.0f, acFloorsPos18.y - 10.0f, acFloorsPos18.z + 370.0f));
//
//	//acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	//auto acFloorsAngle19 = acFloors[18]->GetRotaiton();
//	//auto acFloorsPos19 = acFloors[18]->GetPosition();
//	//acFloors[18]->SetRotation(Vector3(acFloorsAngle19.x, acFloorsAngle19.y + 180.0f, acFloorsAngle19.z));
//	//acFloors[18]->SetPosition(Vector3(acFloorsPos19.x + 800.0f, acFloorsPos19.y - 10.0f, acFloorsPos19.z + 370.0f));
//
//	//acFloors.emplace_back(m_MySceneObjects.emplace_back(Game::GetInstance()->AddObject<AccelerationFloor>()));
//	//auto acFloorsAngle20 = acFloors[19]->GetRotaiton();
//	//auto acFloorsPos20 = acFloors[19]->GetPosition();
//	//acFloors[19]->SetRotation(Vector3(acFloorsAngle20.x, acFloorsAngle20.y + 180.0f, acFloorsAngle20.z));
//	//acFloors[19]->SetPosition(Vector3(acFloorsPos20.x + 600.0f, acFloorsPos20.y - 10.0f, acFloorsPos20.z + 370.0f));
//}