#include "LineMesh.h"

using namespace DirectX;

namespace {
	constexpr size_t VERTEX_NUM = 2; // 線の頂点数
	constexpr size_t INDEX_NUM = 2;  // 線のインデックス数
}

LineMesh::LineMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
	DefaultSetMaterial();
	DefaultSetTexture();
}

std::vector<VERTEX_3D> LineMesh::CreateMeshVertices() {
	// 頂点データ

	m_Vertices.resize(VERTEX_NUM);

	m_Vertices[0].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	m_Vertices[1].position = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_Vertices[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[0].uv = XMFLOAT2(0.0f, 0.0f);
	m_Vertices[1].uv = XMFLOAT2(1.0f, 0.0f);

	m_Vertices[0].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertices[1].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	return m_Vertices;
}

std::vector<unsigned int> LineMesh::CreateMeshIndices() {
	// インデックスバッファ生成

	m_Indices.resize(INDEX_NUM);

	m_Indices = {
		0,1
	};

	return m_Indices;
}