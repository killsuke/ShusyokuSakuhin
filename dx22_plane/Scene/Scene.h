#pragma once
#include <memory>

//Scene クラス
class Scene
{
protected:
	Scene() = default; // コンストラクタ
	bool m_IsSceneActive = true;

public:

	virtual ~Scene() = default; // デストラクタ(virtualが必要)

	virtual void Update() = 0; // 更新（純粋仮想関数）
	void SetSceneActive(bool active) { m_IsSceneActive = active; }
	bool GetSceneActive()const { return m_IsSceneActive; };
};
