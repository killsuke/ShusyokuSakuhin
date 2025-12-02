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

enum class ECullingState {
	CULLING_NONE = 0,							// カリング無し
	CULLING_BACK,							// 裏面カリング
	CULLING_FRONT,							// 表面カリング
	MAX_RASTERIZERSTATE
};

enum class EFillMode {
	FILL_SOLID = 0,
	FILL_WIREFRAME,
	MAX_FILL
};

extern ID3D11Buffer* g_pConstantBuffer;
extern ID3D11Buffer* g_pBoneConstantBuffer;
extern ID3D11Buffer* g_pHPBarConstantBuffer;
extern ID3D11Buffer* g_pBlurBuffer;
extern ID3D11Buffer* m_MaterialBuffer;
extern ID3D11BlendState* g_BlendState[MAX_BLENDSTATE]; // ブレンド ステート;

// 定数バッファ用構造体
struct ConstBuffer
{
	// 頂点カラー行列
	DirectX::SimpleMath::Vector4 color = DirectX::SimpleMath::Vector4::Zero;;
	// UV座標移動行列
	DirectX::XMMATRIX matrixTex = DirectX::XMMatrixIdentity();
	// ワールド変換行列
	DirectX::XMMATRIX matrixWorld = DirectX::XMMatrixIdentity();

	BOOL inverse = FALSE; // 反転フラグ

	int padding[3] = {}; // パディング
};

struct CameraMatrix
{
	// ３Ｄオブジェクト用
	// ビュー変換行列
	DirectX::XMMATRIX matrixView3D = DirectX::XMMatrixIdentity();
	// 射影変換行列
	DirectX::XMMATRIX matrixProjection3D = DirectX::XMMatrixIdentity();

	// ２Ｄオブジェクト用
	// ビュー変換行列
	DirectX::XMMATRIX matrixView2D = DirectX::XMMatrixIdentity();
	// 射影変換行列
	DirectX::XMMATRIX matrixProjection2D = DirectX::XMMatrixIdentity();

	// スカイドーム用
	// ビュー変換行列
	DirectX::XMMATRIX matrixViewSkyDome = DirectX::XMMatrixIdentity();
	// 射影変換行列
	DirectX::XMMATRIX matrixProjectionSkyDome = DirectX::XMMatrixIdentity();
};

struct LineThickness
{
	float thickness = 1.0f;
	float pad[3] = { 0.0f };
};

struct BlurBuffer {
	DirectX::SimpleMath::Vector2 textureSize = DirectX::SimpleMath::Vector2::Zero;
	DirectX::SimpleMath::Vector2 blurPad = DirectX::SimpleMath::Vector2::Zero;
};

struct HitFlashBuffer {
	DirectX::SimpleMath::Vector3 hitFlashColor = DirectX::SimpleMath::Vector3::Zero;
	float hitFlashPower = 0.0f;
	BOOL isHit = false;
	DirectX::SimpleMath::Vector3 pad = DirectX::SimpleMath::Vector3::Zero;
};

class DirectXRender
{
private:

	static ID3D11Device* m_Device;				// デバイス＝DirectXの各種機能を作る
	static IDXGISwapChain* m_SwapChain;			// スワップチェイン＝ダブルバッファ機能
	static ID3D11DeviceContext* m_DeviceContext;	// コンテキスト＝描画関連を司る機能
	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;
	static ID3D11RenderTargetView* g_pRenderTargetView;	// レンダーターゲット＝描画先を表す機能
	static ID3D11DepthStencilView* g_pDepthStencilView;	// デプスバッファ
	static ID3D11Buffer* m_LightBuffer;
	static CameraMatrix m_CameraMatrix;			// カメラ行列
	static ID3D11Buffer* g_pLineThicknessBuffer; // 線の太さ

	// ヒットフラッシュ用のバッファ
	static ID3D11Buffer* m_HitFlashBuffer;

	// ラスタライザーステート
	static ID3D11RasterizerState* m_SolidRasterizerNone;
	static ID3D11RasterizerState* m_SolidRasterizerCullBack;
	static ID3D11RasterizerState* m_SolidRasterizerCullFront;

	static ID3D11RasterizerState* m_WireFrameRasterizerNone;
	static ID3D11RasterizerState* m_WireFrameRasterizerCullBack;
	static ID3D11RasterizerState* m_WireFrameRasterizerCullFront;

	static ECullingState m_CullingState;
	static EFillMode m_FillMode;

	static DirectX::XMFLOAT4 m_ClearColor;

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
	static HRESULT BoneConstantBufferCreate();
	static HRESULT HPBarConstantBufferCreate();
	static HRESULT LightBufferCreate();
	static void LightSetting();
	static HRESULT MaterialBufferCreate();
	static void MaterialSetting();

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

	static ID3D11Buffer* GetHitFlashBuffer();

	static void DrawBegin();
	static void DrawEnd();

	static void SetViewMatrix3D(DirectX::XMMATRIX* ViewMatrix);
	static void SetViewMatrix2D(DirectX::XMMATRIX* ViewMatrix);
	static void SetViewMatrixSkyDome(DirectX::XMMATRIX* ViewMatrix);
	static void SetProjectionMatrix3D(DirectX::XMMATRIX* ProjectionMatrix);
	static void SetProjectionMatrix2D(DirectX::XMMATRIX* ProjectionMatrix);
	static void SetProjectionMatrixSkyDome(DirectX::XMMATRIX* ProjectionMatrix);
	static void GPU_UpdateViewAndProj();
	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);

	static ID3D11DeviceContext* GetDeviceContext() { return m_DeviceContext; };
	static ID3D11Device* GetDevice() { return m_Device; };
	static ID3D11RenderTargetView* GetRenderTargetView() { return g_pRenderTargetView; };
	static ID3D11DepthStencilView* GetDepthStencilView() { return g_pDepthStencilView; };

	static ID3D11DepthStencilState* GetDepthStateEnable() { return m_DepthStateEnable; };
	static ID3D11DepthStencilState* GetDepthStateDisable() { return m_DepthStateDisable; };

	static ID3D11Buffer* GetLineThicknessBuffer() { return g_pLineThicknessBuffer; };

	static DirectX::XMFLOAT4 GetClearColor() { return m_ClearColor; };

	//=============================================================================
	// ブレンド ステート設定
	//=============================================================================
	static void SetBlendState(int nBlendState);

	static void SetCullingState(const ECullingState& state);
	static void SetFillMode(const EFillMode& fillMode);
	static void SwitchingFillMode();
};

