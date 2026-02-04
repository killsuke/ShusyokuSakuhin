#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "StaticMesh.h"

class ModelManager final
{
public:

	static void AddModel(const std::string& modelPath, const std::string& texDirectory,const StaticMesh& mesh);

    // モデルを取得する関数
    static StaticMesh* GetModel(const std::string& modelPath, const std::string& texDirectory);

private:
	// コンストラクタ・デストラクタを削除
	ModelManager() = delete;
	~ModelManager() = delete;

	// コピー・ムーブも削除
	ModelManager(const ModelManager&) = delete;
	ModelManager(ModelManager&&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;
	ModelManager& operator=(ModelManager&&) = delete;

    // 全てのモデルデータを保持
    static std::unordered_map<std::string, std::unique_ptr<StaticMesh>> m_ModelCache;
};

