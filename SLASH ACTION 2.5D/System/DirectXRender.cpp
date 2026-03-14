#include "System/DirectXRender.h"
#include "System/Application.h"
#include "Structs/BoneData.h"
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <locale.h>
//#include <atltypes.h> // CRectを使うためのヘッダーファイル
#include <io.h>
#include <stdio.h>
#include <string.h>
#include "Structs/HPParam.h"

using namespace DirectX;

namespace {

	constexpr int DEFAULT_SCREEN_POS_X = 100;
	constexpr int DEFAULT_SCREEN_POS_Y = 50;
}

HRESULT DirectXRender::Init() {

	DeviceAndSwapCreate();	// デバイス、スワップチェーン作成
	RenderTargetCreate();	// レンダーターゲットビュー作成
	DepthStencilCreate();	// デプスステンシルバッファ作成
	ViewportCreate();		// ビューポート設定
	RasterizerSetting();
	BlandStateCreate();
	DepthStencilSetting();
	SamplerCreate();

	DefaultDrawConstantBufferCreate();	// 描画用定数バッファ作成、０番目
	CreateCameraBuffer();	// カメラ情報用定数バッファ作成、１番目
	CreateLineThicknessBuffer(); // 線の太さ用定数バッファ作成、２番目
	CreateBlurBuffer(); // ブラー用定数バッファ作成、３番目
	CreateHitFlashBuffer(); // ヒットフラッシュ用定数バッファ作成、４番目

	LightBufferCreate();	// ライトバッファ作成、５番目
	LightSetting();
	MaterialBufferCreate();	// マテリアルバッファ作成、６番目

	CreateMotionBlurBuffer();	// モーションブラー用定数バッファ作成、７番目

	BoneConstantBufferCreate();	// ボーン用定数バッファ作成、８番目

	HPBarConstantBufferCreate();	// ＨＰバー用定数バッファ作成、９番目

	CreateGlowBuffer(); // グロー用定数バッファ作成、１０番目

	CreateTimeBuffer();	// 時間管理用定数バッファ作成、１１番目

	CreateShadowBuffer(); // シャドウ用定数バッファ作成、１２番目

	//MaterialSetting();

	SetBlendState(EBlendState::BS_ALPHABLEND);

#ifdef _DEBUG

	SetWindowed(); // ウィンドウモード設定
#else

	// フルスクリーンボーダレス設定
	SetBorderlessFullScreen();
#endif // _DEBUG

	return S_OK;
}

void DirectXRender::UnInit() {

	m_DeviceContext->ClearState();
	SAFE_RELEASE(m_RenderTargetView);
	SAFE_RELEASE(m_DepthStencilView);
	SAFE_RELEASE(m_DepthStateEnable);
	SAFE_RELEASE(m_DepthStateDisable);
	SAFE_RELEASE(m_SwapChain);
	SAFE_RELEASE(m_Sampler);
	SAFE_RELEASE(m_DefaultDrawBuffer);
	SAFE_RELEASE(m_BoneConstantBuffer);
	SAFE_RELEASE(m_OverVertexConstantBuffer);
	SAFE_RELEASE(m_BlurBuffer);
	SAFE_RELEASE(m_MotionBlurBuffer);
	SAFE_RELEASE(m_MotionBlurCircularBuffer);
	SAFE_RELEASE(m_LightBuffer);
	SAFE_RELEASE(m_MaterialBuffer);
	SAFE_RELEASE(m_GlowBuffer);
	SAFE_RELEASE(m_TimeBuffer);
	SAFE_RELEASE(m_ShadowBuffer);
	SAFE_RELEASE(m_RingGlowBuffer);
	SAFE_RELEASE(m_HitFlashBuffer);
	for (int i = 0; i < (int)(EBlendState::MAX_BLENDSTATE); ++i) {
		if (m_BlendState[i]) {  // nullptr チェック
			SAFE_RELEASE(m_BlendState[i]);
			//g_BlendState[i] = nullptr;  // 解放後にポインタをクリア
		}
	}
	SAFE_RELEASE(m_BlendStateATC);
	SAFE_RELEASE(m_CameraInformationBuffer);
	SAFE_RELEASE(m_LineThicknessBuffer);
	SAFE_RELEASE(m_DeviceContext);
	SAFE_RELEASE(m_Device);
}

ID3D11Buffer* DirectXRender::GetHitFlashBuffer() {
	return m_HitFlashBuffer;
}

//=======================================
//描画開始
//=======================================
void DirectXRender::DrawBegin() {

	// 描画先のキャンバスと使用する深度バッファを指定する
	// レンダーターゲットとデプスステンシルビューを設定
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	// 描画先キャンバスを塗りつぶす
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, m_ClearColor);
	// 深度バッファをリセットする
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// 	インプットレイアウト（GPUに渡す頂点データのフォーマットを定義）を設定
	//m_DeviceContext->IASetInputLayout(g_pInputLayout);
	// どのようにテクスチャを扱うかを設定
	// ポストエフェクト等でどんな描画かを使う場合は、
	// 最初の値（スロット）を１以上にする
	// 次の引数たちも増やすことも考える
	m_DeviceContext->PSSetSamplers(0, 1, &m_Sampler);
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
	if (renderTarget != nullptr)m_Device->CreateRenderTargetView(renderTarget, NULL, &m_RenderTargetView);
	renderTarget->Release();
	if (FAILED(hr)) return hr;

	return hr;
}

HRESULT DirectXRender::DepthStencilCreate() {

	HRESULT hr = S_OK;

	// デプスステンシルバッファ作成
	ID3D11Texture2D* depthStencile = nullptr;
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
	if (depthStencile != nullptr)m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
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
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	hr = m_Device->CreateRasterizerState(&rasterizerDesc, &m_SolidRasterizerCullFront);
	if (FAILED(hr)) return hr;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	hr = m_Device->CreateRasterizerState(&rasterizerDesc, &m_SolidRasterizerNone);
	if (FAILED(hr)) return hr;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	hr = m_Device->CreateRasterizerState(&rasterizerDesc, &m_SolidRasterizerCullBack);
	if (FAILED(hr)) return hr;

	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;	// 試してみよう

	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	hr = m_Device->CreateRasterizerState(&rasterizerDesc, &m_WireFrameRasterizerCullFront);
	if (FAILED(hr)) return hr;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	hr = m_Device->CreateRasterizerState(&rasterizerDesc, &m_WireFrameRasterizerNone);
	if (FAILED(hr)) return hr;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	hr = m_Device->CreateRasterizerState(&rasterizerDesc, &m_WireFrameRasterizerCullBack);
	if (FAILED(hr)) return hr;

	m_CullingState = ECullingState::CULLING_BACK;
	m_FillMode = EFillMode::FILL_SOLID;

	m_DeviceContext->RSSetState(m_SolidRasterizerCullBack);

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

	hr = m_Device->CreateBlendState(&BlendDesc, &m_BlendState[0]);
	if (FAILED(hr)) return hr;

	// ブレンド ステート生成 (アルファ ブレンド用)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	hr = m_Device->CreateBlendState(&BlendDesc, &m_BlendState[1]);
	if (FAILED(hr)) return hr;

	// ブレンド ステート生成 (加算合成用)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	hr = m_Device->CreateBlendState(&BlendDesc, &m_BlendState[2]);
	if (FAILED(hr)) return hr;

	// ブレンド ステート生成 (減算合成用)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	hr = m_Device->CreateBlendState(&BlendDesc, &m_BlendState[3]);
	if (FAILED(hr)) return hr;

	// ブレンド ステート生成 (乗算合成用)
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	hr = m_Device->CreateBlendState(&BlendDesc, &m_BlendState[4]);
	if (FAILED(hr)) return hr;

	// エフェクト用加算合成
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	hr = m_Device->CreateBlendState(&BlendDesc, &m_BlendState[5]);
	if (FAILED(hr)) return hr;

	SetBlendState(EBlendState::BS_ALPHABLEND);

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
	m_IsDepthEnable = true;

	return hr;
}

// サンプラー作成
HRESULT DirectXRender::SamplerCreate() {

	HRESULT hr = S_OK;

	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	//	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	/*samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;*/

	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//	ID3D11SamplerState* samplerState{};
	hr = m_Device->CreateSamplerState(&samplerDesc, &m_Sampler);
	if (FAILED(hr)) return hr;

	return hr;
}

// 描画用の定数バッファ作成、０番目
HRESULT DirectXRender::DefaultDrawConstantBufferCreate() {
	HRESULT hr;
	// 定数バッファ作成
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(ConstBuffer);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	hr = m_Device->CreateBuffer(&cbDesc, NULL, &m_DefaultDrawBuffer);
	if (FAILED(hr)) return hr;

	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::DEFAULT_DRAW), 1, &m_DefaultDrawBuffer);

	return hr;
}

// カメラ情報用定数バッファ作成、１番目
HRESULT DirectXRender::CreateCameraBuffer() {

	HRESULT hr = S_OK;

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(CameraMatrix);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_CameraInformationBuffer);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}
	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::CAMERA), 1, &m_CameraInformationBuffer);

	return hr;
}

// ライン用の定数バッファ作成、２番目
HRESULT DirectXRender::CreateLineThicknessBuffer() {

	HRESULT hr = S_OK;

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(LineThickness);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_LineThicknessBuffer);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}

	m_DeviceContext->GSSetConstantBuffers(UINT(EBufferTypes::CAMERA), 1, &m_CameraInformationBuffer);
	m_DeviceContext->GSSetConstantBuffers(UINT(EBufferTypes::LINE_THICKNESS), 1, &m_LineThicknessBuffer);

	return hr;
}

// ブラー用の定数バッファ作成、３番目
HRESULT DirectXRender::CreateBlurBuffer() {

	HRESULT hr = S_OK;

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(BlurBuffer);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_BlurBuffer);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}
	
	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::BLUR), 1, &m_BlurBuffer);
	m_DeviceContext->PSSetConstantBuffers(UINT(EBufferTypes::BLUR), 1, &m_BlurBuffer);

	return hr;
}

// ヒットフラッシュ用の定数バッファ作成、４番目
HRESULT DirectXRender::CreateHitFlashBuffer() {

	HRESULT hr = S_OK;

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(HitFlashBuffer);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_HitFlashBuffer);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}

	m_DeviceContext->PSSetConstantBuffers(UINT(EBufferTypes::HIT_FLASH), 1, &m_HitFlashBuffer);

	return hr;
}

// ライト用定数バッファ作成、５番目
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
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}

	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::LIGHT), 1, &m_LightBuffer);
	m_DeviceContext->PSSetConstantBuffers(UINT(EBufferTypes::LIGHT), 1, &m_LightBuffer);

	return hr;
}

// マテリアル用定数バッファ作成、６番目
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
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}

	// 頂点＆ピクセルシェーダー両方にセット
	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::MATERIAL), 1, &m_MaterialBuffer);
	m_DeviceContext->PSSetConstantBuffers(UINT(EBufferTypes::MATERIAL), 1, &m_MaterialBuffer);

	return hr;
}

// モーションブラー用定数バッファ作成、７番目
HRESULT DirectXRender::CreateMotionBlurBuffer() {

	HRESULT hr = S_OK;

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	bufferDesc.ByteWidth = sizeof(MotionBlurBuffer);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_MotionBlurBuffer);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}

	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::MOTION_BLUR), 1, &m_MotionBlurBuffer);
	m_DeviceContext->PSSetConstantBuffers(UINT(EBufferTypes::MOTION_BLUR), 1, &m_MotionBlurBuffer);

	bufferDesc.ByteWidth = sizeof(MotionBlurCircularBuffer);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_MotionBlurCircularBuffer);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}
	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::MOTION_BLUR), 1, &m_MotionBlurCircularBuffer);
	m_DeviceContext->PSSetConstantBuffers(UINT(EBufferTypes::MOTION_BLUR), 1, &m_MotionBlurCircularBuffer);

	return hr;
}

// ボーン用定数バッファ作成、８番目
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

	hr = m_Device->CreateBuffer(&bd, nullptr, &m_BoneConstantBuffer);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}

	// 定数バッファを頂点シェーダーにセットする
	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::BONE), 1, &m_BoneConstantBuffer);
	
	return hr;
}

// ＨＰ用の定数バッファ作成、９番目
HRESULT DirectXRender::HPBarConstantBufferCreate() {// コンスタントバッファサイズ
	HRESULT hr;

	// ボーン用の定数バッファ作成
	D3D11_BUFFER_DESC bd;

	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(OverVertexParam);									// バッファの大き
	bd.Usage = D3D11_USAGE_DEFAULT;							// バッファ使用方法
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;					// コンスタントバッファ
	bd.CPUAccessFlags = 0;					// CPUアクセス可能

	hr = m_Device->CreateBuffer(&bd, nullptr, &m_OverVertexConstantBuffer);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}

	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::OVER_VERTEX), 1, &m_OverVertexConstantBuffer);

	return hr;
}

HRESULT DirectXRender::CreateGlowBuffer() {

	HRESULT hr = S_OK;

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	bufferDesc.ByteWidth = sizeof(GlowBuffer);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_GlowBuffer);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}

	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::GLOW), 1, &m_GlowBuffer);
	m_DeviceContext->PSSetConstantBuffers(UINT(EBufferTypes::GLOW), 1, &m_GlowBuffer);

	bufferDesc.ByteWidth = sizeof(RingGlowBuffer);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_RingGlowBuffer);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}

	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::GLOW), 1, &m_RingGlowBuffer);
	m_DeviceContext->PSSetConstantBuffers(UINT(EBufferTypes::GLOW), 1, &m_RingGlowBuffer);

	return hr;
}

HRESULT DirectXRender::CreateTimeBuffer() {

	HRESULT hr = S_OK;

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	bufferDesc.ByteWidth = sizeof(TimeBuffer);

	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_TimeBuffer);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}

	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::TIME), 1, &m_TimeBuffer);
	m_DeviceContext->PSSetConstantBuffers(UINT(EBufferTypes::TIME), 1, &m_TimeBuffer);

	return hr;
}

HRESULT DirectXRender::CreateShadowBuffer() {
	HRESULT hr = S_OK;
	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);
	bufferDesc.ByteWidth = sizeof(ShadowBuffer);
	hr = m_Device->CreateBuffer(&bufferDesc, NULL, &m_ShadowBuffer);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateBuffer(constant buffer) error", L"Error", MB_OK);
		return hr;
	}
	m_DeviceContext->VSSetConstantBuffers(UINT(EBufferTypes::SHADOW), 1, &m_ShadowBuffer);
	m_DeviceContext->PSSetConstantBuffers(UINT(EBufferTypes::SHADOW), 1, &m_ShadowBuffer);
	return hr;
}

void DirectXRender::LightSetting() {
	// ライト初期化
	LIGHT light{};
	light.Direction = XMFLOAT4(0.0f, -1.0f, 0.2f, 0.0f);	// 方向
	XMVECTOR lightDir = XMLoadFloat4(&light.Direction);
	lightDir = XMVector4Normalize(lightDir);
	XMStoreFloat4(&light.Direction, lightDir);
	light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// 平行光源の強さと色
	light.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);	// 環境光の強さと色

	m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &light, 0, 0);
}

void DirectXRender::MaterialSetting() {
	// マテリアル初期化
	MATERIAL material{};
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DeviceContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &material, 0, 0);
}

//=======================================
// ビュー行列を設定（３Ｄ）
//=======================================
void DirectXRender::SetViewMatrix3D(DirectX::XMMATRIX* ViewMatrix)
{
	m_CameraMatrix.matrixView3D = *ViewMatrix; // 転置
}

//=======================================
// プロジェクション行列を設定（３Ｄ）
//=======================================
void DirectXRender::SetProjectionMatrix3D(DirectX::XMMATRIX* ProjectionMatrix)
{
	m_CameraMatrix.matrixProjection3D = *ProjectionMatrix; // 転置
}

//=======================================
// ビュー行列を設定（２Ｄ）
//=======================================
void DirectXRender::SetViewMatrix2D(DirectX::XMMATRIX* ViewMatrix)
{
	m_CameraMatrix.matrixView2D = *ViewMatrix; // 転置
}

//=======================================
// プロジェクション行列を設定（２Ｄ）
//=======================================
void DirectXRender::SetProjectionMatrix2D(DirectX::XMMATRIX* ProjectionMatrix)
{
	m_CameraMatrix.matrixProjection2D = *ProjectionMatrix; // 転置
}

//=======================================
// ビュー行列を設定（スカイドーム用）
//=======================================
void DirectXRender::SetViewMatrixSkyDome(DirectX::XMMATRIX* ViewMatrix)
{
	m_CameraMatrix.matrixViewSkyDome = *ViewMatrix; // 転置
}

//=======================================
// プロジェクション行列を設定（スカイドーム用）
//=======================================
void DirectXRender::SetProjectionMatrixSkyDome(DirectX::XMMATRIX* ProjectionMatrix)
{
	m_CameraMatrix.matrixProjectionSkyDome = *ProjectionMatrix; // 転置
}

//=======================================
// カメラ情報をGPUへ送る
//=======================================
void DirectXRender::GPU_UpdateViewAndProj() {
	CameraMatrix mtx = m_CameraMatrix;
	mtx.matrixView3D = XMMatrixTranspose(m_CameraMatrix.matrixView3D);
	mtx.matrixProjection3D = XMMatrixTranspose(m_CameraMatrix.matrixProjection3D);
	mtx.matrixView2D = XMMatrixTranspose(m_CameraMatrix.matrixView2D);
	mtx.matrixProjection2D = XMMatrixTranspose(m_CameraMatrix.matrixProjection2D);
	mtx.matrixViewSkyDome = XMMatrixTranspose(m_CameraMatrix.matrixViewSkyDome);
	mtx.matrixProjectionSkyDome = XMMatrixTranspose(m_CameraMatrix.matrixProjectionSkyDome);

	// カメラ情報をGPU側へ送る
	m_DeviceContext->UpdateSubresource(m_CameraInformationBuffer, 0, NULL, &mtx, 0, 0);
}

//=======================================
// 深度ステンシルの有効・無効を設定
//=======================================
void DirectXRender::SetDepthEnable(bool Enable)
{
	if (Enable == true)
	{
		// 深度テストを有効にするステンシルステートをセット
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
	}
	else
	{
		// 深度テストを無効にするステンシルステートをセット
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);
	}

	m_IsDepthEnable = Enable;
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
		m_DeviceContext->OMSetBlendState(m_BlendStateATC, blendFactor, 0xffffffff);
	}
	else
	{
		// 通常のブレンドステートをセット
		m_DeviceContext->OMSetBlendState(m_BlendState[0], blendFactor, 0xffffffff);
	}
}

void DirectXRender::SetBlendState(const EBlendState& nBlendState)
{
	if (nBlendState < EBlendState::MAX_BLENDSTATE) {
		const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		int result = 0;

		switch (nBlendState)
		{
		case EBlendState::BS_NONE:
			break;
		case EBlendState::BS_ALPHABLEND:
			result = 1;
			break;
		case EBlendState::BS_ADDITIVE:
			result = 2;
			break;
		case EBlendState::BS_SUBTRACTION:
			result = 3;
			break;
		case EBlendState::BS_MULTIPLY:
			result = 4;
			break;
		case EBlendState::BS_EFFECT:
			result = 5;
			break;
		default:
			break;
		}

		m_CurrentBlendState = nBlendState;
		m_DeviceContext->OMSetBlendState(m_BlendState[result], blendFactor, 0xffffffff);
	}
}

void DirectXRender::SetCullingState(const ECullingState& state) {

	switch (state)
	{
	case ECullingState::CULLING_NONE:

		if (m_FillMode == EFillMode::FILL_WIREFRAME) {
			m_DeviceContext->RSSetState(m_WireFrameRasterizerNone);
		}
		else if (m_FillMode == EFillMode::FILL_SOLID) {
			m_DeviceContext->RSSetState(m_SolidRasterizerNone);
		}

		m_CullingState = ECullingState::CULLING_NONE;
		break;
	case ECullingState::CULLING_BACK:

		if (m_FillMode == EFillMode::FILL_WIREFRAME) {
			m_DeviceContext->RSSetState(m_WireFrameRasterizerCullBack);
		}
		else if (m_FillMode == EFillMode::FILL_SOLID) {
			m_DeviceContext->RSSetState(m_SolidRasterizerCullBack);
		}

		m_CullingState = ECullingState::CULLING_BACK;
		break;
	case ECullingState::CULLING_FRONT:

		if (m_FillMode == EFillMode::FILL_WIREFRAME) {
			m_DeviceContext->RSSetState(m_WireFrameRasterizerCullFront);
		}
		else if (m_FillMode == EFillMode::FILL_SOLID) {
			m_DeviceContext->RSSetState(m_SolidRasterizerCullFront);
		}

		m_CullingState = ECullingState::CULLING_FRONT;
		break;
	default:
		break;
	}
}

void DirectXRender::SetFillMode(const EFillMode& fillMode) {

	switch (fillMode)
	{
	case EFillMode::FILL_SOLID:

		if (m_CullingState == ECullingState::CULLING_NONE) {
			m_DeviceContext->RSSetState(m_SolidRasterizerNone);
		}
		else if (m_CullingState == ECullingState::CULLING_BACK) {
			m_DeviceContext->RSSetState(m_SolidRasterizerCullBack);
		}
		else if (m_CullingState == ECullingState::CULLING_FRONT) {
			m_DeviceContext->RSSetState(m_SolidRasterizerCullFront);
		}

		m_FillMode = EFillMode::FILL_SOLID;
		break;
	case EFillMode::FILL_WIREFRAME:

		if (m_CullingState == ECullingState::CULLING_NONE) {
			m_DeviceContext->RSSetState(m_WireFrameRasterizerNone);
		}
		else if (m_CullingState == ECullingState::CULLING_BACK) {
			m_DeviceContext->RSSetState(m_WireFrameRasterizerCullBack);
		}
		else if (m_CullingState == ECullingState::CULLING_FRONT) {
			m_DeviceContext->RSSetState(m_WireFrameRasterizerCullFront);
		}

		m_FillMode = EFillMode::FILL_WIREFRAME;
		break;
	default:
		break;
	}
}

// モデルの表示状態を変更
void DirectXRender::SwitchingFillMode() {

	if (m_FillMode == EFillMode::FILL_SOLID) {
		SetFillMode(EFillMode::FILL_WIREFRAME);
	}
	else if (m_FillMode == EFillMode::FILL_WIREFRAME) {
		SetFillMode(EFillMode::FILL_SOLID);
	}
}

void DirectXRender::SetBorderlessFullScreen() {

	const HWND hWnd = Application::GetWindow();

	// モニター情報を取得
	const HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi = {};
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor, &mi);

	// ウィンドウハンドルスタイル変更（枠なし）
	SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

	// モニターサイズに合わせる
	SetWindowPos(
		hWnd,
		HWND_TOP,
		mi.rcMonitor.left,
		mi.rcMonitor.top,
		mi.rcMonitor.right - mi.rcMonitor.left,
		mi.rcMonitor.bottom - mi.rcMonitor.top,
		SWP_FRAMECHANGED
	);
}

void DirectXRender::SetWindowed() {

	const HWND hWnd = Application::GetWindow();
	const int width = Application::GetWidth();
	const int height = Application::GetHeight();

	// ウィンドウハンドルスタイル変更（枠あり）
	SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	SetWindowPos(
		hWnd,
		nullptr,
		DEFAULT_SCREEN_POS_X, DEFAULT_SCREEN_POS_Y,
		width,
		height,
		SWP_FRAMECHANGED
	);
}

void DirectXRender::OnResize(const UINT& width, const UINT& height) {

	if (m_SwapChain == nullptr) {
		return;
	}

	HRESULT hr = S_OK;

	m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	SAFE_RELEASE(m_RenderTargetView);
	SAFE_RELEASE(m_DepthStencilView);

	// バックバッファサイズ変更
	hr = m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"ResizeBuffers error", L"Error", MB_OK);
		return;
	}

	// バックバッファ再取得
	ID3D11Texture2D* backBuffer = nullptr;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"GetBuffer error", L"Error", MB_OK);
		return;
	}

	// レンダーターゲットビュー再作成
	hr = m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateRenderTargetView error", L"Error", MB_OK);
		return;
	}

	backBuffer->Release();

	// デプスステンシルバッファ再作成
	ID3D11Texture2D* depthStencile = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = m_Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencile);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateTexture2D error", L"Error", MB_OK);
		return;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = depthStencilDesc.Format; // デプスステンシルバッファのフォーマットを設定
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // ビューの次元を2Dテクスチャとして設定（2Dテクスチャ用のデプスステンシルビュー）
	depthStencilViewDesc.Flags = 0; // 特別なフラグは設定しない（デフォルトの動作）

	// デプスステンシルビュー再作成
	hr = m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateDepthStencilView error", L"Error", MB_OK);
		return;
	}

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	depthStencile->Release();

	// ビューポート再設定
	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<FLOAT>(width);
	vp.Height = static_cast<FLOAT>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_DeviceContext->RSSetViewports(1, &vp);
}