// ------------------------
// ここでは地形生成と
// 必要な地形のみを当たり判定とる？
// ------------------------

#pragma once
#include "Component.h"
#include "CSV_Data.h"
#include "ObjectStatus.h"
#include <vector>
#include <string>
#include <utility>

class TerrainManagerComponent : public Component
{
private:
	std::vector<CSV_Data> m_terrainData; // 地形データを格納する2次元ベクター
public:
	TerrainManagerComponent(GameObject& obj);
	~TerrainManagerComponent() = default;

	void Update() override;
	void CreateTerrain(std::vector<TerrainStatus> status); // 地形を生成する関数

	inline void SetTerrainData(std::vector<CSV_Data>&& data) {
		m_terrainData = std::move(data); // 地形データをセット
	}
};
