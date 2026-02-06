#include "SquareMesh.h"

using namespace DirectX;

namespace {
	constexpr size_t VERTEX_NUM = 4;
	constexpr size_t INDEX_NUM = 6;
}

SquareMesh::SquareMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
	DefaultSetMaterial();
	DefaultSetTexture();
}

std::vector<VERTEX_3D> SquareMesh::CreateMeshVertices() {
	// 頂点データ

	m_Vertices.resize(VERTEX_NUM);

	m_Vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	m_Vertices[1].position = XMFLOAT3( 1.0f, -1.0f, 0.0f);
	m_Vertices[2].position = XMFLOAT3( 1.0f,  1.0f, 0.0f);
	m_Vertices[3].position = XMFLOAT3(-1.0f,  1.0f, 0.0f);


	m_Vertices[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);


	m_Vertices[0].uv = XMFLOAT2(0.0f, 1.0f);	// 右下
	m_Vertices[1].uv = XMFLOAT2(1.0f, 1.0f);	// 左下
	m_Vertices[2].uv = XMFLOAT2(1.0f, 0.0f);	// 左上
	m_Vertices[3].uv = XMFLOAT2(0.0f, 0.0f);	// 右上


	m_Vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	m_Vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	m_Vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	m_Vertices[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	return m_Vertices;
}

std::vector<unsigned int> SquareMesh::CreateMeshIndices() {
	// インデックス情報生成

	m_Indices.resize(INDEX_NUM);

	m_Indices = {
		2,1,0,
		0,3,2,
	};

	return m_Indices;
}