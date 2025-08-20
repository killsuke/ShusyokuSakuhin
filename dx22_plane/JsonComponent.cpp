#include "JsonComponent.h"
using json = nlohmann::ordered_json;
using namespace DirectX::SimpleMath;
using namespace std::filesystem;

JsonComponent::JsonComponent(GameObject& obj) : Component(obj) {
    m_sortNum = ComponentTypeManager::GetID_FromName("JSON"); // ソート番号を設定
}

void JsonComponent::Update() {


}

void JsonComponent::MakeSampleStatus() {

}

// Vector3 → JSON
json JsonComponent::Vector3ToJson(const Vector3& v) {
    return { {"x", v.x}, {"y", v.y}, {"z", v.z} };
}

// JSON → Vector3
Vector3 JsonComponent::JsonToVector3(const nlohmann::ordered_json& j) {
    return Vector3(j.at("x"), j.at("y"), j.at("z"));
}

bool JsonComponent::SaveJsonToFile(const nlohmann::ordered_json& j, const std::string& filepath) {

    // ディレクトリ部分を作成（ファイル名を除いたパス）
    path _path(filepath);
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