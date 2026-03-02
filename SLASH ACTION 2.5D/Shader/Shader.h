#pragma once
#include	<wrl/client.h>
#include	<string>
#include	<d3d11.h>
#include	<vector>

using Microsoft::WRL::ComPtr;

//-----------------------------------------------------------------------------
//Shaderクラス
//-----------------------------------------------------------------------------
class Shader final{
public:
	void Create(const std::string& vs, const std::string& ps, const std::string& gs = "", std::vector<D3D11_INPUT_ELEMENT_DESC> lay = std::vector<D3D11_INPUT_ELEMENT_DESC>{});
	void CreateOneSkinAnimation(std::string vs, std::string ps);
	void SetGPU();
	void SetGPU(ID3D11DeviceContext* context);
	std::vector<std::string> GetShaderNames()const { return m_ShaderNames; };
private:
	ComPtr<ID3D11VertexShader> m_pVertexShader = nullptr;		// 頂点シェーダー
	ComPtr<ID3D11PixelShader>  m_pPixelShader = nullptr;		// ピクセルシェーダー
	ComPtr<ID3D11InputLayout>  m_pVertexLayout = nullptr;		// 頂点レイアウト
	ComPtr<ID3D11GeometryShader> m_pGeometryShader = nullptr;	// ジオメトリシェーダー
	std::vector<std::string> m_ShaderNames;
};

