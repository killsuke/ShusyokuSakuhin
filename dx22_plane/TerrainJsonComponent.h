#pragma once
#include "Component.h"
#include "ObjectStatus.h"
#include <string>
#include <vector>
#include <json.hpp>
#include <fstream>

class TerrainJsonComponent :  public Component
{
private:
	std::vector<TerrainStatus> m_terrainStatus;	// 地形情報のベクター
	std::vector<std::string> m_terrainKinds;	// 地形の種類を格納するベクター
public:
	TerrainJsonComponent(GameObject& obj);
	~TerrainJsonComponent() = default;

	void Update() override;	// 更新処理

	void MakeSampleTerrainStatus();	// サンプルの地形情報を作成

	nlohmann::ordered_json  TerrainsToJson(const std::vector<TerrainStatus>& terrains);
	std::vector<TerrainStatus> LoadJsonFile(const std::string& filepath);

	// Vector3 → JSON
	nlohmann::ordered_json  Vector3ToJson(const DirectX::SimpleMath::Vector3& v);

	// JSON → Vector3
	DirectX::SimpleMath::Vector3 JsonToVector3(const nlohmann::ordered_json& j);

	bool SaveJsonToFile(const nlohmann::ordered_json& j, const std::string& filepath);

	inline std::vector<TerrainStatus> GetTerrainStatus() const { return m_terrainStatus; } // 地形情報の取得
	inline std::vector<std::string> GetTerrainKinds() const { return m_terrainKinds; } // 地形の種類の取得
};
