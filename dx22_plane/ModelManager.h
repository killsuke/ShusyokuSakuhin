#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "StaticMesh.h"

class ModelManager {
public:
    // シングルトンのインスタンス取得
    static ModelManager& GetInstance() {
        static ModelManager instance;
        return instance;
    }

    // モデルを取得する関数
    std::shared_ptr<StaticMesh> GetModel(const std::string& modelPath, const std::string& texDirectory) {
        // メモリ内に既に格納しているか検査
        auto it = m_ModelCache.find(modelPath);
        if (it != m_ModelCache.end()) {
            return it->second;
        }

        // メッシュのシェアードポインタを生成して返す
        std::shared_ptr<StaticMesh> newModel = std::make_shared<StaticMesh>();
        newModel->Load(modelPath, texDirectory);
        m_ModelCache[modelPath] = newModel;
        return newModel;
    }

private:
    // プライベートコンストラクタ
    ModelManager() {}
    ~ModelManager() {}

    // コンストラクタを生成されないように
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    // 全てのモデルデータを保持
    std::unordered_map<std::string, std::shared_ptr<StaticMesh>> m_ModelCache;
};

