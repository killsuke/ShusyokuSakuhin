#include "TextureManager.h"

using namespace std::filesystem;

namespace {
	std::string TexturePath = "assets/texture/";
}

void TextureManager::Init() {

	textureMap.clear();

	LoadFolder(TexturePath);
}

void TextureManager::UnInit() {
    ReleaseAllTextures();
}

// 指定されたフォルダ内のすべてのファイルを読み込む
void TextureManager::LoadFolder(const std::string& path) {

    // 指定されたディレクトリのフォルダを開き読み込む
    for (auto& file : directory_iterator(path)) {

        // 通常ファイルであれば読み込む
        if (file.is_regular_file()) {

            std::string name = file.path().filename().string(); // 拡張子を除いたファイル名
            std::string full_path = file.path().string(); // フルパス

			// テクスチャの読み込み
            if (!LoadTexture(name)) {
                MessageBoxA(NULL, ("テクスチャの読み込みに失敗しました: " + name).c_str(), "エラー", MB_ICONERROR | MB_OK);
			}
        }
    }
}

// テクスチャの読み込み関数
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureManager::LoadTexture(const std::string& filename) {
	ID3D11Device* device = DirectXRender::GetDevice();
    ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

	const std::string full_path = TexturePath + filename; // フルパスを作成
	const std::wstring wfull_path = ShortConversion(full_path); // フルパスをワイド文字列に変換
    const std::wstring wfilename = ShortConversion(filename);
    
	// すでに読み込まれているテクスチャがあるかどうかを確認
    const std::unordered_map<std::wstring,Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>::iterator it 
        = textureMap.find(wfilename);

    if (it != textureMap.end()) {
        return it->second;    // 既存のテクスチャが見つかったら、すぐに返す。
    }

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = nullptr;

    // テクスチャの読み込み処理
    const HRESULT hr = DirectX::CreateWICTextureFromFileEx(device, deviceContext, wfull_path.c_str(), 0, D3D11_USAGE_DEFAULT,
                                                     D3D11_BIND_SHADER_RESOURCE, 0, 0, DirectX::WIC_LOADER_IGNORE_SRGB, nullptr, &texture);
    // エラーが起きた場合の処理
    if (FAILED(hr) || !texture) {
        MessageBoxA(NULL, "テクスチャ読み込み失敗", "エラー", MB_ICONERROR | MB_OK);
        return nullptr;
    }
    
    // 読み込みが成功すれば、そのテクスチャのポインタをキャッシュに保存
    textureMap[wfilename] = texture;
    
    return texture;
}

// テクスチャの取得関数
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureManager::GetTexture(const std::string& filename) {

    const std::wstring wfilename = ShortConversion(filename);
    const std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>::iterator it 
        = textureMap.find(wfilename); // マップ内でファイル名に対応する要素を検索
	
    if (it != textureMap.end()) { // 要素が見つかった場合
		return it->second; // 対応するテクスチャを返す
	}
    
    // エラー処理
    MessageBoxA(NULL, "要求されたテクスチャが存在しませんでした。", "エラー", MB_ICONERROR | MB_OK);
    return nullptr;
}

// 解放処理
void TextureManager::ReleaseAllTextures() {
	textureMap.clear();
}

// stringをwstringに変換する関数
std::wstring TextureManager::ShortConversion(const std::string& filename) {

	const int len = MultiByteToWideChar(CP_UTF8, 0, filename.c_str(), -1, nullptr, 0);
    std::wstring result(len -1, wchar_t(0));    // null終端を除外
	MultiByteToWideChar(CP_UTF8, 0, filename.c_str(), -1, &result[0], len);

	return result;
}