#include "SkyBox.h"
#include "Game.h"
#include <DDSTextureLoader.h>
using namespace DirectX::SimpleMath;

// コンストラクタ
SkyBox::SkyBox(Camera* cam) :Object(cam) {
}

// デストラクタ
SkyBox::~SkyBox() {

}

void SkyBox::Init() {
	// 頂点データ
	std::vector<VERTEX_3D>	vertices;

	vertices.resize(24);

	vertices[0].position = Vector3(-1.0f, 1.0f, -1.0f);
	vertices[1].position = Vector3(1.0f, 1.0f, -1.0f);
	vertices[2].position = Vector3(1.0f, -1.0f, -1.0f);
	vertices[3].position = Vector3(-1.0f, -1.0f, -1.0f);

	vertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[0].uv = Vector2(0.0f, 0.0f);
	vertices[1].uv = Vector2(0.33f, 0.0f);
	vertices[2].uv = Vector2(0.33f, 0.5f);
	vertices[3].uv = Vector2(0.0f, 0.5f);

	vertices[4].position = Vector3(-1.0f, 1.0f, 1.0f);
	vertices[5].position = Vector3(1.0f, 1.0f, 1.0f);
	vertices[6].position = Vector3(1.0f, -1.0f, 1.0f);
	vertices[7].position = Vector3(-1.0f, -1.0f, 1.0f);

	vertices[4].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[5].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[6].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[7].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[4].uv = Vector2(0.33f, 0.0f);
	vertices[5].uv = Vector2(0.66f, 0.0f);
	vertices[6].uv = Vector2(0.66f, 0.5f);
	vertices[7].uv = Vector2(0.33f, 0.5f);

	vertices[8].position = Vector3(-1.0f, 1.0f, 1.0f);
	vertices[9].position = Vector3(-1.0f, 1.0f, -1.0f);
	vertices[10].position = Vector3(-1.0f, -1.0f, -1.0f);
	vertices[11].position = Vector3(-1.0f, -1.0f, 1.0f);

	vertices[8].color  = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[9].color  = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[10].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[11].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[8].uv  = Vector2(0.33f, 0.5f);
	vertices[9].uv  = Vector2(0.66f, 0.5f);
	vertices[10].uv = Vector2(0.66f, 1.0f);
	vertices[11].uv = Vector2(0.33f, 1.0f);

	vertices[12].position = Vector3(1.0f, 1.0f, 1.0f);
	vertices[13].position = Vector3(1.0f, 1.0f, -1.0f);
	vertices[14].position = Vector3(1.0f, -1.0f, -1.0f);
	vertices[15].position = Vector3(1.0f, -1.0f, 1.0f);

	vertices[12].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[13].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[14].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[15].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[12].uv = Vector2(0.66f, 0.0f);
	vertices[13].uv = Vector2(1.0f, 0.0f);
	vertices[14].uv = Vector2(1.0f, 0.5f);
	vertices[15].uv = Vector2(0.66f, 0.5f);

	vertices[16].position = Vector3(-1.0f, 1.0f, 1.0f);
	vertices[17].position = Vector3(1.0f, 1.0f, 1.0f);
	vertices[18].position = Vector3(1.0f, 1.0f, -1.0f);
	vertices[19].position = Vector3(-1.0f, 1.0f, -1.0f);

	vertices[16].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[17].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[18].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[19].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[16].uv = Vector2(0.0f, 0.5f);
	vertices[17].uv = Vector2(0.33f, 0.5f);
	vertices[18].uv = Vector2(0.33f, 1.0f);
	vertices[19].uv = Vector2(0.0f, 1.0f);

	vertices[20].position = Vector3(-1.0f, -1.0f, 1.0f);
	vertices[21].position = Vector3(1.0f, -1.0f, 1.0f);
	vertices[22].position = Vector3(1.0f, -1.0f, -1.0f);
	vertices[23].position = Vector3(-1.0f, -1.0f, -1.0f);

	vertices[20].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[21].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[22].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[23].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[20].uv = Vector2(0.66f, 0.5f);
	vertices[21].uv = Vector2(1.0f, 0.5f);
	vertices[22].uv = Vector2(1.0f, 1.0f);
	vertices[23].uv = Vector2(0.66f, 1.0f);

	// 頂点バッファ生成
	m_VertexBuffer.Create(vertices);

	// インデックスバッファ生成
	std::vector<unsigned int> indices;
	indices.resize(36);

	indices = {
		 0, 2, 1, 0, 3, 2,    // 前面
	4, 5, 6, 6, 7, 4,    // 背面
	8, 10, 9, 8, 11, 10, // 左側面
	12, 13, 14, 14, 15, 12, // 右側面
	18, 17, 16, 16, 19, 18, // 上面
	20, 21, 22, 22, 23, 20  // 下面
	};

	// インデックスバッファ生成
	m_IndexBuffer.Create(indices);
	// シェーダオブジェクト生成
	m_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");

	TextureLoadSkyBox();

	m_Scale.x = 300.0f;
	m_Scale.y = 300.0f;
	m_Scale.z = 300.0f;
}

void SkyBox::TextureLoadSkyBox() {
	DirectX::CreateDDSTextureFromFile(RendererSystem::GetDevice(), L"pretoria_gardens_4k.dds", nullptr, &m_pTextureView);
}

void SkyBox::Update() {

}

void SkyBox::Draw() {

	// ここで回転にあわせられる
	//std::vector<CarModel*>cars = Game::GetInstance()->GetObjects<CarModel>();

	//auto rotation =  cars[0]->GetRotaiton();
	
	// ラジアンに変換
	float PitchRadians = DirectX::XMConvertToRadians(m_Rotation.x); // X軸回転
	float YawRadians = DirectX::XMConvertToRadians(m_Rotation.y);     // Y軸回転
	float RollRadians = DirectX::XMConvertToRadians(m_Rotation.z);   // Z軸回転

	//定数バッファを更新
	ConstBuffer cb;
	
	// ワールド変換行列の作成
	// →オブジェクトの位置・大きさ・向きを指定
	cb.matrixWorld = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	cb.matrixWorld *= DirectX::XMMatrixRotationRollPitchYaw(PitchRadians, -YawRadians, RollRadians);
	cb.matrixWorld *= DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	cb.matrixWorld = DirectX::XMMatrixTranspose(cb.matrixWorld);

	cb.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// 描画の処理
	// トポロジーをセット（プリミティブタイプ）
	g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
	m_Shader.SetGPU();
	m_VertexBuffer.SetGPU();
	m_IndexBuffer.SetGPU();
	m_Texture.SetGPU();

	m_Camera->SetCamera(2);

	g_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureView);

	// 行列をシェーダーに渡す
	g_pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	g_pDeviceContext->DrawIndexed(
		36,							// 描画するインデックス数（四角形なんで４）
		0,							// 最初のインデックスバッファの位置
		0);
}

void SkyBox::Uninit() {

}