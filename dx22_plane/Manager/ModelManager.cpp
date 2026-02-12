#include "ModelManager.h"

void ModelManager::AddModel(const std::string& modelPath, const std::string& texDirectory, const StaticMesh& mesh) {
    // Šù‚É“o˜^‚³‚ê‚Ä‚¢‚éê‡‚Í’Ç‰Á‚µ‚È‚¢
    if (m_ModelCache.find(modelPath) != m_ModelCache.end()) {
        return;
    }
    // ƒƒ‚ƒŠ“à‚Éƒ‚ƒfƒ‹ƒf[ƒ^‚ğŠi”[
    std::unique_ptr<StaticMesh> newModel = std::make_unique<StaticMesh>(mesh);
    m_ModelCache[modelPath] = std::move(newModel);
}

StaticMesh* ModelManager::GetModel(const std::string& modelPath, const std::string& texDirectory) {
    // ƒƒ‚ƒŠ“à‚ÉŠù‚ÉŠi”[‚µ‚Ä‚¢‚é‚©ŒŸ¸
    const auto it = m_ModelCache.find(modelPath);
    if (it != m_ModelCache.end()) {
        return (it->second).get();
    }

    return nullptr;
}