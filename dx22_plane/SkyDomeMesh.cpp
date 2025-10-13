#include "SkyDomeMesh.h"
#include <SimpleMath.h>
#include "DirectXRender.h"

using namespace DirectX::SimpleMath;

SkyDomeMesh::SkyDomeMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
}

std::vector<VERTEX_3D> SkyDomeMesh::CreateMeshVertices() {

	// 頂点データ
	int     vcount = (latitudeBands + 1) * (latitudeBands / 2 + 1);	// 頂点の総数を計算
	m_Vertices.resize(vcount);

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
			m_Vertices[inx].position = Vector3(x, y, z);

			// 頂点の法線を設定（位置と同じ）
			m_Vertices[inx].normal = Vector3(0.0f, 0.0f, 0.0f);

			m_Vertices[inx].color = Color(1, 1, 1, 1);

			// 頂点のテクスチャ座標を設定
			m_Vertices[inx].uv = Vector2(u, v);

		}
	}

	return m_Vertices;
}

std::vector<unsigned int> SkyDomeMesh::CreateMeshIndices() {

	// インデックスバッファ生成

	int   icount = latitudeBands * 3 + latitudeBands * (latitudeBands / 2 - 1) * 6 + latitudeBands * 3;
	m_Indices.resize(icount);
	icount = 0;
	int i = 0;
	// 頂点データを定数データ化し、その頂点を元に大量の三角形を作成して球体を表現している
	// 上部の三角形を作成
	for (int j = 0; j < latitudeBands; ++j) {
		m_Indices[icount] = i * (latitudeBands + 1) + j;
		m_Indices[icount + 2] = (i + 1) * (latitudeBands + 1) + j + 1;
		m_Indices[icount + 1] = (i + 1) * (latitudeBands + 1) + j;
		icount += 3;
	}

	// 中間部の三角形を作成
	for (i = 1; i < latitudeBands / 2; ++i) {
		for (int j = 0; j < latitudeBands; ++j) {
			m_Indices[icount] = i * (latitudeBands + 1) + j;
			m_Indices[icount + 2] = i * (latitudeBands + 1) + j + 1;
			m_Indices[icount + 1] = (i + 1) * (latitudeBands + 1) + j;
			icount += 3;
			m_Indices[icount] = i * (latitudeBands + 1) + j + 1;
			m_Indices[icount + 2] = (i + 1) * (latitudeBands + 1) + j + 1;
			m_Indices[icount + 1] = (i + 1) * (latitudeBands + 1) + j;
			icount += 3;
		}
	}

	// 株の三角形を作成
	i = latitudeBands / 2;
	for (int j = 0; j < latitudeBands; ++j) {
		m_Indices[icount] = i * (latitudeBands + 1) + j;
		m_Indices[icount + 2] = (i + 1) * (latitudeBands + 1) + j + 1;
		m_Indices[icount + 1] = (i + 1) * (latitudeBands + 1) + j;
		icount += 3;
	}

	return m_Indices;
}

// 描画処理も書く