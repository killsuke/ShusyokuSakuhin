#pragma once
#include "Scene.h"
#include "GameObject/GameObject.h"
//#include "Texture2D.h"
#include <chrono>

// ResultSceneクラス
class ResultScene : public Scene
{
private:

public:
	ResultScene(); // コンストラクタ
	~ResultScene(); // デストラクタ

	void Update(); // 更新
};

