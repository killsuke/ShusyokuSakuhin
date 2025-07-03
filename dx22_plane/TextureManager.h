#pragma once
#include "Texture2D.h"

class TextureManager
{
private:

    TextureManager() {};
    ~TextureManager() {};

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    // 全テクスチャ保持用
    std::unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> m_textures;

public:
    // シングルトンのインスタンス取得
    static TextureManager& GetInstance() {
        static TextureManager instance;
        return instance;
    };


    // モデルを取得する関数
    //std::shared_ptr<ComPtr<ID3D11ShaderResourceView>> GetModel(const std::string& modelPath, const std::string& texDirectory) {
    //    // メモリ内に既に格納しているか検査
    //    auto it = m_ModelCache.find(modelPath);
    //    if (it != m_ModelCache.end()) {
    //        return it->second;
    //    }

    //    // メッシュのシェアードポインタを生成して返す
    //    std::shared_ptr<StaticMesh> newModel = std::make_shared<StaticMesh>();
    //    newModel->Load(modelPath, texDirectory);
    //    m_ModelCache[modelPath] = newModel;
    //    return newModel;
    //}
};