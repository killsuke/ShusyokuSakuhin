#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<SimpleMath.h>
#include	<map>
#include	<iostream>
#include	<io.h>
#include	<vector>
#include	<Windows.h>
#include "RenderElement.h"

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
extern ID3D11Buffer* g_pBoneConstantBuffer;
extern ID3D11Buffer* g_pHPBarConstantBuffer;
extern ID3D11BlendState* g_BlendState[MAX_BLENDSTATE]; // ブレンド ステート;

// 定数バッファ用構造体
struct ConstBuffer
{
	// 頂点カラー行列
	DirectX::SimpleMath::Vector4 color = DirectX::SimpleMath::Vector4::Zero;;
	// UV座標移動行列
	DirectX::SimpleMath::Matrix matrixTex = DirectX::SimpleMath::Matrix();
	// ワールド変換行列
	DirectX::SimpleMath::Matrix matrixWorld = DirectX::SimpleMath::Matrix();

	BOOL inverse = FALSE; // 反転フラグ

	int padding[3] = {}; // パディング
};

struct CameraMatrix
{
	// ３Ｄオブジェクト用
	// ビュー変換行列
	DirectX::SimpleMath::Matrix matrixView3D = DirectX::SimpleMath::Matrix();
	// 射影変換行列
	DirectX::SimpleMath::Matrix matrixProjection3D = DirectX::SimpleMath::Matrix();

	// ２Ｄオブジェクト用
	// ビュー変換行列
	DirectX::SimpleMath::Matrix matrixView2D = DirectX::SimpleMath::Matrix();
	// 射影変換行列
	DirectX::SimpleMath::Matrix matrixProjection2D = DirectX::SimpleMath::Matrix();

	// スカイドーム用
	// ビュー変換行列
	DirectX::SimpleMath::Matrix matrixViewSkyDome = DirectX::SimpleMath::Matrix();
	// 射影変換行列
	DirectX::SimpleMath::Matrix matrixProjectionSkyDome = DirectX::SimpleMath::Matrix();
};

struct LineThickness
{
	float thickness = 1.0f;
	float pad[3] = { 0.0f };
};

class DirectXRender final
{
private:

	ID3D11Device* m_Device;				// デバイス＝DirectXの各種機能を作る
	IDXGISwapChain* m_SwapChain;			// スワップチェイン＝ダブルバッファ機能
	ID3D11DeviceContext* m_DeviceContext;	// コンテキスト＝描画関連を司る機能
	ID3D11DepthStencilState* m_DepthStateEnable;
	ID3D11DepthStencilState* m_DepthStateDisable;
	ID3D11RenderTargetView* g_pRenderTargetView;	// レンダーターゲット＝描画先を表す機能
	ID3D11DepthStencilView* g_pDepthStencilView;	// デプスバッファ
	ID3D11Buffer* m_LightBuffer;
	ID3D11Buffer* m_MaterialBuffer;
	CameraMatrix m_CameraMatrix;			// カメラ行列
	ID3D11Buffer* g_pLineThicknessBuffer; // 線の太さ

	DirectXRender();
	~DirectXRender();

	HRESULT DeviceAndSwapCreate();
	HRESULT RenderTargetCreate();
	HRESULT DepthStencilCreate();
	void ViewportCreate();
	HRESULT InputLayoutAndShadersCreate();
	HRESULT RasterizerSetting();
	HRESULT BlandStateCreate();
	HRESULT DepthStencilSetting();
	HRESULT SamplerCreate();
	HRESULT ConstantBufferCreate();
	HRESULT BoneConstantBufferCreate();
	HRESULT HPBarConstantBufferCreate();
	HRESULT LightBufferCreate();
	void LightSetting();
	HRESULT MaterialBufferCreate();
	void MaterialSetting();

	HRESULT CreateVertexShader(ID3D11Device* device, const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel,
		D3D11_INPUT_ELEMENT_DESC* layout, unsigned int numElements, ID3D11VertexShader** ppVertexShader, ID3D11InputLayout** ppVertexLayout);
	HRESULT CreatePixelShader(ID3D11Device* device, const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11PixelShader** ppPixelShader);
	HRESULT CompileShader(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, void** ShaderObject, size_t& ShaderObjectSize, ID3DBlob** ppBlobOut);

	HRESULT VeiwProjConstantCreate();

public:
	
	static DirectXRender& GetInstance();

	HRESULT Init();
	void UnInit();

	void DrawBegin();
	void DrawEnd();

	void SetViewMatrix3D(DirectX::SimpleMath::Matrix* ViewMatrix);
	void SetViewMatrix2D(DirectX::SimpleMath::Matrix* ViewMatrix);
	void SetViewMatrixSkyDome(DirectX::SimpleMath::Matrix* ViewMatrix);
	void SetProjectionMatrix3D(DirectX::SimpleMath::Matrix* ProjectionMatrix);
	void SetProjectionMatrix2D(DirectX::SimpleMath::Matrix* ProjectionMatrix);
	void SetProjectionMatrixSkyDome(DirectX::SimpleMath::Matrix* ProjectionMatrix);
	void GPU_UpdateViewAndProj();
	void SetDepthEnable(bool Enable);
	void SetATCEnable(bool Enable);
	ID3D11DeviceContext* GetDeviceContext() { return m_DeviceContext; };
	ID3D11Device* GetDevice() { return m_Device; };
	ID3D11RenderTargetView* GetRenderTargetView() { return g_pRenderTargetView; };
	ID3D11DepthStencilView* GetDepthStencilView() { return g_pDepthStencilView; };

	ID3D11DepthStencilState* GetDepthStateEnable() { return m_DepthStateEnable; };
	ID3D11DepthStencilState* GetDepthStateDisable() { return m_DepthStateDisable; };

	ID3D11Buffer* GetLineThicknessBuffer() { return g_pLineThicknessBuffer; };

	//=============================================================================
	// ブレンド ステート設定
	//=============================================================================
	void SetBlendState(int nBlendState);
};

