#pragma once
#include "GameObjectManager.h"
#include "sound.h"
#include "Input.h"
#include "Scene.h"
#include "SceneList.h"

// シーンの列挙型
enum SCENES {
	TITLE,
	GAME,
	RESULT
};

class SceneManager
{
public:
	
	SceneManager();	// コンストラクタ
	~SceneManager();			// デストラクタ
	static void SceneChange(SCENES scene);	// sceneの値で現在処理するべきシーンへと変更する
	void Update();					// 現在のシーンの更新
	void Draw();					// 現在のシーンの描画
	// シーンチェンジのフラグ管理
	static void SetSCFrag(const bool _sceneChangeFg) { sceneChangeFg = _sceneChangeFg; };
	static bool GetSCFrag() { return sceneChangeFg; };
	static SCENES& GetNowScene() { return nowScene; };	// 現在のシーンを返す

private:
	static Scene* m_pScene;		// 現在のシーン（形だけの定義）
	//Input input = Input::GetInstance();	// 入力処理のインスタンスを取得
	Sound* sound = nullptr;	// サウンド用のインスタンス
	static bool sceneChangeFg;	// シーンチェンジが起こったのかのフラグ
	static SCENES nowScene;
};