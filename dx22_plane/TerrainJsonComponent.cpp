#include "TerrainJsonComponent.h"
#include <filesystem>
#include <iostream>

using json = nlohmann::ordered_json;
using namespace DirectX::SimpleMath;
using namespace std::filesystem;

TerrainJsonComponent::TerrainJsonComponent(GameObject& obj) :Component(obj) {
	m_sortNum = TERRAIN_JSON; // ソート番号を設定
}

void TerrainJsonComponent::Update() {

}

void TerrainJsonComponent::MakeSampleTerrainStatus() {
    std::vector<TerrainStatus> terrains = {
        { "Block" , {5.0f,5.0f,5.0f}, { 0.0f,0.0f,0.0f },"assets/texture/NoTexture.png","shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl"},
        { "dice" ,  {5.0f,10.0f,5.0f}, { 0.0f,0.0f,0.0f },"assets/texture/dice.png","shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl"},
    };

	m_terrainStatus = terrains;

    for(auto& t : m_terrainStatus) {
		m_terrainKinds.push_back(t.kind); // 地形の種類をベクターに追加
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
        item["scale"] = Vector3ToJson(t.scale);
        item["angle"] = Vector3ToJson(t.angle);
        item["texture"] = t.texture;
        item["shaderVS"] = t.shaderVS;
		item["shaderPS"] = t.shaderPS;
        j["terrains"].push_back(item);
    }
    return j;
}


std::vector<TerrainStatus> TerrainJsonComponent::LoadJsonFile(const std::string& filepath) {
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
        m_terrainKinds.push_back(t.kind); // 地形の種類をベクターに追加
    }

    return terrains;
}

// Vector3 → JSON
json TerrainJsonComponent::Vector3ToJson(const DirectX::SimpleMath::Vector3& v) {
    return { {"x", v.x}, {"y", v.y}, {"z", v.z} };
}

// JSON → Vector3
DirectX::SimpleMath::Vector3 TerrainJsonComponent::JsonToVector3(const nlohmann::ordered_json& j) {
    return DirectX::SimpleMath::Vector3(j.at("x"), j.at("y"), j.at("z"));
}

bool TerrainJsonComponent::SaveJsonToFile(const nlohmann::ordered_json& j, const std::string& filepath) {

    // ディレクトリ部分を作成（ファイル名を除いたパス）
    path _pash(filepath);
    if (!_pash.parent_path().empty()) {
		std::error_code ec;
        create_directories(_pash.parent_path(), ec);
        if (ec) {
            std::cerr << "ディレクトリ作成失敗：" << ec.message() << std::endl;
            return false;
		}
    }

    std::ofstream ofs(filepath, std::ios::out | std::ios::trunc);
    if(!ofs) {
        std::cerr << "ファイルを開けませんでした：" << filepath << std::endl;
        return false;
	}

	// JSONを書き込み
    ofs << j.dump(4); // 4はインデントのスペース数
    if (!ofs) {
        std::cerr << "ファイル書き込み失敗：" << filepath << std::endl;
        return false;
    }
    ofs.close();
	return true;
}