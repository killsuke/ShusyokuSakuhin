#include "TriangularPrismMesh.h"

using namespace DirectX::SimpleMath;

namespace {
	unsigned int vertexSize = 18;
	unsigned int indexSize = 24;
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

	m_Vertices.resize(vertexSize);

	// 上面
	m_Vertices[0].position = Vector3(0.0f, 1.0f, 1.0f);
	m_Vertices[1].position = Vector3(-1.0f, 1.0f, -1.0f);
	m_Vertices[2].position = Vector3(1.0f, 1.0f, -1.0f);

	// 下面
	m_Vertices[3].position = Vector3(0.0f, -1.0f, 1.0f);
	m_Vertices[4].position = Vector3(-1.0f, -1.0f, -1.0f);
	m_Vertices[5].position = Vector3(1.0f, -1.0f, -1.0f);

	// 背面
	m_Vertices[6].position = Vector3(-1.0f, 1.0f, -1.0f);
	m_Vertices[7].position = Vector3( 1.0f, 1.0f, -1.0f);
	m_Vertices[8].position = Vector3(-1.0f, -1.0f, -1.0f);
	m_Vertices[9].position = Vector3( 1.0f, -1.0f, -1.0f);

	// 左側面
	m_Vertices[10].position = Vector3( 0.0f,  1.0f, 1.0f);
	m_Vertices[11].position = Vector3(-1.0f,  1.0f,-1.0f);
	m_Vertices[12].position = Vector3( 0.0f, -1.0f, 1.0f);
	m_Vertices[13].position = Vector3(-1.0f, -1.0f,-1.0f);

	// 右側面
	m_Vertices[14].position = Vector3(1.0f, 1.0f, -1.0f);
	m_Vertices[15].position = Vector3(0.0f, 1.0f,  1.0f);
	m_Vertices[16].position = Vector3(1.0f, -1.0f, -1.0f);
	m_Vertices[17].position = Vector3(0.0f, -1.0f, 1.0f);


	// 上面
	m_Vertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	// 下面
	m_Vertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[4].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[5].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	// 背面
	m_Vertices[6].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[7].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[8].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[9].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	// 左側面
	m_Vertices[10].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[11].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[12].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[13].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	// 右側面
	m_Vertices[14].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[15].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[16].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[17].color = Color(1.0f, 1.0f, 1.0f, 1.0f);


	// 上面
	m_Vertices[0].uv = Vector2(0.5f, 0.0f);
	m_Vertices[1].uv = Vector2(0.33f, 0.33f);
	m_Vertices[2].uv = Vector2(0.66f, 0.33f);

	// 下面
	m_Vertices[3].uv = Vector2(0.5f, 1.0f);
	m_Vertices[4].uv = Vector2(0.33f, 0.66f);
	m_Vertices[5].uv = Vector2(0.66f, 0.66f);

	// 背面
	m_Vertices[6].uv = Vector2(0.33f, 0.33f);
	m_Vertices[7].uv = Vector2(0.66f, 0.33f);
	m_Vertices[8].uv = Vector2(0.33f, 0.66f);
	m_Vertices[9].uv = Vector2(0.66f, 0.66f);

	// 左側面
	m_Vertices[10].uv = Vector2(0.0f, 0.33f);
	m_Vertices[11].uv = Vector2(0.33f, 0.33f);
	m_Vertices[12].uv = Vector2(0.0f, 0.66f);
	m_Vertices[13].uv = Vector2(0.33f, 0.66f);

	// 右側面
	m_Vertices[14].uv = Vector2(0.66f, 0.33f);
	m_Vertices[15].uv = Vector2(1.0f, 0.33f);
	m_Vertices[16].uv = Vector2(0.66f, 0.66f);
	m_Vertices[17].uv = Vector2(1.0f, 0.66f);


	// 上面
	m_Vertices[0].normal = Vector3(0.0f, 1.0f, 0.0f);
	m_Vertices[1].normal = Vector3(0.0f, 1.0f, 0.0f);
	m_Vertices[2].normal = Vector3(0.0f, 1.0f, 0.0f);

	// 下面
	m_Vertices[3].normal = Vector3(0.0f, -1.0f, 0.0f);
	m_Vertices[4].normal = Vector3(0.0f, -1.0f, 0.0f);
	m_Vertices[5].normal = Vector3(0.0f, -1.0f, 0.0f);

	// 背面
	m_Vertices[6].normal = Vector3(0.0f, 0.0f, -1.0f);
	m_Vertices[7].normal = Vector3(0.0f, 0.0f, -1.0f);
	m_Vertices[8].normal = Vector3(0.0f, 0.0f, -1.0f);
	m_Vertices[9].normal = Vector3(0.0f, 0.0f, -1.0f);

	// 左側面
	m_Vertices[10].normal = Vector3(-0.89f, 0.0f, 0.45f);
	m_Vertices[11].normal = Vector3(-0.89f, 0.0f, 0.45f);
	m_Vertices[12].normal = Vector3(-0.89f, 0.0f, 0.45f);
	m_Vertices[13].normal = Vector3(-0.89f, 0.0f, 0.45f);

	// 右側面
	m_Vertices[14].normal = Vector3(0.89f, 0.0f, 0.45f);
	m_Vertices[15].normal = Vector3(0.89f, 0.0f, 0.45f);
	m_Vertices[16].normal = Vector3(0.89f, 0.0f, 0.45f);
	m_Vertices[17].normal = Vector3(0.89f, 0.0f, 0.45f);


	return m_Vertices;
}

std::vector<unsigned int> TriangularPrismMesh::CreateMeshIndices() {
	// インデックス情報生成

	m_Indices.resize(indexSize);

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