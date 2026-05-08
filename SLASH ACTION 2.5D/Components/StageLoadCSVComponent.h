#pragma once
#include "Component.h"
#include <iostream>
#include <string>
#include <vector>

class StageLoadCSVComponent :  public Component
{
public:
	StageLoadCSVComponent(GameObject& obj);
	~StageLoadCSVComponent() = default;

	void Update() override;

	void LoadStageCSV(const std::string& fileName,GameObject& terrainManager); // ステージのCSVを読み込む関数
	void WriteStageCSV(const std::string& fileName); // ステージのCSVを書き出す関数
	void CSVToBinary(const std::string& csvFileName, const std::string& binaryFileName); // CSVをバイナリに変換する関数
};
