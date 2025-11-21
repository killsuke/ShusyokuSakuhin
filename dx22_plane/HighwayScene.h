#pragma once
#include "Scene.h"
class HighwayScene : public Scene
{
private:

public:
	HighwayScene(); // コンストラクタ
	~HighwayScene(); // デストラクタ

	void Update(); // 更新

	void TerrainsInFront();
	void TerrainsCenter();
	void TerrainsBack();
	void Enemeis();
};