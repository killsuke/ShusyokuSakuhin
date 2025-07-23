#include "TestBoard.h"
#include "input.h"
#include "Collider.h"

using namespace DirectX::SimpleMath;

// コンストラクタ
TestBoard::TestBoard(Camera* cam) :GameObject(cam) {
}

// デストラクタ
TestBoard::~TestBoard() {

}

//=======================================
//初期化処理
//=======================================
void TestBoard::Init()
{
	// 頂点データ
	std::vector<VERTEX_3D>	vertices;

	vertices.resize(6);

	vertices[0].position = Vector3(-1.0f, -1.0f, 0.0f);
	vertices[1].position = Vector3( 1.0f, -1.0f, 0.0f);
	vertices[2].position = Vector3( 1.0f,  1.0f, 0.0f);
	vertices[3].position = Vector3(-1.0f,  1.0f, 0.0f);


	vertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	

	vertices[0].uv = Vector2(0.0f, 1.0f);	// 右下
	vertices[1].uv = Vector2(1.0f, 1.0f);	// 左下
	vertices[2].uv = Vector2(1.0f, 0.0f);	// 左上
	vertices[3].uv = Vector2(0.0f, 0.0f);	// 右上


	// 頂点バッファ生成
	m_VertexBuffer.Create(vertices);

	// インデックスバッファ生成
	std::vector<unsigned int> indices;
	indices.resize(6);

	indices = {
		2,1,0,
		0,3,2,
	};

	// 切断用にインデックスデータを記録
	for(int i = 0; i < vertices.size(); ++i) {
		SlashVertex v = { vertices[i].position,vertices[i].uv};
		triangles.push_back(v);

	}

	// インデックスバッファ生成
	m_IndexBuffer.Create(indices);

	// シェーダオブジェクト生成
	m_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");

	// テクスチャロード
	bool sts = m_Texture.Load("assets/texture/title_car.png");	// コード内で "assets/texture/"を省いて使えるようにしています。
	assert(sts == true);

	//モデルによってスケールを調整
	m_Scale.x = 40.0f;
	m_Scale.y = 20.0f;
	m_Scale.z = 1.0f;

	m_Position.x = 0.0f;
	m_Position.y = 50.0f;
	m_Position.z = 0.0f;

	AddComponent<ColliderComponent>(/*this, m_Position, m_Scale, ColliderType::AABB*/);
}


//=======================================
//更新処理
//=======================================
void TestBoard::Update()
{
	Vector3 newScale = m_Scale;
	newScale.z = 20.0f;
	auto coll = GetComponent<ColliderComponent>();

	coll->SetColliderSize_AABB(m_Position, newScale);
	coll->SetColliderSize_Sphere(m_Position, 20.0f);

	if(Input::GetKeyTrigger(VK_SPACE)) {
		std::vector<SlashVertex> cutTriangles;

		// Y軸方向にカットするラインを作成
		Line2D cutLine = MakeLineFromPoints({ 0, -1 }, { 0, 1 }); // Y軸方向にカット

		for (size_t i = 0; i < triangles.size(); i += 3) {
			CutTriangleWithLine(triangles[i], triangles[i + 1], triangles[i + 2], cutLine, cutTriangles);
		}
	}
}

//=======================================
//描画処理
//=======================================
void TestBoard::Draw()
{
	// ラジアンに変換
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

	// 描画の処理
	// トポロジーをセット（プリミティブタイプ）
	g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
	m_Shader.SetGPU();
	m_VertexBuffer.SetGPU();
	m_IndexBuffer.SetGPU();
	m_Texture.SetGPU();

	m_Camera->SetCamera(0);

	// 行列をシェーダーに渡す
	g_pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	g_pDeviceContext->DrawIndexed(
		6,							// 描画するインデックス数（四角形なんで４）
		0,							// 最初のインデックスバッファの位置
		0);
}

//=======================================
//終了処理
//=======================================
void TestBoard::Uninit()
{
}