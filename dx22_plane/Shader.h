#pragma once
#include	<wrl/client.h>
#include	<string>
#include	<d3d11.h>
#include	<vector>

using Microsoft::WRL::ComPtr;

//-----------------------------------------------------------------------------
//Shaderクラス
//-----------------------------------------------------------------------------
class Shader{
public:
	void Create(const std::string& vs, const std::string& ps, const std::string& gs = "", std::vector<D3D11_INPUT_ELEMENT_DESC> lay = std::vector<D3D11_INPUT_ELEMENT_DESC>{});
	void CreateOneSkinAnimation(std::string vs, std::string ps);
	void SetGPU();
private:
	ComPtr<ID3D11VertexShader> m_pVertexShader;		// 頂点シェーダー
	ComPtr<ID3D11PixelShader>  m_pPixelShader;		// ピクセルシェーダー
	ComPtr<ID3D11InputLayout>  m_pVertexLayout;		// 頂点レイアウト
	ComPtr<ID3D11GeometryShader> m_pGeometryShader;	// ジオメトリシェーダー
};

