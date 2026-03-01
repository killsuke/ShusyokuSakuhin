// ------------------------
// ここでは地形生成と
// 必要な地形のみを当たり判定とる？
// ------------------------

#pragma once
#include "CSVObjectManagerComponent.h"
#include "Structs/CSV_Data.h"
#include "Structs/ObjectStatus.h"
#include <vector>
#include <string>
#include <utility>

class TerrainCreateComponent final : public CSVObjectManagerComponent
{
public:
	TerrainCreateComponent(GameObject& obj);
	~TerrainCreateComponent() = default;

	void Update() override;
	void CreateTerrains(std::vector<TerrainStatus> status,const float centerZ); // 地形を生成する関数
};