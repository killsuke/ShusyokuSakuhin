#include "TextureManager.h"

// 静的な変数宣言を確定
std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>
TextureManager::textureMap;

// テクスチャの読み込み関数
HRESULT TextureManager::LoadTexture(const std::string& filename) {
    // ファイルネームに「asset」追加と「L」の省略処理
    std::wstring wfilename = ShortConversion(filename);
    
    auto it = textureMap.find(wfilename);
    if (it != textureMap.end()) {
        return S_OK;    // 既存のテクスチャが見つかったら、すぐに返す。
    }

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = nullptr;

    // テクスチャの読み込み処理
    HRESULT hr = DirectX::CreateWICTextureFromFileEx(g_pDevice, g_pDeviceContext, wfilename.c_str(), 0, D3D11_USAGE_DEFAULT,
                                                     D3D11_BIND_SHADER_RESOURCE, 0, 0, DirectX::WIC_LOADER_IGNORE_SRGB, nullptr, &texture);
    // エラーが起きた場合の処理
    if (FAILED(hr)) {
        MessageBoxA(NULL, "テクスチャ読み込み失敗", "エラー", MB_ICONERROR | MB_OK);
        return E_FAIL;
    }
    
    // 読み込みが成功すれば、そのテクスチャのポインタをキャッシュに保存
    textureMap[wfilename] = texture;
    
    return hr;
}

// テクスチャの取得関数
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureManager::GetTexture(const std::string& filename) {
    std::wstring wfilename = ShortConversion(filename);
	auto it = textureMap.find(wfilename); // マップ内でファイル名に対応する要素を検索
	if (it != textureMap.end()) { // 要素が見つかった場合
		return it->second; // 対応するテクスチャを返す
	}
	//return nullptr; // 要素が見つからなかった場合、nullptrを返す
    throw std::runtime_error("Texture not found"); // 見つからなかった場合に例外をスロー

}

// 解放処理
void TextureManager::ReleaseAllTextures() {
	textureMap.clear(); // これだけでOK
}

// Lとasset/の省略
std::wstring TextureManager::ShortConversion(const std::string& filename) {
    // ファイルパスに "asset/" を追加する
   // std::string fullPath = "asset/" + filename;

    // UTF-8 から UTF-16 に文字列を変換
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(filename);
}