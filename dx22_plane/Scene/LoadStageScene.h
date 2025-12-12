#pragma once
#include "Scene.h"
class LoadStageScene : public Scene
{
private:

public:
	LoadStageScene();
	~LoadStageScene();

	void Update();

	void TargetAndScroolCreate();
	void BamboosCreate();
	void BackGroundCreate();
};
