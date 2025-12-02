#pragma once
#include <memory>

//Scene クラス
class Scene
{
protected:
	Scene() = default; // コンストラクタ

public:

	virtual ~Scene() = default; // デストラクタ(virtualが必要)

	virtual void Update() = 0; // 更新（純粋仮想関数）
};
