#pragma once
#include "Component.h"
#include "ObjectStatus.h"
#include <string>
#include <vector>
#include <json.hpp>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <SimpleMath.h>

class JsonComponent : public Component
{
protected:
	JsonComponent(GameObject& obj);
	~JsonComponent() = default;

	std::vector<std::string> m_kindNames;	// 種類名を格納するベクター

private:

public:

	void Update()override;

	inline std::vector<std::string> GetKindNames() const { return m_kindNames; } // 地形の種類の取得

	// Vector3 → JSON
	nlohmann::ordered_json  Vector3ToJson(const DirectX::SimpleMath::Vector3& v);

	// JSON → Vector3
	DirectX::SimpleMath::Vector3 JsonToVector3(const nlohmann::ordered_json& j);

	bool SaveJsonToFile(const nlohmann::ordered_json& j, const std::string& filepath);

	virtual void MakeSampleStatus();	// サンプルを作成
};
