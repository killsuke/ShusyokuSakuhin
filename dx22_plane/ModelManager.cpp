#include "ModelManager.h"

std::unordered_map<std::string, std::unique_ptr<StaticMesh>> ModelManager::m_ModelCache;

StaticMesh* ModelManager::GetModel(const std::string& modelPath, const std::string& texDirectory) {
    // メモリ内に既に格納しているか検査
    auto it = m_ModelCache.find(modelPath);
    if (it != m_ModelCache.end()) {
        return (it->second).get();
    }

    // メッシュの生ポインタを生成して返す
    std::unique_ptr<StaticMesh> newModel = std::make_unique<StaticMesh>();
    newModel->Load(modelPath, texDirectory);
    m_ModelCache[modelPath] = std::move(newModel);

    return m_ModelCache[modelPath].get();
}