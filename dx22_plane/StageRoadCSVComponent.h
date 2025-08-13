#pragma once
#include "Component.h"
#include <iostream>
#include <string>

class StageRoadCSVComponent :  public Component
{
private:

public:
	StageRoadCSVComponent(GameObject& obj);
	~StageRoadCSVComponent() = default;

	void Update() override;

	void RoadStageCSV(const std::string& fileName,GameObject& blockContainer); // ステージのCSVを読み込む関数
};
