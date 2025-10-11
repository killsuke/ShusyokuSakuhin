#pragma once
#include "GameObjectManager.h"
#include "sound.h"
#include "Input.h"
#include "Scene.h"
#include "SceneList.h"
#include <memory>
#include "sound.h"

class SceneManager
{
public:

	SceneManager() = default;	// コンストラクタ
	~SceneManager() = default;			// デストラクタ
	template <typename T1>
	static void SceneChange() {	// sceneの値で現在処理するべきシーンへと変更する
		static_assert(std::is_base_of<Scene, T1>::value, "T1 must be derived from Scene");
		{
			sceneChangeFg = true;	// シーンチェンジのフラグを立てる
			m_pScene.reset();	// 現在のシーンを片付ける
		}
		m_pScene = std::make_unique<T1>();	// シーンを変更
	};

	// ちょっと待ってからシーン遷移するように組む？
	/*template <typename T2>
	static void WaitSceneChange(const float time) {

		if (waitTimeCounter > waitTime) {
			waitTimeCounter = 0.0f;
			waitTime = 0.0f;
			SceneChange<T2>();
		}
	};*/

	static void Init();					// シーンの初期化
	static void Update();					// 現在のシーンの更新
	static void Draw();					// 現在のシーンの描画
	static void UnInit();					// シーンの片付け

	// シーンチェンジのフラグ管理
	static void SetSCFrag(const bool _sceneChangeFg) { sceneChangeFg = _sceneChangeFg; };
	static bool GetSCFrag() { return sceneChangeFg; };
	
	static Sound GetSound() { return sound; };

private:
	static std::unique_ptr<Scene> m_pScene;		// 現在のシーン
	//Input input = Input::GetInstance();	// 入力処理のインスタンスを取得
	//Sound* sound = nullptr;	// サウンド用のインスタンス
	static bool sceneChangeFg;	// シーンチェンジが起こったのかのフラグ
	static float waitTime;	// シーン遷移を少し待つ
	static float waitTimeCounter;
	inline static Sound sound; // サウンド

};