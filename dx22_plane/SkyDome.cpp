#include "SkyDome.h"
#include "Game.h"
#include <DDSTextureLoader.h>
using namespace DirectX::SimpleMath;

// コンストラクタ
SkyDome::SkyDome(Camera* cam) :Object(cam) {
}

// デストラクタ
SkyDome::~SkyDome() {

}

void SkyDome::Init() {

	int latitudeBands = 16;
	int longitudeBands = 32;
	float radius = 10.0f;

	// 頂点データ
	std::vector<VERTEX_3D>	vertices;
	int     vcount = (latitudeBands + 1) * (latitudeBands / 2 + 1);	// 頂点の総数を計算
	vertices.resize(vcount);

	// 頂点作成
	for (int i = 0; i <= (latitudeBands / 2); ++i) {
		// 緯度方向の角度を計算
		float irad = DirectX::XM_PI * 2.0f / (float)latitudeBands * (float)i;
		float y = (float)cos(irad);					// y座標を計算
		float r = (float)sin(irad);					// 半径を計算
		float v = (float)i / (float)(latitudeBands / 2);	//テクスチャ座標vを計算
		for (int j = 0; j <= latitudeBands; ++j) {
			// 経度方向の角度を計算
			float jrad = DirectX::XM_PI * 2.0f / (float)latitudeBands * (float)j;
			float x = r * (float)cos(jrad);			// x座標を計算
			float z = r * (float)sin(jrad);			// z座標を計算
			float u = (float)j / (float)latitudeBands;	// テクスチャ座標uを計算

			// 頂点インデックスを計算
			int   inx = i * (latitudeBands + 1) + j;

			// 頂点の位置を設定
			vertices[inx].position = Vector3(x,y,z);
			
			// 頂点の法線を設定（位置と同じ）
			vertices[inx].normal = Vector3(0.0f,0.0f,0.0f);
			
			vertices[inx].color = Color(1, 1, 1, 1);

			// 頂点のテクスチャ座標を設定
			vertices[inx].uv = Vector2(u,v);

		}
	}

	// インデックスバッファ生成
	std::vector<unsigned int> indices;

	int   icount = latitudeBands * 3 + latitudeBands * (latitudeBands / 2 - 1) * 6 + latitudeBands * 3;
	indices.resize(icount);
	icount = 0;
	int i = 0;
	// 頂点データを定数データ化し、その頂点を元に大量の三角形を作成して球体を表現している
	// 上部の三角形を作成
	for (int j = 0; j < latitudeBands; ++j) {
		indices[icount] = i * (latitudeBands + 1) + j;
		indices[icount + 2] = (i + 1) * (latitudeBands + 1) + j + 1;
		indices[icount + 1] = (i + 1) * (latitudeBands + 1) + j;
		icount += 3;
	}

	// 中間部の三角形を作成
	for (i = 1; i < latitudeBands / 2; ++i) {
		for (int j = 0; j < latitudeBands; ++j) {
			indices[icount] = i * (latitudeBands + 1) + j;
			indices[icount + 2] = i * (latitudeBands + 1) + j + 1;
			indices[icount + 1] = (i + 1) * (latitudeBands + 1) + j;
			icount += 3;
			indices[icount] = i * (latitudeBands + 1) + j + 1;
			indices[icount + 2] = (i + 1) * (latitudeBands + 1) + j + 1;
			indices[icount + 1] = (i + 1) * (latitudeBands + 1) + j;
			icount += 3;
		}
	}

	// 株の三角形を作成
	i = latitudeBands / 2;
	for (int j = 0; j < latitudeBands; ++j) {
		indices[icount] = i * (latitudeBands + 1) + j;
		indices[icount + 2] = (i + 1) * (latitudeBands + 1) + j + 1;
		indices[icount + 1] = (i + 1) * (latitudeBands + 1) + j;
		icount += 3;
	}

	m_IndexCount = icount;

	// 頂点バッファ生成
	m_VertexBuffer.Create(vertices);

	// インデックスバッファ生成
	//std::vector<unsigned int> indices;
	//indices.resize(36);

	//indices = {
	//	 0, 2, 1, 0, 3, 2,    // 前面
	//4, 5, 6, 6, 7, 4,    // 背面
	//8, 10, 9, 8, 11, 10, // 左側面
	//12, 13, 14, 14, 15, 12, // 右側面
	//18, 17, 16, 16, 19, 18, // 上面
	//20, 21, 22, 22, 23, 20  // 下面
	//};

	// インデックスバッファ生成
	m_IndexBuffer.Create(indices);
	// シェーダオブジェクト生成
	m_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");

	TextureLoadSkyBox();

	m_Scale.x = 500.0f;
	m_Scale.y = 500.0f;
	m_Scale.z = 500.0f;
}

void SkyDome::TextureLoadSkyBox() {
	auto hr = DirectX::CreateDDSTextureFromFile(g_pDevice, L"assets/texture/skyDome.dds", nullptr, &m_pTextureView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "DDSファイルの読み込みに失敗しました。", "エラー", MB_OK | MB_ICONERROR);
	}
}

void SkyDome::Update() {

}

void SkyDome::Draw() {

	// ここで回転が出来る
	/*std::vector<CarModel*>cars = Game::GetInstance()->GetObjects<CarModel>();

	auto rotation = cars[0]->GetRotaiton();*/

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
	//m_Texture.SetGPU();

	m_Camera->SetCamera(2);

	g_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureView);

	// 行列をシェーダーに渡す
	g_pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	g_pDeviceContext->DrawIndexed(
		m_IndexCount,							// 描画するインデックス数（四角形なんで４）
		0,							// 最初のインデックスバッファの位置
		0);
}

void SkyDome::Uninit() {

}