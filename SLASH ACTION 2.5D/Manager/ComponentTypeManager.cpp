#include "ComponentTypeManager.h"
using json = nlohmann::ordered_json;
using namespace std::filesystem;


std::unordered_map<std::string, uint32_t> ComponentTypeManager::nameToId;
std::unordered_map<uint32_t, std::string> ComponentTypeManager::idToName;

void ComponentTypeManager::Init() {
	// 初期化処理
	nameToId.clear();
	idToName.clear();
}

void ComponentTypeManager::UnInit() {
	// 後始末処理
	nameToId.clear();
	idToName.clear();
}

void ComponentTypeManager::Update() {
	// 更新処理（必要に応じて実装）
}

void ComponentTypeManager::MakeSampleJson() {
	std::unordered_map<std::string, uint32_t> components = {
		{ "BASECOMPONENT" ,				0},
		{ "STAGE_LOAD_CSV" ,			1},
		{ "JSON" ,						2},
		{ "CSV_OBJECT_MANAGER" ,		3},
		{ "TEST_MOVE" ,				4},
		{ "JUMP" ,					5},
		{ "SPRING" ,				6},
		{ "RIGIDBODY" ,				7},
		{ "CAMERA_TARGET" ,				8},
		{ "CAMERA_POINT" ,				9},
		{ "CAMERA_MOVE" ,			10},
		{ "CAMERA" ,				11},
		{ "BONE" ,					12},
		{ "GO_AROUND" ,			13},
		{ "HP_BAR_MOVE" ,			14},
		{ "COLLIDER" ,				15},
		{ "TEST_JUDGE_EXTRUSION" ,		16},
		{ "ATTACK" ,				17},
		{ "ENEMY_DAMAGE" ,			18},
		{ "FIGHTER" ,				19},
		{ "TRANSFORM" ,				20},
		{ "RENDER" ,				21},
		{ "RENDER_ONE_SKIN_ANIMATION" , 22},
		{ "RENDER_DEBUG" ,			23},
		{ "MAX" ,					24},
	};

	nameToId = components;

	for (const std::pair<const std::string, uint32_t>& comp : nameToId) {
		idToName[comp.second] = comp.first;
	}

	json j = ComponentTypeNameToJson(nameToId);
	// 保存先のファイルパス
	const std::string filepath = "json/component.json";
	// JSONをファイルに保存
	if (SaveJsonToFile(j, filepath)) {
		std::cout << "Component JSON saved successfully to " << filepath << std::endl;
	}
	else {
		std::cerr << "Failed to save Component JSON." << std::endl;
	}
}

void ComponentTypeManager::LoadComponentTypeJsonFile(const std::string& filepath) {
	std::ifstream ifs(filepath);
	if (!ifs) {
		std::cerr << "ファイルを開けません: " << filepath << "\n";
		return;
	}

	nlohmann::json j;
	ifs >> j;

	for (const nlohmann::json& comp : j["components"]) {
		std::string compName = comp.at("name");
		uint32_t id = comp.at("ID");
		nameToId[compName] = id; // 名前とIDを紐付け
		idToName[id] = compName; // IDと名前を紐付け
	}
}

json ComponentTypeManager::ComponentTypeNameToJson(const std::unordered_map<std::string, uint32_t>& comps) {
	json j;
	j["components"] = json::array();

	std::vector<std::pair<std::string, uint32_t>> sorted(comps.begin(), comps.end());
	std::sort(sorted.begin(), sorted.end(), [](const std::pair<std::string, uint32_t>& a, const std::pair<std::string, uint32_t>& b) {
		return a.second < b.second; // 名前に変えるのも可
		});

	for (const std::pair<std::string, uint32_t>& c : sorted) {
		json comp;
		comp["name"] = c.first;
		comp["ID"] = c.second; // IDを追加
		j["components"].push_back(comp);
	}
	return j;
}

bool ComponentTypeManager::SaveJsonToFile(const nlohmann::ordered_json& j, const std::string& filepath) {

	// ディレクトリ部分を作成（ファイル名を除いたパス）
	const path _path(filepath);
	if (!_path.parent_path().empty()) {
		std::error_code ec;
		create_directories(_path.parent_path(), ec);
		if (ec) {
			std::cerr << "ディレクトリ作成失敗：" << ec.message() << std::endl;
			return false;
		}
	}

	std::ofstream ofs(filepath, std::ios::out | std::ios::trunc);
	if (!ofs) {
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