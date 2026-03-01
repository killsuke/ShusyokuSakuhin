#pragma once
#include "Scene.h"

class LoadStageScene final : public Scene
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
