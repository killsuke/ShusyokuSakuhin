// ----------------------------------------------
// テクスチャを管理するクラス
// ----------------------------------------------

#pragma once
#include <d3d11.h>
#include <unordered_map>
#include <string>
#include "DirectXRender.h"
#include "WICTextureLoader.h" // テクスチャ読み込みライブラリ
#include <wrl.h>
//#include <codecvt>
//#include <locale>

class TextureManager
{
private:
	// テクスチャ管理用のマップ
	// 文字列とDirectX11のシェーダーリソースビューを関連付ける
	static std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureMap;

public:
	static HRESULT LoadTexture(const std::string& filename);			// テクスチャの読み込み、シーンをロードするたびにテクスチャごとに一度だけ行う。
 	static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture(const std::string& filename);	// テクスチャの取得、インスタンスにテクスチャを渡す。
	static void ReleaseAllTextures();	// 片付け
	static std::wstring ShortConversion(const std::string& filename);		// ファイルパスを簡単に変換する

};

// ---------------------------------------------------------
// std::unordered_map：
// 〇ハッシュテーブルを内部に持つ連想コンテナ
// キーと値のペアを格納
// 高速な検索、挿入、削除が可能
// 
// ID3D11ShaderResourceView*
// DirectX11のシェーダーリソースビューを指すポインタ
// シェーダーリソースビューはシェーダーがアクセスするリソース（テクスチャなど）を表す
// ここでは、テクスチャデータを指すポインタを値として使用している
// ---------------------------------------------------------