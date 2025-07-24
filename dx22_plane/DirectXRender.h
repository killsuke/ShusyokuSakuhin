#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<SimpleMath.h>
#include	<map>
#include	<iostream>
#include	<io.h>
#include	<vector>

//外部ライブラリ
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

// Direct3D解放の簡略化マクロ
#define SAFE_RELEASE(p) { if( NULL != p ) { p->Release(); p = NULL; } }

// ブレンドステート
enum EBlendState {
	BS_NONE = 0,							// 半透明合成無し
	BS_ALPHABLEND,							// 半透明合成
	BS_ADDITIVE,							// 加算合成
	BS_SUBTRACTION,							// 減算合成
	MAX_BLENDSTATE
};

extern ID3D11Buffer* g_pConstantBuffer;
extern ID3D11BlendState* g_BlendState[MAX_BLENDSTATE]; // ブレンド ステート;

// 定数バッファ用構造体
struct ConstBuffer
{
	// 頂点カラー行列
	DirectX::XMFLOAT4 color;
	// UV座標移動行列
	DirectX::XMMATRIX matrixTex;
	// プロジェクション変換行列
	DirectX::XMMATRIX matrixProj;
	// ワールド変換行列
	DirectX::XMMATRIX matrixWorld;
	// ビュー変換行列
	DirectX::XMMATRIX matrixView;
};

class DirectXRender
{
private:

	static ID3D11Device* m_Device;				// デバイス＝DirectXの各種機能を作る
	static IDXGISwapChain* m_SwapChain;			// スワップチェイン＝ダブルバッファ機能
	static ID3D11DeviceContext* m_DeviceContext;	// コンテキスト＝描画関連を司る機能
	
	static HRESULT DeviceAndSwapCreate();
	static HRESULT RenderTargetCreate();
	static HRESULT DepthStencilCreate();
	static void ViewportCreate();
	HRESULT InputLayoutAndShadersCreate();
	static HRESULT RasterizerSetting();
	static HRESULT BlandStateCreate();
	static HRESULT DepthStencilSetting();
	static HRESULT SamplerCreate();
	static HRESULT ConstantBufferCreate();

	HRESULT CreateVertexShader(ID3D11Device* device, const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel,
	D3D11_INPUT_ELEMENT_DESC* layout, unsigned int numElements, ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout);
	HRESULT CreatePixelShader(ID3D11Device* device, const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11PixelShader** ppPixelShader);
	HRESULT CompileShader(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, void** ShaderObject, size_t& ShaderObjectSize, ID3DBlob** ppBlobOut);

	static HRESULT VeiwProjConstantCreate();

public:
	DirectXRender();
	~DirectXRender();

	static HRESULT Init();
	static void UnInit();

	static void DrawBegin();
	static void DrawEnd();

	static void SetViewMatrix(DirectX::SimpleMath::Matrix* ViewMatrix);
	static void SetProjectionMatrix(DirectX::SimpleMath::Matrix* ProjectionMatrix);
	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static ID3D11DeviceContext* GetDeviceContext() { return m_DeviceContext; };
	static ID3D11Device* GetDevice() { return m_Device; };

	//=============================================================================
	// ブレンド ステート設定
	//=============================================================================
	static void SetBlendState(int nBlendState);
};

