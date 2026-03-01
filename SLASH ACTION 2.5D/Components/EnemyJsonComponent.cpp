#include "EnemyJsonComponent.h"

using json = nlohmann::ordered_json;
using namespace std::filesystem;

EnemyJsonComponent::EnemyJsonComponent(GameObject& obj) : JsonComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("JSON"); // ソート番号を設定
}

void EnemyJsonComponent::Update() {


}

void EnemyJsonComponent::MakeSampleStatus() {
	std::vector<EnemyStatus> enemies = {
	{ "E_NoMove" , {5.0f,5.0f,5.0f}, { 0.0f,0.0f,0.0f },10,10,"NoTexture.png","ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl"},
	{ "E_dice" ,  {5.0f,10.0f,5.0f}, { 0.0f,0.0f,0.0f },10,10,"dice.png","ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl"},
	};

	m_enemyStatus = enemies;

	for (auto& e : m_enemyStatus) {
		m_kindNames.push_back(e.kind); // 地形の種類をベクターに追加
	}

	json j = EnemyToJson(m_enemyStatus);
	// 保存先のファイルパス
	std::string filepath = "json/enemy.json";
	// JSONをファイルに保存
	if (SaveJsonToFile(j, filepath)) {
		std::cout << "Enemy JSON saved successfully to " << filepath << std::endl;
	}
	else {
		std::cerr << "Failed to save Enemy JSON." << std::endl;
	}
}

// 複数 EnemyStatus を JSON に変換
json EnemyJsonComponent::EnemyToJson(const std::vector<EnemyStatus>& enemies) {
	json j;
	j["enemies"] = json::array();
	for (const auto& e : enemies) {
		json item;
		item["kind"] = e.kind;
		item["scale"] = XMFLOAT3ToJson(e.scale);
		item["angle"] = XMFLOAT3ToJson(e.angle);
		item["atk"] = e.atk;
		item["hp"] = e.hp;
		item["texture"] = e.texture;
		item["shaderVS"] = e.shaderVS;
		item["shaderPS"] = e.shaderPS;
		j["enemies"].push_back(item);
	}
	return j;
}

std::vector<EnemyStatus> EnemyJsonComponent::LoadEnemyJsonFile(const std::string& filepath) {
	std::ifstream ifs(filepath);
	if (!ifs) {
		std::cerr << "ファイルを開けません: " << filepath << "\n";
		return {};
	}

	nlohmann::json j;
	ifs >> j;

	std::vector<EnemyStatus> enemies;
	for (const auto& item : j.at("enemies")) {
		EnemyStatus e;
		e.kind = item.at("kind");
		e.scale = JsonToVector3(item.at("scale"));
		e.angle = JsonToVector3(item.at("angle"));
		e.atk = item.at("atk");
		e.hp = item.at("hp");
		e.texture = item.at("texture");
		e.shaderVS = item.at("shaderVS");
		e.shaderPS = item.at("shaderPS");
		enemies.push_back(e);
	}

	m_enemyStatus = enemies;

	for (auto& e : m_enemyStatus) {
		m_kindNames.push_back(e.kind); // 地形の種類をベクターに追加
	}

	return enemies;
}