#include "DirectXRender.h"
#include "Application.h"
#include "BoneData.h"
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <locale.h>
//#include <atltypes.h> // CRectを使うためのヘッダーファイル
#include <io.h>
#include <stdio.h>
#include <string.h>
#include "HPParam.h"
using namespace DirectX::SimpleMath;

D3D_FEATURE_LEVEL      m_FeatureLevel;

ID3D11DeviceContext* DirectXRender::m_DeviceContext = nullptr; // コンテキスト＝描画関連を司る機能

ID3D11Device* DirectXRender::m_Device = nullptr; // コンテキスト＝描画関連を司る機能

IDXGISwapChain* DirectXRender::m_SwapChain = nullptr; // スワップチェーン＝ダブルバッファ機能

ID3D11DepthStencilState* DirectXRender::m_DepthStateEnable = nullptr;

ID3D11DepthStencilState* DirectXRender::m_DepthStateDisable = nullptr;

// レンダーターゲット＝描画先を表す機能
ID3D11RenderTargetView* DirectXRender::g_pRenderTargetView = nullptr;
// デプスバッファ
ID3D11DepthStencilView* DirectXRender::g_pDepthStencilView = nullptr;

ID3D11Buffer* DirectXRender::m_LightBuffer = nullptr;

CameraMatrix DirectXRender::m_CameraMatrix = {};

ID3D11Buffer* DirectXRender::g_pLineThicknessBuffer = nullptr; // 線の太さ

ID3D11DepthStencilState* g_DepthStateEnable = nullptr;

ID3D11DepthStencilState* g_DepthStateDisable = nullptr;

// インプットレイアウト
ID3D11InputLayout* g_pInputLayout = nullptr;
// 頂点シェーダーオブジェクト
ID3D11VertexShader* g_pUnlitVertexShader = nullptr;
// ピクセルシェーダーオブジェクト
ID3D11PixelShader* g_pUnlitPixelShader = nullptr;
// サンプラー用変数
ID3D11SamplerState* g_pSampler = nullptr;
// 定数バッファ用変数
ID3D11Buffer* g_pConstantBuffer = nullptr;

// ボーン用の定数バッファ構造体
ID3D11Buffer* g_pBoneConstantBuffer = nullptr;

// ＨＰバー用の定数バッファ構造体
ID3D11Buffer* g_pHPBarConstantBuffer = nullptr;

// ブラー用のバッファ
ID3D11Buffer* g_pBlurBuffer = nullptr;

ID3D11Buffer* m_MaterialBuffer = nullptr;

// ブレンドステート用変数（アルファブレンディング）
ID3D11BlendState* g_BlendState[MAX_BLENDSTATE]; // ブレンド ステート;

ID3D11BlendState* g_BlendStateATC = nullptr;

//ID3D11Buffer* g_pViewBuffer3D{}; // ビュー行列
//ID3D11Buffer* g_pProjectionBuffer3D{}; // プロジェクション行列
//
//ID3D11Buffer* g_pViewBuffer2D{}; // ビュー行列
//ID3D11Buffer* g_pProjectionBuffer2D{}; // プロジェクション行列
//
//ID3D11Buffer* g_pViewBufferSkyDome{}; // ビュー行列
//ID3D11Buffer* g_pProjectionBufferSkyDome{}; // プロジェクション行列

ID3D11Buffer* g_pCameraInformationBuffer{}; // カメラ情報

DirectXRender::DirectXRender() {

}

DirectXRender::~DirectXRender() {
	//	UnInit();	// これはちゃんと呼び出されている？
}

HRESULT DirectXRender::Init() {

	DeviceAndSwapCreate();	// デバイス、スワップチェーン作成
	RenderTargetCreate();	// レンダーターゲットビュー作成
	DepthStencilCreate();	// デプスステンシルバッファ作成
	ViewportCreate();		// ビューポート設定
	//InputLayoutAndShadersCreate();
	RasterizerSetting();
	BlandStateCreate();
	DepthStencilSetting();
	SamplerCreate();
	ConstantBufferCreate();
	BoneConstantBufferCreate();
	HPBarConstantBufferCreate();

	LightBufferCreate();
	LightSetting();

	MaterialBufferCreate();
	//MaterialSetting();

	SetBlendState(1);

	VeiwProjConstantCreate();

	// シェーダーオブジェクトを作る
	// 定数バッファ構造体を作る
	// 定数バッファとかけて描画できるようにする




	//bufferDesc.ByteWidth = sizeof(MATERIAL);
	//hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_MaterialBuffer);
	//m_DeviceContext->VSSetConstantBuffers(6, 1, &m_MaterialBuffer);
	//m_DeviceContext->PSSetConstantBuffers(6, 1, &m_MaterialBuffer);
	//if (FAILED(hr)) return;

	//// マテリアル初期化
	//MATERIAL material{};
	//material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	//material.Ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	//SetMaterial(material);

	//bufferDesc.ByteWidth = sizeof(Matrix);
	//hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_TextureBuffer);
	//m_DeviceContext->VSSetConstantBuffers(7, 1, &m_TextureBuffer);
	//if (FAILED(hr)) return;

	//// ＵＶ初期化
	//SetUV(0, 0, 1, 1);

	return S_OK;
}

void DirectXRender::UnInit() {

	/*m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();*/

	//m_LightBuffer->Release();
	//m_MaterialBuffer->Release();

	m_DeviceContext->ClearState();
	SAFE_RELEASE(g_pRenderTargetView);
	SAFE_RELEASE(g_pDepthStencilView);
	SAFE_RELEASE(m_DepthStateEnable);
	SAFE_RELEASE(m_DepthStateDisable);
	SAFE_RELEASE(m_SwapChain);
	SAFE_RELEASE(g_pInputLayout);
	SAFE_RELEASE(g_pUnlitVertexShader);
	SAFE_RELEASE(g_pUnlitPixelShader);
	SAFE_RELEASE(g_pSampler);
	SAFE_RELEASE(g_pConstantBuffer);
	SAFE_RELEASE(g_pBoneConstantBuffer);
	SAFE_RELEASE(g_pHPBarConstantBuffer);
	SAFE_RELEASE(g_pBlurBuffer);
	SAFE_RELEASE(m_LightBuffer);
	SAFE_RELEASE(m_MaterialBuffer);
	for (int i = 0; i < MAX_BLENDSTATE; ++i) {
		if (g_BlendState[i]) {  // nullptr チェック
			SAFE_RELEASE(g_BlendState[i]);
			//g_BlendState[i] = nullptr;  // 解放後にポインタをクリア
		}
	}
	SAFE_RELEASE(g_BlendStateATC);
	/*SAFE_RELEASE(g_pViewBuffer3D);
	SAFE_RELEASE(g_pProjectionBuffer3D);
	SAFE_RELEASE(g_pViewBuffer2D);
	SAFE_RELEASE(g_pProjectionBuffer2D);
	SAFE_RELEASE(g_pViewBufferSkyDome);
	SAFE_RELEASE(g_pProjectionBufferSkyDome);*/
	SAFE_RELEASE(g_pCameraInformationBuffer);
	SAFE_RELEASE(g_pLineThicknessBuffer);
	SAFE_RELEASE(m_DeviceContext);
	SAFE_RELEASE(m_Device);
}

//=======================================
//描画開始
//=======================================
void DirectXRender::DrawBegin() {
	// 塗りつぶしたい色
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	// 描画先のキャンバスと使用する深度バッファを指定する
	// レンダーターゲットとデプスステンシルビューを設定
	m_DeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	// 描画先キャンバスを塗りつぶす
	m_DeviceContext->ClearRenderTargetView(g_pRenderTargetView, clearColor);
	// 深度バッファをリセットする
	m_DeviceContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// 	インプットレイアウト（GPUに渡す頂点データのフォーマットを定義）を設定
	//m_DeviceContext->IASetInputLayout(g_pInputLayout);
	// どのようにテクスチャを扱うかを設定
	// ポストエフェクト等でどんな描画かを使う場合は、
	// 最初の値（スロット）を１以上にする
	// 次の引数たちも増やすことも考える
	m_DeviceContext->PSSetSamplers(0, 1, &g_pSampler);
	// 定数バッファを頂点シェーダーにセットする
	m_DeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	//m_DeviceContext->OMSetDepthStencilState(g_DepthStateDisable, 0);

	// 定数バッファを頂点シェーダーにセットする
	m_DeviceContext->VSSetConstantBuffers(8, 1, &g_pBoneConstantBuffer);

	m_DeviceContext->VSSetConstantBuffers(9, 1, &g_pHPBarConstantBuffer);
}

//=======================================
//描画終了
//=======================================
void DirectXRender::DrawEnd() {
	// ダブルバッファの切り替え
	m_SwapChain->Present(1, 0);
}

HRESULT DirectXRender::DeviceAndSwapCreate() {
	HRESULT hr = S_OK;

	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1; // バックバッファの数を1に設定（ダブルバッファリング）
	swapChainDesc.BufferDesc.Width = Application::GetWidth(); // バッファの幅をウィンドウサイズに合わせる
	swapChainDesc.BufferDesc.Height = Application::GetHeight(); // バッファの高さをウィンドウサイズに合わせる
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // バッファのピクセルフォーマットを設定
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60; // リフレッシュレートを設定（Hz）
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // バッファの使用用途を設定
	swapChainDesc.OutputWindow = Application::GetWindow(); // スワップチェーンのターゲットウィンドウを設定
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプリングの設定（アンチエイリアスのサンプル数とクオリティ）
	swapChainDesc.SampleDesc.Quality = 0; //同上
	swapChainDesc.Windowed = TRUE; // ウィンドウモード（フルスクリーンではなく、ウィンドウモードで実行）

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0,
		D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, &m_FeatureLevel, &m_DeviceContext);
	if (FAILED(hr)) return hr;

	return hr;
}

// レンダーターゲット作成
HRESULT DirectXRender::RenderTargetCreate() {
	HRESULT hr = S_OK; // デバイス、スワップチェーン作成

	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget{};
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	if (renderTarget != nullptr)m_Device->CreateRenderTargetView(renderTarget, NULL, &g_pRenderTargetView);
	renderTarget->Release();
	if (FAILED(hr)) return hr;

	return hr;
}

HRESULT DirectXRender::DepthStencilCreate() {

	HRESULT hr = S_OK;

	// デプスステンシルバッファ作成
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = Application::GetWidth();   // バッファの幅をスワップチェーンに合わせる
	textureDesc.Height = Application::GetHeight(); // バッファの高さをスワップチェーンに合わせる
	textureDesc.MipLevels = 1;                            // ミップレベルは1（ミップマップは使用しない）
	textureDesc.ArraySize = 1;                            // テクスチャの配列サイズ（通常1）
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;           // フォーマットは16ビットの深度バッファを使用
	//textureDesc.Format = DXGI_FORMAT_D32_FLOAT;           // フォーマットは16ビットの深度バッファを使用
	// ↑ここをいじると奥行判定の精度が変わる（パフォーマンスに影響を及ぼす）
	textureDesc.SampleDesc.Count = 1;    // スワップチェーンと同じサンプル設定
	textureDesc.SampleDesc.Quality = 0;    // スワップチェーンと同じサンプル設定
	textureDesc.Usage = D3D11_USAGE_DEFAULT;              // 使用方法はデフォルト（GPUで使用）
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;     // 深度ステンシルバッファとして使用
	textureDesc.CPUAccessFlags = 0;                       // CPUからのアクセスは不要
	textureDesc.MiscFlags = 0;                            // その他のフラグは設定なし

	hr = m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);
	if (FAILED(hr)) return hr;

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format; // デプスステンシルバッファのフォーマットを設定
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // ビューの次元を2Dテクスチャとして設定（2Dテクスチャ用のデプスステンシルビュー）
	depthStencilViewDesc.Flags = 0; // 特別なフラグは設定しない（デフォルトの動作）
	if (depthStencile != nullptr)m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &g_pDepthStencilView);
	if (FAILED(hr)) return hr;
	depthStencile->Release();

	return hr;
}

void DirectXRender::ViewportCreate() {
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)Application::GetWidth();   // ビューポートの幅
	viewport.Height = (FLOAT)Application::GetHeight(); // ビューポートの高さ
	viewport.MinDepth = 0.0f;                          // 深度範囲の最小値
	viewport.MaxDepth = 1.0f;                          // 深度範囲の最大値
	viewport.TopLeftX = 0;                             // ビューポートの左上隅のX座標
	viewport.TopLeftY = 0;                             // ビューポートの左上隅のY座標）
	m_DeviceContext->RSSetViewports(1, &viewport);
}

HRESULT DirectXRender::RasterizerSetting() {
	HRESULT hr = S_OK;

	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	//rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;	// 試してみよう
	//rasterizerDesc.CullMode = D3D11_CULL_BACK;	 // 裏面を表示しない
	rasterizerDesc.CullMode = D3D11_CULL_NONE; // 裏面も表示する
	//rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	ID3D11RasterizerState* rs;
	hr = m_Device->CreateRasterizerState(&rasterizerDesc, &rs);
	if (FAILED(hr)) return hr;

	m_DeviceContext->RSSetState(rs);

	return hr;
}

// ブレンドステート作成
HRESULT DirectXRender::BlandStateCreate() {
	HRESULT hr = S_OK;

	// ブレンド ステート生成
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));                   // BlendDesc構造体をゼロで初期化し、メモリをクリア
	BlendDesc.AlphaToCoverageEnable = FALSE;                     // アルファ・トゥ・カバレッジを無効化（透明度をカバレッジとして利用しない）
	BlendDesc.IndependentBlendEnable = TRUE;                     // 各レンダーターゲットに対して個別のブレンド設定を有効化
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;               // ブレンドを無効に設定（不透明な描画）
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;  // ソース（描画するピクセル）のアルファ値を使用
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // デスティネーション（既存のピクセル）の逆アルファ値を使用
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;      // ソースとデスティネーションを加算する操作
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;   // ソースのアルファ値をそのまま使用
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // デスティネーションのアルファ値を無視
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // アルファ値に対して加算操作を行う
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // レンダーターゲットのカラーチャンネル書き込みマスク

	hr = m_Device->CreateBlendState(&BlendDesc, &g_BlendState[0]);
	if (FAILED(hr)) return hr;

	// ブレンド ステート生成 (アルファ ブレンド用)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	hr = m_Device->CreateBlendState(&BlendDesc, &g_BlendState[1]);
	if (FAILED(hr)) return hr;

	// ブレンド ステート生成 (加算合成用)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	hr = m_Device->CreateBlendState(&BlendDesc, &g_BlendState[2]);
	if (FAILED(hr)) return hr;

	// ブレンド ステート生成 (減算合成用)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	hr = m_Device->CreateBlendState(&BlendDesc, &g_BlendState[3]);
	if (FAILED(hr)) return hr;

	SetBlendState(BS_ALPHABLEND);

	return hr;
}

// デプスステンシル設定
HRESULT DirectXRender::DepthStencilSetting() {
	HRESULT hr = S_OK;

	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	hr = m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable); //深度有効ステート
	if (FAILED(hr)) return hr;

	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS; // 深度比較関数を常に通す

	hr = m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable); //深度無効ステート
	if (FAILED(hr)) return hr;

	m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);

	return hr;
}

// サンプラー作成
HRESULT DirectXRender::SamplerCreate() {

	HRESULT hr = S_OK;

	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//	ID3D11SamplerState* samplerState{};
	hr = m_Device->CreateSamplerState(&samplerDesc, &g_pSampler);
	if (FAILED(hr)) return hr;

	return hr;
}

// 定数バッファ作成
HRESULT DirectXRender::ConstantBufferCreate() {
	HRESULT hr;
	// 定数バッファ作成
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(ConstBuffer);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	hr = m_Device->CreateBuffer(&cbDesc, NULL, &g_pConstantBuffer);
	if (FAILED(hr)) return hr;

	return hr;
}

// 定数バッファ作成
HRESULT DirectXRender::BoneConstantBufferCreate() {// コンスタントバッファサイズ
	HRESULT hr;

	// ボーン用の定数バッファ作成
	D3D11_BUFFER_DESC bd;

	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(CBBoneMatrix);									// バッファの大き
	bd.Usage = D3D11_USAGE_DEFAULT;							// バッファ使用方法
	//	bd.Usage = D3D11_USAGE_DYNAMIC;							// バッファ使用方法
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;					// コンスタントバッファ
	bd.CPUAccessFlags = 0;					// CPUアクセス可能
	//	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;					// CPUアクセス可能

	hr = m_Device->CreateBuffer(&bd, nullptr, &g_pBoneConstantBuffer);
	if (FAILED(hr)) {
		MessageBox(nullptr, "CreateBuffer(constant buffer) error", "Error", MB_OK);
		return hr;
	}

	return hr;
}

// 定数バッファ作成
HRESULT DirectXRender::HPBarConstantBufferCreate() {// コンスタントバッファサイズ
	HRESULT hr;

	// ボーン用の定数バッファ作成
	D3D11_BUFFER_DESC bd;

	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(HPParam);									// バッファの大き
	bd.Usage = D3D11_USAGE_DEFAULT;							// バッファ使用方法
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;					// コンスタントバッファ
	bd.CPUAccessFlags = 0;					// CPUアクセス可能

	hr = m_Device->CreateBuffer(&bd, nullptr, &g_pHPBarConstantBuffer);
	if (FAILED(hr)) {
		MessageBox(nullptr, "CreateBuffer(constant buffer) error", "Error", MB_OK);
		return hr;
	}

	return hr;
}

HRESULT DirectXRender::LightBufferCreate() {
	HRESULT hr;

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(LIGHT);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_LightBuffer);

	if (FAILED(hr)) {
		MessageBox(nullptr, "CreateBuffer(constant buffer) error", "Error", MB_OK);
		return hr;
	}

	m_DeviceContext->VSSetConstantBuffers(5, 1, &m_LightBuffer);

	return hr;
}

void DirectXRender::LightSetting() {
	// ライト初期化
	LIGHT light{};
	light.Direction = Vector4(0.0f, -1.0f, 0.2f, 0.0f);	// 方向
	light.Direction.Normalize();
	light.Diffuse = Color(1.5f, 1.5f, 1.0f, 1.0f);	// 平行光源の強さと色
	light.Ambient = Color(0.7f, 0.7f, 0.7f, 1.0f);	// 環境光の強さと色

	m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &light, 0, 0);
}

void DirectXRender::MaterialSetting() {
	// マテリアル初期化
	MATERIAL material{};
	material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_DeviceContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &material, 0, 0);
}

HRESULT DirectXRender::MaterialBufferCreate() {
	HRESULT hr;

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(MATERIAL);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_MaterialBuffer);

	if (FAILED(hr)) {
		MessageBox(nullptr, "CreateBuffer(constant buffer) error", "Error", MB_OK);
		return hr;
	}

	// 頂点＆ピクセルシェーダー両方にセット
	m_DeviceContext->VSSetConstantBuffers(6, 1, &m_MaterialBuffer);
	m_DeviceContext->PSSetConstantBuffers(6, 1, &m_MaterialBuffer);

	return hr;
}

HRESULT DirectXRender::InputLayoutAndShadersCreate() {
	HRESULT hr;
	// インプットレイアウト作成
	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		// 頂点の位置情報（3つのfloat値）
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// 頂点の法線ベクトル情報（3つのfloat値）
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	4 * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// 頂点の色情報（4つのfloat値：RGBA）
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	4 * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// 頂点のテクスチャ座標情報（2つのfloat値）
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0,	4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	hr = CreateVertexShader(m_Device, "unlitTextureVS.hlsl", "vs_main", "vs_5_0",
		layout, numElements, &g_pUnlitVertexShader, &g_pInputLayout);
	if (FAILED(hr)) {
		MessageBoxA(NULL, "CreateVertexShader error", "error", MB_OK);
		return E_FAIL;
	}

	// ピクセルシェーダーオブジェクトを生成
	hr = CreatePixelShader(m_Device, "unlitTexturePS.hlsl", "ps_main", "ps_5_0", &g_pUnlitPixelShader);
	if (FAILED(hr)) {
		MessageBoxA(NULL, "CreatePixelShader error", "error", MB_OK);
		return E_FAIL;
	}

	return hr;
}

//--------------------------------------------------------------------------------------
// 頂点シェーダーオブジェクトを生成する
//--------------------------------------------------------------------------------------
HRESULT DirectXRender::CreateVertexShader(ID3D11Device* device, const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel,
	D3D11_INPUT_ELEMENT_DESC* layout, unsigned int numElements, ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout)
{
	HRESULT   hr;
	ID3DBlob* pBlob = nullptr;
	void* ShaderObject;
	size_t	  ShaderObjectSize;

	// ファイルの拡張子に合わせてコンパイル
	hr = CompileShader(szFileName, szEntryPoint, szShaderModel, &ShaderObject, ShaderObjectSize, &pBlob);
	if (FAILED(hr))
	{
		if (pBlob)pBlob->Release();
		return E_FAIL;
	}

	// 頂点シェーダーを生成
	hr = device->CreateVertexShader(ShaderObject, ShaderObjectSize, NULL, ppVertexShader);
	if (FAILED(hr))
	{
		if (pBlob)pBlob->Release();
		return E_FAIL;
	}

	// 頂点データ定義生成
	hr = device->CreateInputLayout(
		layout,
		numElements,
		ShaderObject,
		ShaderObjectSize,
		ppVertexLayout);
	if (FAILED(hr)) {
		MessageBoxA(NULL, "CreateInputLayout error", "error", MB_OK);
		pBlob->Release();
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// ピクセルシェーダーオブジェクトを生成する
//--------------------------------------------------------------------------------------
HRESULT DirectXRender::CreatePixelShader(ID3D11Device* device, const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11PixelShader** ppPixelShader)
{
	HRESULT   hr;
	ID3DBlob* pBlob = nullptr;
	void* ShaderObject;
	size_t	  ShaderObjectSize;

	// ファイルの拡張子に合わせてコンパイル
	hr = CompileShader(szFileName, szEntryPoint, szShaderModel, &ShaderObject, ShaderObjectSize, &pBlob);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// ピクセルシェーダーを生成
	hr = device->CreatePixelShader(ShaderObject, ShaderObjectSize, NULL, ppPixelShader);
	if (FAILED(hr))
	{
		if (pBlob)pBlob->Release();
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// シェーダーをファイル拡張子に合わせてコンパイル
//--------------------------------------------------------------------------------------
HRESULT DirectXRender::CompileShader(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, void** ShaderObject, size_t& ShaderObjectSize, ID3DBlob** ppBlobOut)
{
	*ppBlobOut = nullptr;
	//int aaa = strlen(szFileName);
	//拡張子をhlslからcsoに変更したファイル名を作成
	char* csoFileName = new char[strlen(szFileName)];
	const char* dot = strrchr(szFileName, '.'); // 最後の"."を探す
	if (dot != NULL) {
		size_t prefixLength = dot - szFileName;
		strncpy_s(csoFileName, prefixLength + 1, szFileName, _TRUNCATE); // 拡張子以外をコピー
		strcpy_s(csoFileName + prefixLength, 5, ".cso"); // 新しい拡張子を追加
	}
	else {
		strcpy_s(csoFileName, strlen(szFileName), szFileName); // 拡張子がない場合はそのままコピー
	}

	FILE* fp;
	// コンパイル済みシェーダーファイル(cso)があれば読み込む
	if (fopen_s(&fp, csoFileName, "rb") == 0)
	{
		long int size = _filelength(_fileno(fp));
		unsigned char* buffer = new unsigned char[size];
		fread(buffer, size, 1, fp);
		if (!buffer) return E_FAIL;
		*ShaderObject = buffer;
		ShaderObjectSize = size;
		fclose(fp);
	}
	// コンパイル済みシェーダーファイルが無ければシェーダーファイル(hlsl)をコンパイルする
	else
	{
		ID3DBlob* p1 = nullptr;
		HRESULT hr = S_OK;
		WCHAR	filename[512];
		size_t 	wLen = 0;
		int err = 0;

		// char -> wcharに変換
		setlocale(LC_ALL, "japanese");
		err = mbstowcs_s(&wLen, filename, 512, szFileName, _TRUNCATE);

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		// D3DCOMPILE_DEBUGフラグを設定すると、シェーダーにデバッグ情報が埋め込まれる
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* pErrorBlob = nullptr;
		hr = D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
		if (FAILED(hr))
		{
			if (pErrorBlob != nullptr) {
				MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "Error", MB_OK);
			}
			if (pErrorBlob) pErrorBlob->Release();
			if (*ppBlobOut)(*ppBlobOut)->Release();
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();

		*ShaderObject = (*ppBlobOut)->GetBufferPointer();
		ShaderObjectSize = (*ppBlobOut)->GetBufferSize();
	}

	delete[] csoFileName;
	return S_OK;
}

HRESULT DirectXRender::VeiwProjConstantCreate() {
	HRESULT hr = S_OK;

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(Matrix);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	//hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_WorldBuffer);
	//m_DeviceContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);
	//if (FAILED(hr)) return;

	/*hr = m_Device->CreateBuffer(&bufferDesc, NULL, &g_pViewBuffer3D);
	m_DeviceContext->VSSetConstantBuffers(1, 1, &g_pViewBuffer3D);
	if (FAILED(hr)) return hr;

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &g_pProjectionBuffer3D);
	m_DeviceContext->VSSetConstantBuffers(2, 1, &g_pProjectionBuffer3D);
	if (FAILED(hr)) return hr;

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &g_pViewBuffer2D);
	m_DeviceContext->VSSetConstantBuffers(3, 1, &g_pViewBuffer2D);
	if (FAILED(hr)) return hr;

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &g_pProjectionBuffer2D);
	m_DeviceContext->VSSetConstantBuffers(4, 1, &g_pProjectionBuffer2D);
	if (FAILED(hr)) return hr;

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &g_pViewBufferSkyDome);
	m_DeviceContext->VSSetConstantBuffers(10, 1, &g_pViewBufferSkyDome);
	if (FAILED(hr)) return hr;

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &g_pProjectionBufferSkyDome);
	m_DeviceContext->VSSetConstantBuffers(11, 1, &g_pProjectionBufferSkyDome);
	if (FAILED(hr)) return hr;*/
	bufferDesc.ByteWidth = sizeof(CameraMatrix);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &g_pCameraInformationBuffer);
	m_DeviceContext->VSSetConstantBuffers(1, 1, &g_pCameraInformationBuffer);

	bufferDesc.ByteWidth = sizeof(LineThickness);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &g_pLineThicknessBuffer);
	m_DeviceContext->GSSetConstantBuffers(1, 1, &g_pCameraInformationBuffer);
	m_DeviceContext->GSSetConstantBuffers(2, 1, &g_pLineThicknessBuffer);

	bufferDesc.ByteWidth = sizeof(BlurBuffer);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &g_pBlurBuffer);
	m_DeviceContext->VSSetConstantBuffers(3, 1, &g_pBlurBuffer);
	m_DeviceContext->PSSetConstantBuffers(3, 1, &g_pBlurBuffer);

	// ここ後で１番に変更
	if (FAILED(hr)) return hr;
	
	return hr;
}

//=======================================
// ビュー行列を設定（３Ｄ）
//=======================================
void DirectXRender::SetViewMatrix3D(DirectX::SimpleMath::Matrix* ViewMatrix)
{
//	DirectX::SimpleMath::Matrix view;
	m_CameraMatrix.matrixView3D = *ViewMatrix; // 転置

	// ビュー行列をGPU側へ送る
//	m_DeviceContext->UpdateSubresource(g_pViewBuffer3D, 0, NULL, &view, 0, 0);
}

//=======================================
// プロジェクション行列を設定（３Ｄ）
//=======================================
void DirectXRender::SetProjectionMatrix3D(DirectX::SimpleMath::Matrix* ProjectionMatrix)
{
//	DirectX::SimpleMath::Matrix projection;
	m_CameraMatrix.matrixProjection3D = *ProjectionMatrix; // 転置

	// プロジェクション行列をGPU側へ送る
//	m_DeviceContext->UpdateSubresource(g_pProjectionBuffer3D, 0, NULL, &projection, 0, 0);
}

//=======================================
// ビュー行列を設定（２Ｄ）
//=======================================
void DirectXRender::SetViewMatrix2D(DirectX::SimpleMath::Matrix* ViewMatrix)
{
//	DirectX::SimpleMath::Matrix view;
	m_CameraMatrix.matrixView2D = *ViewMatrix; // 転置

	// ビュー行列をGPU側へ送る
//	m_DeviceContext->UpdateSubresource(g_pViewBuffer2D, 0, NULL, &view, 0, 0);
}

//=======================================
// プロジェクション行列を設定（２Ｄ）
//=======================================
void DirectXRender::SetProjectionMatrix2D(DirectX::SimpleMath::Matrix* ProjectionMatrix)
{
//	DirectX::SimpleMath::Matrix projection;
	m_CameraMatrix.matrixProjection2D = *ProjectionMatrix; // 転置

	// プロジェクション行列をGPU側へ送る
//	m_DeviceContext->UpdateSubresource(g_pProjectionBuffer2D, 0, NULL, &projection, 0, 0);
}

//=======================================
// ビュー行列を設定（スカイドーム用）
//=======================================
void DirectXRender::SetViewMatrixSkyDome(DirectX::SimpleMath::Matrix* ViewMatrix)
{
//	DirectX::SimpleMath::Matrix view;
	m_CameraMatrix.matrixViewSkyDome = *ViewMatrix; // 転置

	// ビュー行列をGPU側へ送る
//	m_DeviceContext->UpdateSubresource(g_pViewBufferSkyDome, 0, NULL, &view, 0, 0);
}

//=======================================
// プロジェクション行列を設定（スカイドーム用）
//=======================================
void DirectXRender::SetProjectionMatrixSkyDome(DirectX::SimpleMath::Matrix* ProjectionMatrix)
{
//	DirectX::SimpleMath::Matrix projection;
	m_CameraMatrix.matrixProjectionSkyDome = *ProjectionMatrix; // 転置

	// プロジェクション行列をGPU側へ送る
//	m_DeviceContext->UpdateSubresource(g_pProjectionBufferSkyDome, 0, NULL, &projection, 0, 0);
}

//=======================================
// カメラ情報をGPUへ送る
//=======================================
void DirectXRender::GPU_UpdateViewAndProj() {
	CameraMatrix mtx = m_CameraMatrix;
	mtx.matrixView3D = m_CameraMatrix.matrixView3D.Transpose();
	mtx.matrixProjection3D = m_CameraMatrix.matrixProjection3D.Transpose();
	mtx.matrixView2D = m_CameraMatrix.matrixView2D.Transpose();
	mtx.matrixProjection2D = m_CameraMatrix.matrixProjection2D.Transpose();
	mtx.matrixViewSkyDome = m_CameraMatrix.matrixViewSkyDome.Transpose();
	mtx.matrixProjectionSkyDome = m_CameraMatrix.matrixProjectionSkyDome.Transpose();

	// カメラ情報をGPU側へ送る
	m_DeviceContext->UpdateSubresource(g_pCameraInformationBuffer, 0, NULL, &mtx, 0, 0);
}

//=======================================
// 深度ステンシルの有効・無効を設定
//=======================================
void DirectXRender::SetDepthEnable(bool Enable)
{
	if (Enable)
	{
		// 深度テストを有効にするステンシルステートをセット
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
	}
	else
	{
		// 深度テストを無効にするステンシルステートをセット
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);
	}
}

//=======================================
// アルファテストとカバレッジ（ATC）の有効・無効を設定
//=======================================
void DirectXRender::SetATCEnable(bool Enable)
{
	// ブレンドファクター（透明度などの調整に使用）
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
	{
		// アルファテストとカバレッジ (ATC) を有効にするブレンドステートをセット
		m_DeviceContext->OMSetBlendState(g_BlendStateATC, blendFactor, 0xffffffff);
	}
	else
	{
		// 通常のブレンドステートをセット
		m_DeviceContext->OMSetBlendState(g_BlendState[0], blendFactor, 0xffffffff);
	}
}

void DirectXRender::SetBlendState(int nBlendState)
{
	if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_DeviceContext->OMSetBlendState(g_BlendState[nBlendState], blendFactor, 0xffffffff);
	}
}