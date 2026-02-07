#pragma once
#include "Scene.h"
#include "GameObject/GameObject.h"
#include <chrono>

// Stage1Scenクラス
class Stage1Scene : public Scene
{
private:
	void Init(); // 初期化
	void Uninit(); // 終了処理
	uint32_t m_testID = 0;
	
public:
	Stage1Scene(); // コンストラクタ
	~Stage1Scene(); // デストラクタ

	void Update(); // 更新

	void BonePartTest();
	void BoneToPartTest();

	void TestBlur();
};