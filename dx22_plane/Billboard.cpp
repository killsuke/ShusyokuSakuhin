#include "Billboard.h"
#include <iostream>
using namespace DirectX::SimpleMath;

// コンストラクタ
Billboard::Billboard(Camera* cam) :Object(cam) {
}

// デストラクタ
Billboard::~Billboard() {

}

//=======================================
//初期化処理
//=======================================
void Billboard::Init()
{
	// 頂点データ
	std::vector<VERTEX_3D>	vertices;

	vertices.resize(4);

	vertices[0].position = Vector3(-1, 1, -1);
	vertices[1].position = Vector3(1, 1, -1);
	vertices[2].position = Vector3(-1, -1, -1);
	vertices[3].position = Vector3(1, -1, -1);

	vertices[0].color = Color(1, 1, 1, 1);
	vertices[1].color = Color(1, 1, 1, 1);
	vertices[2].color = Color(1, 1, 1, 1);
	vertices[3].color = Color(1, 1, 1, 1);

	vertices[0].uv = Vector2(0, 0);
	vertices[1].uv = Vector2(1, 0);
	vertices[2].uv = Vector2(0, 1);
	vertices[3].uv = Vector2(1, 1);

	vertices[0].normal = Vector3(0, 1, 0);
	vertices[1].normal = Vector3(0, 1, 0);
	vertices[2].normal = Vector3(0, 1, 0);
	vertices[3].normal = Vector3(0, 1, 0);

	// 頂点バッファ生成
	m_VertexBuffer.Create(vertices);

	// インデックスバッファ生成
	std::vector<unsigned int> indices;
	indices.resize(4);

	// インデックスバッファの設定
	indices = {
		0,1,2,
		3,
	};

	// インデックスバッファ生成
	m_IndexBuffer.Create(indices);

	// シェーダオブジェクト生成
	//m_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	m_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	m_Shader.Create("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");

	bool sts = m_Texture.Load("assets/texture/cars_title.png");

	assert(sts == true);

	m_Position.x = 0.0f;
	m_Position.y = 20.0f;
	m_Position.z = 0.0f;

	m_Scale.x = 18.0f;
	m_Scale.y = 10.0f;

	m_Rotation.x = 180.0f;

	splitX = 2.0f;
	splitY = 2.0f;
}


//=======================================
//更新処理
//=======================================
void Billboard::Update()
{
}

//=======================================
//描画処理
//=======================================
void Billboard::Draw()
{
	float PitchRadians = DirectX::XMConvertToRadians(m_Rotation.x); // X軸回転
	float YawRadians = DirectX::XMConvertToRadians(m_Rotation.y);     // Y軸回転
	float RollRadians = DirectX::XMConvertToRadians(m_Rotation.z);   // Z軸回転

	// SRT情報作成
	Matrix r = Matrix::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);
	Matrix t = Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	Matrix s = Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);

	Matrix view = RendererSystem::GetBillboardMatrix();

	// サイズが変わらないならもっと軽い方法があったはず
	// ビルボードの行列を取得
	r = view.Invert();

	r._41 = 0.0f;
	r._42 = 0.0f;
	r._43 = 0.0f;

	Matrix worldmtx;
	worldmtx = s * r * t;
	RendererSystem::SetWorldMatrix(&worldmtx); // GPUにセット

	// 描画の処理
	ID3D11DeviceContext* devicecontext;
	devicecontext = RendererSystem::GetDeviceContext();

	// トポロジーをセット（プリミティブタイプ）
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// 頂点の結び方の規則

	m_Shader.SetGPU();
	m_VertexBuffer.SetGPU();
	m_IndexBuffer.SetGPU();
	m_Texture.SetGPU();

//	RendererSystem::SetColors(DirectX::XMFLOAT4{ 1.0f,1.0f,1.0f,1.0f });

	// UVの設定を指定
	float u = numU - 1;
	float v = numV - 1;
	float uw = 1 / splitX;
	float vh = 1 / splitY;

	// UV座標をセット
	RendererSystem::SetUV(u, v, uw, vh);

	// カメラの設定を指定
	m_Camera->SetCamera(0);

	devicecontext->DrawIndexed(
		4,							// 描画するインデックス数（四角形なんで４）
		0,							// 最初のインデックスバッファの位置
		0);
}

//=======================================
//終了処理
//=======================================
void Billboard::Uninit()
{

}
