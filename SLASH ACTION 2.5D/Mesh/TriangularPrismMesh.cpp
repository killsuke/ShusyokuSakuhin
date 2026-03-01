#include "TriangularPrismMesh.h"

using namespace DirectX;

namespace {
	constexpr size_t VERTEX_NUM = 18;
	constexpr size_t INDEX_NUM = 24;
}

TriangularPrismMesh::TriangularPrismMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
	DefaultSetMaterial();
	DefaultSetTexture();
}

std::vector<VERTEX_3D> TriangularPrismMesh::CreateMeshVertices() {
	// 頂点データ

	m_Vertices.resize(VERTEX_NUM);

	// 上面
	m_Vertices[0].position = XMFLOAT3(0.0f, 1.0f, 1.0f);
	m_Vertices[1].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	m_Vertices[2].position = XMFLOAT3(1.0f, 1.0f, -1.0f);

	// 下面
	m_Vertices[3].position = XMFLOAT3(0.0f, -1.0f, 1.0f);
	m_Vertices[4].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	m_Vertices[5].position = XMFLOAT3(1.0f, -1.0f, -1.0f);

	// 背面
	m_Vertices[6].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	m_Vertices[7].position = XMFLOAT3( 1.0f, 1.0f, -1.0f);
	m_Vertices[8].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	m_Vertices[9].position = XMFLOAT3( 1.0f, -1.0f, -1.0f);

	// 左側面
	m_Vertices[10].position = XMFLOAT3( 0.0f,  1.0f, 1.0f);
	m_Vertices[11].position = XMFLOAT3(-1.0f,  1.0f,-1.0f);
	m_Vertices[12].position = XMFLOAT3( 0.0f, -1.0f, 1.0f);
	m_Vertices[13].position = XMFLOAT3(-1.0f, -1.0f,-1.0f);

	// 右側面
	m_Vertices[14].position = XMFLOAT3(1.0f, 1.0f, -1.0f);
	m_Vertices[15].position = XMFLOAT3(0.0f, 1.0f,  1.0f);
	m_Vertices[16].position = XMFLOAT3(1.0f, -1.0f, -1.0f);
	m_Vertices[17].position = XMFLOAT3(0.0f, -1.0f, 1.0f);


	// 上面
	m_Vertices[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// 下面
	m_Vertices[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[4].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[5].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// 背面
	m_Vertices[6].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[7].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[8].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[9].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// 左側面
	m_Vertices[10].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[11].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[12].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[13].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// 右側面
	m_Vertices[14].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[15].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[16].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[17].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);


	// 上面
	m_Vertices[0].uv = XMFLOAT2(0.5f, 0.0f);
	m_Vertices[1].uv = XMFLOAT2(0.33f, 0.33f);
	m_Vertices[2].uv = XMFLOAT2(0.66f, 0.33f);

	// 下面
	m_Vertices[3].uv = XMFLOAT2(0.5f, 1.0f);
	m_Vertices[4].uv = XMFLOAT2(0.33f, 0.66f);
	m_Vertices[5].uv = XMFLOAT2(0.66f, 0.66f);

	// 背面
	m_Vertices[6].uv = XMFLOAT2(0.33f, 0.33f);
	m_Vertices[7].uv = XMFLOAT2(0.66f, 0.33f);
	m_Vertices[8].uv = XMFLOAT2(0.33f, 0.66f);
	m_Vertices[9].uv = XMFLOAT2(0.66f, 0.66f);

	// 左側面
	m_Vertices[10].uv = XMFLOAT2(0.0f, 0.33f);
	m_Vertices[11].uv = XMFLOAT2(0.33f, 0.33f);
	m_Vertices[12].uv = XMFLOAT2(0.0f, 0.66f);
	m_Vertices[13].uv = XMFLOAT2(0.33f, 0.66f);

	// 右側面
	m_Vertices[14].uv = XMFLOAT2(0.66f, 0.33f);
	m_Vertices[15].uv = XMFLOAT2(1.0f, 0.33f);
	m_Vertices[16].uv = XMFLOAT2(0.66f, 0.66f);
	m_Vertices[17].uv = XMFLOAT2(1.0f, 0.66f);


	// 上面
	m_Vertices[0].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertices[1].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertices[2].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	// 下面
	m_Vertices[3].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_Vertices[4].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_Vertices[5].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	// 背面
	m_Vertices[6].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	m_Vertices[7].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	m_Vertices[8].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	m_Vertices[9].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 左側面
	m_Vertices[10].normal = XMFLOAT3(-0.89f, 0.0f, 0.45f);
	m_Vertices[11].normal = XMFLOAT3(-0.89f, 0.0f, 0.45f);
	m_Vertices[12].normal = XMFLOAT3(-0.89f, 0.0f, 0.45f);
	m_Vertices[13].normal = XMFLOAT3(-0.89f, 0.0f, 0.45f);

	// 右側面
	m_Vertices[14].normal = XMFLOAT3(0.89f, 0.0f, 0.45f);
	m_Vertices[15].normal = XMFLOAT3(0.89f, 0.0f, 0.45f);
	m_Vertices[16].normal = XMFLOAT3(0.89f, 0.0f, 0.45f);
	m_Vertices[17].normal = XMFLOAT3(0.89f, 0.0f, 0.45f);


	return m_Vertices;
}

std::vector<unsigned int> TriangularPrismMesh::CreateMeshIndices() {
	// インデックス情報生成

	m_Indices.resize(INDEX_NUM);

	m_Indices = {
		// 上面
		0,2,1,

		// 下面
		3,4,5,

		// 背面
		6,7,8,7,9,8,

		// 左側面
		10,11,12,11,13,12,

		// 右側面
		14,15,16,15,17,16
	};

	return m_Indices;
}