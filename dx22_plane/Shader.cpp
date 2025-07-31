#include	"Shader.h"
#include	"dx11helper.h"
#include	"DirectXRender.h"

//=======================================
//Shader作成
//=======================================
void Shader::Create(std::string vs, std::string ps)
{
	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	D3D11_APPEND_ALIGNED_ELEMENT,   D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
			&m_pVertexShader,
			&m_pVertexLayout);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return;
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
		device,							// デバイスオブジェクト
		ps.c_str(),
		"ps_main",
		"ps_5_0",
		&m_pPixelShader);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return;
	}

	return;
}

void Shader::CreateOneSkinAnimation(std::string vs, std::string ps)
{
	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
	//	{ "PAD",		0, DXGI_FORMAT_R32_FLOAT,			0,  D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	 	{ "COLOR",			0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0,  D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },	
		{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },

	//	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "PADDING",		0, DXGI_FORMAT_R32G32_FLOAT,			0,  D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
		&m_pVertexShader,
		&m_pVertexLayout);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return;
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
		device,							// デバイスオブジェクト
		ps.c_str(),
		"ps_main",
		"ps_5_0",
		&m_pPixelShader);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
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

	devicecontext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);		// 頂点シェーダーをセット
	devicecontext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);		// ピクセルシェーダーをセット
	devicecontext->IASetInputLayout(m_pVertexLayout.Get());				// 頂点レイアウトセット
}

