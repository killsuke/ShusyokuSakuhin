#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<map>
#include	<iostream>
#include	<io.h>
#include	<vector>
#include	<array>
#include	<Windows.h>
#include "Structs/RenderElement.h"
#include "Manager/ShadowManager.h"

//外部ライブラリ
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

// Direct3D解放の簡略化マクロ
#define SAFE_RELEASE(p) { if( NULL != p ) { p->Release(); p = NULL; } }

// ブレンドステート
enum class EBlendState {
	BS_NONE = 0,							// 半透明合成無し
	BS_ALPHABLEND,							// 半透明合成
	BS_ADDITIVE,							// 加算合成
	BS_SUBTRACTION,							// 減算合成
	BS_MULTIPLY,							// 乗算合成
	BS_EFFECT,								// エフェクト用（加算合成）
	MAX_BLENDSTATE
};

enum class ECullingState {
	CULLING_NONE = 0,							// カリング無し
	CULLING_BACK,							// 裏面カリング
	CULLING_FRONT,							// 表面カリング
	MAX_RASTERIZERSTATE
};

// フィルモード
enum class EFillMode {
	FILL_SOLID = 0,
	FILL_WIREFRAME,
	MAX_FILL
};

// 定数バッファの種類
enum class EBufferTypes {

	DEFAULT_DRAW = 0,
	CAMERA,
	LINE_THICKNESS,
	BLUR,
	HIT_FLASH,
	LIGHT,
	MATERIAL,
	MOTION_BLUR,
	BONE,
	OVER_VERTEX,
	GLOW,
	TIME,
	SHADOW,

	MAX
};



// 定数バッファ用構造体
struct ConstBuffer
{
	// 頂点カラー行列
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
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

// パーティクル用（一度描画して処理負荷を考えてから使う）
//struct PointPosition {
//	DirectX::SimpleMath::Vector3 position = DirectX::SimpleMath::Vector3::Zero;
//	float pad = 0.0f;
//};

struct BlurBuffer {
	DirectX::XMFLOAT2 textureSize = DirectX::XMFLOAT2();
	DirectX::XMFLOAT2 blurPad = DirectX::XMFLOAT2();
};

struct MotionBlurBuffer {
	// １フレーム前のワールド変換行列
	DirectX::XMMATRIX matrixPrevWorld = DirectX::XMMatrixIdentity();

	// シェルのインデックス（x）、シェルの数（y）、ブラー強度（z）、使用・未使用（w）
	DirectX::XMFLOAT4 BlurParams = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
};

struct MotionBlurCircularBuffer {
	DirectX::XMFLOAT3 centerPos = DirectX::XMFLOAT3();
	float length = 0.0f;

	DirectX::XMVECTOR prevRot = DirectX::XMQuaternionIdentity();
	DirectX::XMVECTOR currRot = DirectX::XMQuaternionIdentity();

	int shellCount = 0;
	DirectX::XMFLOAT3 padding = DirectX::XMFLOAT3();
};

struct HitFlashBuffer {
	DirectX::XMFLOAT3 hitFlashColor = DirectX::XMFLOAT3();
	float hitFlashPower = 0.0f;
	BOOL isHit = false;
	DirectX::XMFLOAT3 pad = DirectX::XMFLOAT3();
};

struct GlowBuffer {
	DirectX::XMFLOAT4 glowColor = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT2 ellipseScale = DirectX::XMFLOAT2(0.0f, 0.0f);
	float glowPower = 0.0f;
	float glowRadius = 0.0f;
	float angleRadian = 0.0f;
	float expand = 0.0f;
	float pad1 = 0.0f;
	float pad2 = 0.0f;
};

struct RingGlowBuffer {
	DirectX::XMFLOAT4 glowColor = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT2 ellipseScale = DirectX::XMFLOAT2(0.0f, 0.0f);
	float glowPower = 0.0f;
	float glowRadius = 0.0f;
	float angleRadian = 0.0f;
	float expand = 0.0f;
	float ringRadius = 0.0f;
	float ringWidth = 0.0f;
};

struct TimeBuffer {
	float totalTime = 0.0f;
	float deltaTime = 0.0f;
	DirectX::XMFLOAT2 pad = DirectX::XMFLOAT2();
};

class DirectXRender
{
private:

	static inline ID3D11Device* m_Device = nullptr;				// デバイス＝DirectXの各種機能を作る
	static inline IDXGISwapChain* m_SwapChain = nullptr;			// スワップチェイン＝ダブルバッファ機能
	static inline ID3D11DeviceContext* m_DeviceContext = nullptr; // コンテキスト＝描画関連を司る機能
	static inline ID3D11DepthStencilState* m_DepthStateEnable = nullptr;
	static inline ID3D11DepthStencilState* m_DepthStateDisable = nullptr;
	static inline ID3D11RenderTargetView* m_RenderTargetView = nullptr;	// レンダーターゲット＝描画先を表す機能
	static inline ID3D11DepthStencilView* m_DepthStencilView = nullptr;	// デプスバッファ
	static inline CameraMatrix m_CameraMatrix = { DirectX::XMMatrixIdentity() };			// カメラ行列


	// ラスタライザーステート
	static inline ID3D11RasterizerState* m_SolidRasterizerNone = nullptr;
	static inline ID3D11RasterizerState* m_SolidRasterizerCullBack = nullptr;
	static inline ID3D11RasterizerState* m_SolidRasterizerCullFront = nullptr;

	static inline ID3D11RasterizerState* m_WireFrameRasterizerNone = nullptr;
	static inline ID3D11RasterizerState* m_WireFrameRasterizerCullBack = nullptr;
	static inline ID3D11RasterizerState* m_WireFrameRasterizerCullFront = nullptr;

	static inline ECullingState m_CullingState = ECullingState::CULLING_NONE;
	static inline EFillMode m_FillMode = EFillMode::FILL_SOLID;

	static inline FLOAT m_ClearColor[4] = { 0.4f,0.75f, 1.0f, 1.0f };

	static inline EBlendState m_CurrentBlendState = EBlendState::BS_ALPHABLEND;
	static inline ID3D11BlendState* m_BlendState[(int)(EBlendState::MAX_BLENDSTATE)] = { nullptr }; // ブレンド ステート;

	static inline bool m_IsDepthEnable = false;

	static inline D3D_FEATURE_LEVEL m_FeatureLevel;
	static inline ID3D11BlendState* m_BlendStateATC = nullptr;


	static inline ID3D11SamplerState* m_Sampler = nullptr;		// サンプラー用変数

	// ブラー用バッファとモーションブラー用バッファは別途用意する
	// モーションブラー用バッファは実験中の為

	static inline ID3D11Buffer* m_DefaultDrawBuffer = nullptr;		// デフォルト描画用定数バッファ、０番目
	static inline ID3D11Buffer* m_CameraInformationBuffer = nullptr; // カメラ関係の行列をまとめたバッファ、１番目
	static inline ID3D11Buffer* m_LineThicknessBuffer = nullptr; // 線の太さ・描画モード用定数バッファ、２番目
	static inline ID3D11Buffer* m_BlurBuffer = nullptr;				// ブラー用バッファ、３番目
	static inline ID3D11Buffer* m_HitFlashBuffer = nullptr;		// ヒットフラッシュ用のバッファ、４番目
	static inline ID3D11Buffer* m_LightBuffer = nullptr;		// ライト用定数バッファ、５番目
	static inline ID3D11Buffer* m_MaterialBuffer = nullptr;		// マテリアル用定数バッファ、６番目
	static inline ID3D11Buffer* m_MotionBlurBuffer = nullptr;	// 縦横のみモーションブラー用バッファ、７番目
	static inline ID3D11Buffer* m_MotionBlurCircularBuffer = nullptr;	// 自由に使えるモーションブラー用バッファ（開発中）、７番目
	static inline ID3D11Buffer* m_BoneConstantBuffer = nullptr;		// ボーン用バッファ、８番目（このプロジェクトでは使わない）

	static inline ID3D11Buffer* m_OverVertexConstantBuffer = nullptr;	// HPバー用バッファ、９番目
	static inline ID3D11Buffer* m_GlowBuffer = nullptr;				// グロー用定数バッファ、１０番目
	static inline ID3D11Buffer* m_RingGlowBuffer = nullptr;			// リンググロー用定数バッファ、１０番目
	static inline ID3D11Buffer* m_TimeBuffer = nullptr;				// 時間管理用定数バッファ、１１番目
	static inline ID3D11Buffer* m_ShadowBuffer = nullptr;				// 丸影用バッファ、１２番目

	DirectXRender() = default;
	~DirectXRender() = default;

	// コピー・ムーブも削除
	DirectXRender(const DirectXRender&) = delete;
	DirectXRender(DirectXRender&&) = delete;
	DirectXRender& operator=(const DirectXRender&) = delete;
	DirectXRender& operator=(DirectXRender&&) = delete;

	static HRESULT DeviceAndSwapCreate();
	static HRESULT RenderTargetCreate();
	static HRESULT DepthStencilCreate();
	static void ViewportCreate();
	static HRESULT RasterizerSetting();
	static HRESULT BlandStateCreate();
	static HRESULT DepthStencilSetting();
	static HRESULT SamplerCreate();
	static HRESULT DefaultDrawConstantBufferCreate();
	static HRESULT BoneConstantBufferCreate();
	static HRESULT HPBarConstantBufferCreate();
	static HRESULT LightBufferCreate();
	static void LightSetting();
	static HRESULT MaterialBufferCreate();
	static void MaterialSetting();

	static HRESULT CreateCameraBuffer();
	static HRESULT CreateLineThicknessBuffer();
	static HRESULT CreateBlurBuffer();
	static HRESULT CreateHitFlashBuffer();
	static HRESULT CreateMotionBlurBuffer();
	static HRESULT CreateGlowBuffer();
	static HRESULT CreateTimeBuffer();
	static HRESULT CreateShadowBuffer();

public:

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
	static void SetClearColor(const FLOAT colors[4]) {
		m_ClearColor[0] = colors[0];
		m_ClearColor[1] = colors[1];
		m_ClearColor[2] = colors[2];
		m_ClearColor[3] = colors[3];
	}

	static void SetFullscreen(const BOOL flag) {

		m_SwapChain->SetFullscreenState(flag, NULL);
	};

	static ID3D11DeviceContext* GetDeviceContext() { return m_DeviceContext; };
	static ID3D11Device* GetDevice() { return m_Device; };
	static ID3D11RenderTargetView* GetRenderTargetView() { return m_RenderTargetView; };
	static ID3D11DepthStencilView* GetDepthStencilView() { return m_DepthStencilView; };

	static ID3D11DepthStencilState* GetDepthStateEnable() { return m_DepthStateEnable; };
	static ID3D11DepthStencilState* GetDepthStateDisable() { return m_DepthStateDisable; };

	static ID3D11Buffer* GetLineThicknessBuffer() { return m_LineThicknessBuffer; };

	static FLOAT* GetClearColor() { return m_ClearColor; };

	static EBlendState GetCurrentBlendState() { return m_CurrentBlendState; };
	static EFillMode GetFillMode() { return m_FillMode; };
	static ECullingState GetCullingState() { return m_CullingState; };
	static bool GetIsDepthEnable() { return m_IsDepthEnable; };

	static ID3D11Buffer* GetDefaultDrawBuffer() { return m_DefaultDrawBuffer; }
	static ID3D11Buffer* GetBoneBuffer() { return m_BoneConstantBuffer; };
	static ID3D11Buffer* GetOverVertexMoveBuffer() { return m_OverVertexConstantBuffer; };
	static ID3D11Buffer* GetMaterialBuffer() { return m_MaterialBuffer; };
	static ID3D11Buffer* GetBlurBuffer() { return m_BlurBuffer; };
	static ID3D11Buffer* GetMotionBlurBuffer() { return m_MotionBlurBuffer; };
	static ID3D11Buffer* GetMotionBlurCircularBuffer() { return m_MotionBlurCircularBuffer; };
	static ID3D11Buffer* GetGlowBuffer() { return m_GlowBuffer; };
	static ID3D11Buffer* GetRingGlowBuffer() { return m_RingGlowBuffer; };
	static ID3D11Buffer* GetTimeBuffer() { return m_TimeBuffer; };
	static ID3D11Buffer* GetShadowBuffer() { return m_ShadowBuffer; };
	static ID3D11SamplerState* GetSampler() { return m_Sampler; };

	//=============================================================================
	// ブレンド ステート設定
	//=============================================================================
	static void SetBlendState(const EBlendState& nBlendState);

	static void SetCullingState(const ECullingState& state);
	static void SetFillMode(const EFillMode& fillMode);
	static void SwitchingFillMode();

	static void SetBorderlessFullScreen();
	static void SetWindowed();
	static void OnResize(const UINT& width,const UINT& height);
};