#pragma once
#include "GameObjectManager.h"
#include "EventBusManager.h"
#include "Input/Input.h"
#include "Scene/Scene.h"
#include "Scene/SceneList.h"
#include <memory>

class SceneManager final
{
private:
	static inline std::unique_ptr<Scene> m_Scene = nullptr;		// 現在のシーン
	static inline bool m_IsSceneChange = false;					// シーンチェンジが起こったのかのフラグ
	static inline float m_WaitTime = 0.0f;						// シーン遷移を少し待つ
	static inline float m_WaitTimeCounter = 0.0f;				// シーン遷移を少し待つためのカウンター

	// コンストラクタ・デストラクタを削除
	SceneManager() = delete;
	~SceneManager() = delete;

	// コピー・ムーブも削除
	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

public:

	template <typename T1>
	static void SceneChange() {	// sceneの値で現在処理するべきシーンへと変更する
		static_assert(std::is_base_of<Scene, T1>::value, "T1 must be derived from Scene");
		{
			EventBusManager::UnInit();	// イベントバスマネージャーの片付け
			EventBusManager::Init();		// イベントバスマネージャーの初期化

			m_IsSceneChange = true;	// シーンチェンジのフラグを立てる
			m_Scene.reset();	// 現在のシーンを片付ける
		}
		m_Scene = std::make_unique<T1>();	// シーンを変更
	};
	
	static void Init();					// シーンの初期化
	static void Update();					// 現在のシーンの更新
	static void Draw();					// 現在のシーンの描画
	static void UnInit();					// シーンの片付け

	// シーンチェンジのフラグ管理
	static void SetSCFrag(const bool _sceneChangeFg) { m_IsSceneChange = _sceneChangeFg; };
	static bool GetSCFrag() { return m_IsSceneChange; };
	
	static Scene* GetScene() { return m_Scene.get(); };
};