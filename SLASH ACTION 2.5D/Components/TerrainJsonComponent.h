#pragma once
#include "JsonComponent.h"

class TerrainJsonComponent : public JsonComponent
{
private:
	std::vector<TerrainStatus> m_terrainStatus;	// 地形情報のベクター
public:
	TerrainJsonComponent(GameObject& obj);
	~TerrainJsonComponent() = default;

	void Update() override;	// 更新処理

	void MakeSampleStatus() override;	// サンプルを作成

	nlohmann::ordered_json  TerrainsToJson(const std::vector<TerrainStatus>& terrains);
	std::vector<TerrainStatus> LoadTerrainJsonFile(const std::string& filepath);


	inline std::vector<TerrainStatus> GetTerrainStatus() const { return m_terrainStatus; } // 地形情報の取得
};
