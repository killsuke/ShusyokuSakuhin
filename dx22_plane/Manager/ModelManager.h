#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "StaticMesh.h"

class ModelManager {
public:

	static void AddModel(const std::string& modelPath, const std::string& texDirectory,const StaticMesh& mesh);

    // モデルを取得する関数
    static StaticMesh* GetModel(const std::string& modelPath, const std::string& texDirectory);

private:
    // プライベートコンストラクタ
	ModelManager() = default;
	~ModelManager() = default;

    // コンストラクタを生成されないように
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    // 全てのモデルデータを保持
    static std::unordered_map<std::string, std::unique_ptr<StaticMesh>> m_ModelCache;
};

