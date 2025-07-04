#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "sound.h"
#include <chrono>

// ResultSceneクラス
class ResultScene : public Scene
{
private:
	std::vector<GameObject*> m_MySceneObjects; // このシーンのオブジェクト

	Texture2D* texTime1 = nullptr;	// 小数点のタイム計測
	Texture2D* texTime2 = nullptr;	// 小数点のタイム計測
	Texture2D* texTime3 = nullptr;	// 小数点のタイム計測
	Texture2D* texTime4 = nullptr;	// 小数点のタイム計測
	Texture2D* texTime5 = nullptr;	// 小数点のタイム計測
	Texture2D* texTime6 = nullptr;	// 小数点のタイム計測

	Texture2D* texRank = nullptr;	// 小数点のタイム計測

	std::chrono::high_resolution_clock::time_point startTime;	// スタートs
	//Sound& soundResult = Sound::GetInstance();

	int resultCount = 0;

	void Init(); // 初期化
	void Uninit(); // 終了処理

public:
	ResultScene(); // コンストラクタ
	~ResultScene(); // デストラクタ

	void Update(); // 更新

	void SetScore(int c);	// スコアを設定
};

