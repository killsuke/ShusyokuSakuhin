#include "CubeMesh.h"
using namespace DirectX;

namespace {
	constexpr size_t VERTEX_NUM = 24;
	constexpr size_t INDEX_NUM = 36;
}

CubeMesh::CubeMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
	DefaultSetMaterial();
	DefaultSetTexture();
}

std::vector<VERTEX_3D> CubeMesh::CreateMeshVertices() {
	// 頂点データ
	m_Vertices.resize(VERTEX_NUM);

	m_Vertices[0].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	m_Vertices[1].position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Vertices[2].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	m_Vertices[3].position = XMFLOAT3(1.0f, 1.0f, -1.0f);

	m_Vertices[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[0].uv = XMFLOAT2(0.0f, 0.0f);
	m_Vertices[1].uv = XMFLOAT2(0.33f, 0.0f);
	m_Vertices[2].uv = XMFLOAT2(0.0f, 0.5f);
	m_Vertices[3].uv = XMFLOAT2(0.33f, 0.5f);

	m_Vertices[0].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertices[1].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertices[2].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertices[3].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);


	m_Vertices[4].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	m_Vertices[5].position = XMFLOAT3(1.0f, 1.0f, -1.0f);
	m_Vertices[6].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	m_Vertices[7].position = XMFLOAT3(1.0f, -1.0f, -1.0f);

	m_Vertices[4].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[5].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[6].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[7].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[4].uv = XMFLOAT2(0.33f, 0.0f);
	m_Vertices[5].uv = XMFLOAT2(0.66f, 0.0f);
	m_Vertices[6].uv = XMFLOAT2(0.33f, 0.5f);
	m_Vertices[7].uv = XMFLOAT2(0.66f, 0.5f);

	m_Vertices[4].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	m_Vertices[5].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	m_Vertices[6].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	m_Vertices[7].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);


	m_Vertices[8].position = XMFLOAT3(1.0f, -1.0f, 1.0f);
	m_Vertices[9].position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Vertices[10].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	m_Vertices[11].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);

	m_Vertices[8].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[9].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[10].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[11].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[8].uv = XMFLOAT2(0.33f, 0.5f);
	m_Vertices[9].uv = XMFLOAT2(0.66f, 0.5f);
	m_Vertices[10].uv = XMFLOAT2(0.33f, 1.0f);
	m_Vertices[11].uv = XMFLOAT2(0.66f, 1.0f);

	m_Vertices[8].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_Vertices[9].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_Vertices[10].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_Vertices[11].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);


	m_Vertices[12].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	m_Vertices[13].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	m_Vertices[14].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	m_Vertices[15].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);

	m_Vertices[12].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[13].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[14].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[15].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[12].uv = XMFLOAT2(0.66f, 0.0f);
	m_Vertices[13].uv = XMFLOAT2(1.0f, 0.0f);
	m_Vertices[14].uv = XMFLOAT2(0.66f, 0.5f);
	m_Vertices[15].uv = XMFLOAT2(1.0f, 0.5f);

	m_Vertices[12].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	m_Vertices[13].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	m_Vertices[14].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	m_Vertices[15].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);


	m_Vertices[16].position = XMFLOAT3(1.0f, -1.0f, -1.0f);
	m_Vertices[17].position = XMFLOAT3(1.0f, 1.0f, -1.0f);
	m_Vertices[18].position = XMFLOAT3(1.0f, -1.0f, 1.0f);
	m_Vertices[19].position = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_Vertices[16].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[17].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[18].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[19].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[16].uv = XMFLOAT2(0.0f, 0.5f);
	m_Vertices[17].uv = XMFLOAT2(0.33f, 0.5f);
	m_Vertices[18].uv = XMFLOAT2(0.0f, 1.0f);
	m_Vertices[19].uv = XMFLOAT2(0.33f, 1.0f);

	m_Vertices[16].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_Vertices[17].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_Vertices[18].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_Vertices[19].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);


	m_Vertices[20].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	m_Vertices[21].position = XMFLOAT3( 1.0f, -1.0f, 1.0f);
	m_Vertices[22].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	m_Vertices[23].position = XMFLOAT3(1.0f, -1.0f, -1.0f);

	m_Vertices[20].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[21].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[22].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[23].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[20].uv = XMFLOAT2(0.66f, 0.5f);
	m_Vertices[21].uv = XMFLOAT2(1.0f, 0.5f);
	m_Vertices[22].uv = XMFLOAT2(0.66f, 1.0f);
	m_Vertices[23].uv = XMFLOAT2(1.0f, 1.0f);

	m_Vertices[20].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_Vertices[21].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_Vertices[22].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_Vertices[23].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	return m_Vertices;
}

std::vector<unsigned int> CubeMesh::CreateMeshIndices() {
	// インデックスバッファ生成

	m_Indices.resize(INDEX_NUM);

	m_Indices = {
		0,1,2,
		1,3,2,

		4,5,6,
		5,7,6,

		8,9,10,
		9,11,10,

		12,13,14,
		13,15,14,

		16,17,18,
		17,19,18,

		21,20,22,
		23,21,22,
	};

	return m_Indices;
}