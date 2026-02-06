#include "CircleMesh.h"
using namespace DirectX;

CircleMesh::CircleMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
	DefaultSetMaterial();
	DefaultSetTexture();
}

std::vector<VERTEX_3D> CircleMesh::CreateMeshVertices() {
	// 頂点データ

	m_Vertices.resize(numSegments + 1);

	m_Vertices[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Vertices[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[0].uv = XMFLOAT2(0.5f, 0.5f);	// 中心はUVの中心
	m_Vertices[0].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

	for (int i = 1; i < numSegments + 1; ++i) {
		float theta = (DirectX::XM_2PI * i) / numSegments;
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);

		m_Vertices[i].position = XMFLOAT3(x, y, 0.0f);
		m_Vertices[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_Vertices[i].uv = XMFLOAT2(0.5f + cosf(theta) * 0.5f, 0.5f - sinf(theta) * 0.5f);
		m_Vertices[i].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	}

	return m_Vertices;
}

std::vector<unsigned int> CircleMesh::CreateMeshIndices() {
	// インデックス情報生成
	m_Indices.clear(); // 念のためクリア

	for (int i = 1; i < numSegments; ++i) {
		m_Indices.push_back(0);
		m_Indices.push_back(i + 1);
		m_Indices.push_back(i);
	}

	// 最後の三角形で円周を閉じる
	m_Indices.push_back(0);
	m_Indices.push_back(1);
	m_Indices.push_back(numSegments);

	return m_Indices;
}
