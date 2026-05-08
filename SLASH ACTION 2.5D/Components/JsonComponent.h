#pragma once
#include "Component.h"
#include "Structs/ObjectStatus.h"
#include <string>
#include <vector>
#include <json.hpp>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <DirectXMath.h>

class JsonComponent : public Component
{
protected:
	JsonComponent(GameObject& obj);
	~JsonComponent() = default;

	std::vector<std::string> m_KindNames;	// 種類名を格納するベクター

private:

public:

	void Update()override;

	std::vector<std::string> GetKindNames() const { return m_KindNames; } // 地形の種類の取得

	// Vector3 → JSON
	nlohmann::ordered_json  XMFLOAT3ToJson(const DirectX::XMFLOAT3& v);

	// JSON → Vector3
	DirectX::XMFLOAT3 JsonToVector3(const nlohmann::ordered_json& j);

	bool SaveJsonToFile(const nlohmann::ordered_json& j, const std::string& filepath);

	virtual void MakeSampleStatus();	// サンプルを作成
};
