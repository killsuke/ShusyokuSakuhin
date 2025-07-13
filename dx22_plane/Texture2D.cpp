#include "Texture2D.h"

using namespace std;
using namespace DirectX::SimpleMath;

// コンストラクタ
Texture2D::Texture2D(Camera* cam) :GameObject(cam)
{

}

// デストラクタ
Texture2D::~Texture2D()
{

}

//=======================================
// 初期化処理
//=======================================
void Texture2D::Init()
{
	// 頂点データ
	std::vector<VERTEX_3D> vertices;

	vertices.resize(4);

	vertices[0].position = Vector3(-0.5f, 0.5f, 0);
	vertices[1].position = Vector3(0.5f, 0.5f, 0);
	vertices[2].position = Vector3(-0.5f, -0.5f, 0);
	vertices[3].position = Vector3(0.5f, -0.5f, 0);

	vertices[0].color = Color(1, 1, 1, 1);
	vertices[1].color = Color(1, 1, 1, 1);
	vertices[2].color = Color(1, 1, 1, 1);
	vertices[3].color = Color(1, 1, 1, 1);

	vertices[0].uv = Vector2(0, 0);
	vertices[1].uv = Vector2(1, 0);
	vertices[2].uv = Vector2(0, 1);
	vertices[3].uv = Vector2(1, 1);

	// 頂点バッファ生成
	m_VertexBuffer.Create(vertices);

	// インデックスバッファ生成
	std::vector<unsigned int> indices;
	indices.resize(4);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;

	// インデックスバッファ生成
	m_IndexBuffer.Create(indices);

	// シェーダオブジェクト生成
	m_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");

	// マテリアル情報取得
	m_Materiale = std::make_unique<Material>();
	MATERIAL mtrl;
	mtrl.Diffuse = Color(1, 1, 1, 1);
	mtrl.Shiness = 1;
	mtrl.TextureEnable = true; // テクスチャを使うか否かのフラグ
	m_Materiale->Create(mtrl);

	m_Rotation.x = 0.0f;
	m_Rotation.y = 0.0f;
	m_Rotation.z = 0.0f;
}

//=======================================
// 更新処理
//=======================================
void Texture2D::Update()
{

}

//=======================================
// 描画処理
//=======================================
void Texture2D::Draw()
{
	float PitchRadians = DirectX::XMConvertToRadians(m_Rotation.x); // X軸回転
	float YawRadians = DirectX::XMConvertToRadians(m_Rotation.y);     // Y軸回転
	float RollRadians = DirectX::XMConvertToRadians(m_Rotation.z);   // Z軸回転

	//定数バッファを更新
	ConstBuffer cb;

	// ワールド変換行列の作成
	// →オブジェクトの位置・大きさ・向きを指定
	cb.matrixWorld = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	cb.matrixWorld *= DirectX::XMMatrixRotationRollPitchYaw(PitchRadians, YawRadians, RollRadians);
	cb.matrixWorld *= DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	cb.matrixWorld = DirectX::XMMatrixTranspose(cb.matrixWorld);

	cb.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// トポロジーをセット（プリミティブタイプ）
	g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_Shader.SetGPU();
	m_VertexBuffer.SetGPU();
	m_IndexBuffer.SetGPU();

	m_Texture.SetGPU();
	m_Materiale->SetGPU();

	// UVの設定を指定
	float u = m_NumU - 1;
	float v = m_NumV - 1;
	float uw = 1 / m_SplitX;
	float vh = 1 / m_SplitY;

	RendererSystem::SetUV(u, v, uw, vh);

	// カメラの設定を指定
	m_Camera->SetCamera(1);

	g_pDeviceContext->DrawIndexed(
		4, // 描画するインデックス数（四角形なんで４）
		0, // 最初のインデックスバッファの位置
		0);
}

//=======================================
// 終了処理
//=======================================
void Texture2D::Uninit()
{

}

// テクスチャを指定
void Texture2D::SetTexture(const char* imgname)
{
	// テクスチャロード
	bool sts = m_Texture.Load(imgname);
	assert(sts == true);
}

// 位置を指定
void Texture2D::SetPosition(const float& x, const float& y, const float& z)
{
	Vector3 p = { x, y, z };
	SetPosition(p);
}
void Texture2D::SetPosition(const Vector3& pos)
{
	this->m_Position = pos;
}

// 角度を指定
void Texture2D::SetRotation(const float& x, const float& y, const float& z)
{
	Vector3 r = { x, y, z };
	SetRotation(r);
}
void Texture2D::SetRotation(const Vector3& rot)
{
	this->m_Rotation = rot * 3.14f/180; // deg→radに変換
}

// 大きさを指定
void Texture2D::SetScale(const float& x, const float& y, const float& z)
{
	Vector3 s = { x, y, z };
	SetScale(s);
}
void Texture2D::SetScale(const Vector3& scl)
{
	this->m_Scale = scl;
}

// UV座標を指定
void Texture2D::SetUV(const float& nu, const float& nv, const float& sx, const float& sy)
{
	m_NumU = nu;
	m_NumV = nv;
	m_SplitX = sx;
	m_SplitY = sy;
}
