#include "TerrainJsonComponent.h"

using json = nlohmann::ordered_json;
using namespace DirectX;
using namespace std::filesystem;

TerrainJsonComponent::TerrainJsonComponent(GameObject& obj) :JsonComponent(obj) {
    m_SortNum = ComponentTypeManager::GetID_FromName("JSON"); // ソート番号を設定
}

void TerrainJsonComponent::Update() {

}

void TerrainJsonComponent::MakeSampleStatus() {
    std::vector<TerrainStatus> terrains = {
        { "T_Block" , {5.0f,5.0f,5.0f}, { 0.0f,0.0f,0.0f },"assets/texture/NoTexture.png","ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl"},
        { "T_dice" ,  {5.0f,10.0f,5.0f}, { 0.0f,0.0f,0.0f },"assets/texture/dice.png","ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl"},
    };

	m_terrainStatus = terrains;

    for(auto& t : m_terrainStatus) {
		m_kindNames.push_back(t.kind); // 地形の種類をベクターに追加
    }

    json j = TerrainsToJson(terrains);
    // 保存先のファイルパス
    std::string filepath = "json/terrain.json";
    // JSONをファイルに保存
    if (SaveJsonToFile(j, filepath)) {
        std::cout << "Terrain JSON saved successfully to " << filepath << std::endl;
    } else {
        std::cerr << "Failed to save Terrain JSON." << std::endl;
	}
}

// 複数 TerrainStatus を JSON に変換
json TerrainJsonComponent::TerrainsToJson(const std::vector<TerrainStatus>& terrains) {
    json j;
    j["terrains"] = json::array();
    for (const auto& t : terrains) {
        json item;
        item["kind"] = t.kind;
        item["scale"] = XMFLOAT3ToJson(t.scale);
        item["angle"] = XMFLOAT3ToJson(t.angle);
        item["texture"] = t.texture;
        item["shaderVS"] = t.shaderVS;
		item["shaderPS"] = t.shaderPS;
        j["terrains"].push_back(item);
    }
    return j;
}

std::vector<TerrainStatus> TerrainJsonComponent::LoadTerrainJsonFile(const std::string& filepath) {
    std::ifstream ifs(filepath);
    if (!ifs) {
        std::cerr << "ファイルを開けません: " << filepath << "\n";
        return {};
    }

    nlohmann::json j;
    ifs >> j;

    std::vector<TerrainStatus> terrains;
    for (const auto& item : j.at("terrains")) {
        TerrainStatus t;
        t.kind = item.at("kind");
        t.scale = JsonToVector3(item.at("scale"));
        t.angle = JsonToVector3(item.at("angle"));
        t.texture = item.at("texture");
        t.shaderVS = item.at("shaderVS");
        t.shaderPS = item.at("shaderPS");
        terrains.push_back(t);
    }

    m_terrainStatus = terrains;

    for (auto& t : m_terrainStatus) {
        m_kindNames.push_back(t.kind); // 地形の種類をベクターに追加
    }

    return terrains;
}
