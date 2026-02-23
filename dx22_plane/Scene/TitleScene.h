#pragma once
#include "Scene.h"
#include "GameObject/GameObject.h"

// TitleSceneクラス
class TitleScene final : public Scene
{
private:
	void Init(); // 初期化
	void Uninit(); // 終了処理
	float m_RecordTime = 0.0f; // タイトルシーンの経過時間

public:
	TitleScene(); // コンストラクタ
	~TitleScene(); // デストラクタ

	void Update(); // 更新
};

