#pragma once
#include <unordered_map>
#include <string>
#include <cstdint>
#include <optional>
#include <json.hpp>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <vector>
#include <Windows.h>
#include "Helper/StringHelper.h"

class ComponentTypeManager final
{
private:
	static std::unordered_map<std::string, uint32_t> nameToId;
	static std::unordered_map<uint32_t, std::string> idToName;

	// コンストラクタ・デストラクタを削除
	ComponentTypeManager() = delete;
	~ComponentTypeManager() = delete;

	// コピー・ムーブも削除
	ComponentTypeManager(const ComponentTypeManager&) = delete;
	ComponentTypeManager(ComponentTypeManager&&) = delete;
	ComponentTypeManager& operator=(const ComponentTypeManager&) = delete;
	ComponentTypeManager& operator=(ComponentTypeManager&&) = delete;

public:

	static void Init();
	static void UnInit();
	
	static void Update();

	static void MakeSampleJson();
	static void LoadComponentTypeJsonFile(const std::string& filepath);
	static nlohmann::ordered_json ComponentTypeNameToJson(const std::unordered_map<std::string, uint32_t>& comps);
	static bool SaveJsonToFile(const nlohmann::ordered_json& j, const std::string& filepath);

	static uint32_t GetID_FromName(const std::string& name) {
		const std::unordered_map<std::string, uint32_t>::const_iterator it = nameToId.find(name);
		if (it != nameToId.end()) {
			return it->second;
		}

		std::wstring newName = StringToWString(name);

		std::wstring errorMsg = L"コンポーネント名 ： " + newName + L" に対応するＩＤは存在しません。";
		MessageBoxW(nullptr, errorMsg.c_str(), L"Error", MB_ICONERROR | MB_OK);
		return UINT32_MAX; // 魔法の数字で「存在しない」を表す
	}

	static std::string GetName_FromID(const uint32_t& id) {
		const std::unordered_map<uint32_t, std::string>::const_iterator it = idToName.find(id);
		if(it != idToName.end()) {
			return it->second; // 見つかった場合は名前を返す
		}
		MessageBoxW(nullptr, L"その ID のコンポーネントの名前は存在しません。", L"Error", MB_ICONERROR | MB_OK);

		return ""; // 見つからなかった場合は空文字を返す
		// ↑ここをどうするか決める
	}

	// コンポーネントの最大数
	static uint32_t GetMax() {
		return static_cast<uint32_t>(nameToId.size()); };
};
