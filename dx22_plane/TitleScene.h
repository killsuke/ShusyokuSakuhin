#pragma once
#include "Scene.h"
#include "Object.h"
#include "sound.h"

// TitleSceneクラス
class TitleScene : public Scene
{
private:
	std::vector<Object*> m_MySceneObjects; // このシーンのオブジェクト
	Sound& soundTitle = Sound::GetInstance();
	bool setumeiFg = false;
	void Init(); // 初期化
	void Uninit(); // 終了処理

public:
	TitleScene(); // コンストラクタ
	~TitleScene(); // デストラクタ

	void Update(); // 更新
};

