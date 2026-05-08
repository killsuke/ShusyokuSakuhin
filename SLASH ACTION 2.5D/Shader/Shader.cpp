#include "Shader.h"
#include "Helper/dx11helper.h"
#include "System/DirectXRender.h"
#include "Manager/ShaderManager.h"

//=======================================
//Shader作成
//=======================================
void Shader::Create(const std::string& vs, const std::string& ps, const std::string& gs, std::vector<D3D11_INPUT_ELEMENT_DESC> lay)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = lay;

	if (layout.size() == 0) {
		// 頂点データの定義
		layout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout.push_back({ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout.push_back({ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	D3D11_APPEND_ALIGNED_ELEMENT,   D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}

	m_ShaderNames.push_back(vs);
	m_ShaderNames.push_back(ps);
	m_ShaderNames.push_back(gs);

	const unsigned int numElements = static_cast<unsigned int>(layout.size());

	ID3D11Device* device = DirectXRender::GetDevice();

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	bool sts = false;

	sts = CreateVertexShader(device,
		vs.c_str(),
		VS_INFORMATION.entryName.c_str(),
		VS_INFORMATION.modelName.c_str(),
		layout.data(),	// ポインタに変換
		numElements,
		&m_VertexShader,
		&m_VertexLayout);
	if (!sts) {
		MessageBoxW(nullptr, L"CreateVertexShader error", L"error", MB_OK | MB_ICONWARNING);
		return;
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
		device,							// デバイスオブジェクト
		ps.c_str(),
		PS_INFORMATION.entryName.c_str(),
		PS_INFORMATION.modelName.c_str(),
		&m_PixelShader);
	if (!sts) {
		MessageBoxW(nullptr, L"CreatePixelShader error", L"error", MB_OK | MB_ICONWARNING);
		return;
	}

	if (gs != "") {
		// ジオメトリシェーダーを生成
		sts = CreateGeometryShader(			// ピクセルシェーダーオブジェクトを生成
			device,							// デバイスオブジェクト
			gs.c_str(),
			GS_INFORMATION.entryName.c_str(),
			GS_INFORMATION.modelName.c_str(),
			&m_GeometryShader);
		if (!sts) {
			MessageBoxW(nullptr, L"CreateGeometryShader error", L"error", MB_OK | MB_ICONWARNING);
			return;
		}
	}

	return;
}

void Shader::CreateOneSkinAnimation(std::string vs, std::string ps)
{
	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "COLOR",			0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0,  D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "PAD",			0, DXGI_FORMAT_R32_FLOAT,			0,  D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,  D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	unsigned int numElements = ARRAYSIZE(layout);

	ID3D11Device* device = DirectXRender::GetDevice();

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	bool sts = CreateVertexShader(device,
		vs.c_str(),
		"vs_main",
		"vs_5_0",
		layout,
		numElements,
		&m_VertexShader,
		&m_VertexLayout);
	if (!sts) {
		MessageBoxW(nullptr, L"CreateVertexShader error", L"error", MB_OK | MB_ICONWARNING);
		return;
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
		device,							// デバイスオブジェクト
		ps.c_str(),
		"ps_main",
		"ps_5_0",
		&m_PixelShader);
	if (!sts) {
		MessageBoxW(nullptr, L"CreatePixelShader error", L"error", MB_OK | MB_ICONWARNING);
		return;
	}

	return;
}

//=======================================
//GPUにデータを送る
//=======================================
void Shader::SetGPU()
{
	ID3D11DeviceContext* devicecontext = DirectXRender::GetDeviceContext();

	devicecontext->VSSetShader(m_VertexShader.Get(), nullptr, 0);		// 頂点シェーダーをセット
	devicecontext->PSSetShader(m_PixelShader.Get(), nullptr, 0);		// ピクセルシェーダーをセット
	devicecontext->GSSetShader(m_GeometryShader.Get(), nullptr, 0);	// ジオメトリシェーダーをセット
	devicecontext->IASetInputLayout(m_VertexLayout.Get());				// 頂点レイアウトセット
}

void Shader::SetGPU(ID3D11DeviceContext* context) {

	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);		// 頂点シェーダーをセット
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);		// ピクセルシェーダーをセット
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);	// ジオメトリシェーダーをセット
	context->IASetInputLayout(m_VertexLayout.Get());				// 頂点レイアウトセット
}
