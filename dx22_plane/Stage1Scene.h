#pragma once
#include "Scene.h"
#include "Object.h"
#include "Texture2D.h"
#include <chrono>
#include "sound.h"

// Stage1Scenクラス
class Stage1Scene : public Scene
{
private:
	std::vector<Object*> m_MySceneObjects; // このシーンのオブジェクト

	int m_State = 0;	// 状態　０：ボール移動中、１：方向選択中、２：パワー選択中

	void Init(); // 初期化
	void Uninit(); // 終了処理
	
	int m_Par; // パー（標準打数）
	int m_StrokeCount;	// 現在の打数

	static int countDown;	// カウントダウン
	int onceStop = 0;	// カウントダウンの回転と縮小を一旦止める
	float texAngleCount = 0.0f;

	Texture2D* texOne = nullptr;
	Texture2D* texTen = nullptr;
	Object* car = nullptr;
	Texture2D* texTime1 = nullptr;	// 小数点のタイム計測
	Texture2D* texTime2 = nullptr;	// 小数点のタイム計測
	Texture2D* texTime3 = nullptr;	// 小数点のタイム計測
	Texture2D* texTime4 = nullptr;	// 小数点のタイム計測
	Texture2D* texTime5 = nullptr;	// 小数点のタイム計測
	Texture2D* texTime6 = nullptr;	// 小数点のタイム計測

	Texture2D* texCounts = nullptr;

	//std::chrono::high_resolution_clock nowTime;		// 現在時刻
	std::chrono::high_resolution_clock::time_point startTime;	// スタート

	Sound& soundStage = Sound::GetInstance();
public:
	Stage1Scene(); // コンストラクタ
	~Stage1Scene(); // デストラクタ

	void Update(); // 更新

	int GetScore(); // スコアを取得

	static int GetCountDown() { return countDown; };
};

