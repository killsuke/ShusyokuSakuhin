#pragma once
#include "Component.h"
#include <iostream>
#include <string>

class StageLoadCSVComponent :  public Component
{
private:

public:
	StageLoadCSVComponent(GameObject& obj);
	~StageLoadCSVComponent() = default;

	void Update() override;

	void LoadStageCSV(const std::string& fileName,GameObject& terrainManager); // ステージのCSVを読み込む関数
};
