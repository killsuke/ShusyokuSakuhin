#include "CubeMesh.h"
using namespace DirectX::SimpleMath;

CubeMesh::CubeMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
	DefaultSetMaterial();
	DefaultSetTexture();
}

std::vector<VERTEX_3D> CubeMesh::CreateMeshVertices() {
	// 頂点データ
	m_Vertices.resize(24);

	m_Vertices[0].position = Vector3(-1.0f, 1.0f, 1.0f);
	m_Vertices[1].position = Vector3(1.0f, 1.0f, 1.0f);
	m_Vertices[2].position = Vector3(-1.0f, 1.0f, -1.0f);
	m_Vertices[3].position = Vector3(1.0f, 1.0f, -1.0f);

	m_Vertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[0].uv = Vector2(0.0f, 0.0f);
	m_Vertices[1].uv = Vector2(0.33f, 0.0f);
	m_Vertices[2].uv = Vector2(0.0f, 0.5f);
	m_Vertices[3].uv = Vector2(0.33f, 0.5f);

	m_Vertices[0].normal = Vector3(0.0f, 1.0f, 0.0f);
	m_Vertices[1].normal = Vector3(0.0f, 1.0f, 0.0f);
	m_Vertices[2].normal = Vector3(0.0f, 1.0f, 0.0f);
	m_Vertices[3].normal = Vector3(0.0f, 1.0f, 0.0f);


	m_Vertices[4].position = Vector3(-1.0f, 1.0f, -1.0f);
	m_Vertices[5].position = Vector3(1.0f, 1.0f, -1.0f);
	m_Vertices[6].position = Vector3(-1.0f, -1.0f, -1.0f);
	m_Vertices[7].position = Vector3(1.0f, -1.0f, -1.0f);

	m_Vertices[4].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[5].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[6].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[7].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[4].uv = Vector2(0.33f, 0.0f);
	m_Vertices[5].uv = Vector2(0.66f, 0.0f);
	m_Vertices[6].uv = Vector2(0.33f, 0.5f);
	m_Vertices[7].uv = Vector2(0.66f, 0.5f);

	m_Vertices[4].normal = Vector3(0.0f, 0.0f, -1.0f);
	m_Vertices[5].normal = Vector3(0.0f, 0.0f, -1.0f);
	m_Vertices[6].normal = Vector3(0.0f, 0.0f, -1.0f);
	m_Vertices[7].normal = Vector3(0.0f, 0.0f, -1.0f);


	m_Vertices[8].position = Vector3(1.0f, -1.0f, 1.0f);
	m_Vertices[9].position = Vector3(1.0f, 1.0f, 1.0f);
	m_Vertices[10].position = Vector3(-1.0f, -1.0f, 1.0f);
	m_Vertices[11].position = Vector3(-1.0f, 1.0f, 1.0f);

	m_Vertices[8].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[9].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[10].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[11].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[8].uv = Vector2(0.33f, 0.5f);
	m_Vertices[9].uv = Vector2(0.66f, 0.5f);
	m_Vertices[10].uv = Vector2(0.33f, 1.0f);
	m_Vertices[11].uv = Vector2(0.66f, 1.0f);

	m_Vertices[8].normal = Vector3(0.0f, 0.0f, 1.0f);
	m_Vertices[9].normal = Vector3(0.0f, 0.0f, 1.0f);
	m_Vertices[10].normal = Vector3(0.0f, 0.0f, 1.0f);
	m_Vertices[11].normal = Vector3(0.0f, 0.0f, 1.0f);


	m_Vertices[12].position = Vector3(-1.0f, -1.0f, 1.0f);
	m_Vertices[13].position = Vector3(-1.0f, 1.0f, 1.0f);
	m_Vertices[14].position = Vector3(-1.0f, -1.0f, -1.0f);
	m_Vertices[15].position = Vector3(-1.0f, 1.0f, -1.0f);

	m_Vertices[12].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[13].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[14].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[15].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[12].uv = Vector2(0.66f, 0.0f);
	m_Vertices[13].uv = Vector2(1.0f, 0.0f);
	m_Vertices[14].uv = Vector2(0.66f, 0.5f);
	m_Vertices[15].uv = Vector2(1.0f, 0.5f);

	m_Vertices[12].normal = Vector3(-1.0f, 0.0f, 0.0f);
	m_Vertices[13].normal = Vector3(-1.0f, 0.0f, 0.0f);
	m_Vertices[14].normal = Vector3(-1.0f, 0.0f, 0.0f);
	m_Vertices[15].normal = Vector3(-1.0f, 0.0f, 0.0f);


	m_Vertices[16].position = Vector3(1.0f, -1.0f, -1.0f);
	m_Vertices[17].position = Vector3(1.0f, 1.0f, -1.0f);
	m_Vertices[18].position = Vector3(1.0f, -1.0f, 1.0f);
	m_Vertices[19].position = Vector3(1.0f, 1.0f, 1.0f);

	m_Vertices[16].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[17].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[18].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[19].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[16].uv = Vector2(0.0f, 0.5f);
	m_Vertices[17].uv = Vector2(0.33f, 0.5f);
	m_Vertices[18].uv = Vector2(0.0f, 1.0f);
	m_Vertices[19].uv = Vector2(0.33f, 1.0f);

	m_Vertices[16].normal = Vector3(1.0f, 0.0f, 0.0f);
	m_Vertices[17].normal = Vector3(1.0f, 0.0f, 0.0f);
	m_Vertices[18].normal = Vector3(1.0f, 0.0f, 0.0f);
	m_Vertices[19].normal = Vector3(1.0f, 0.0f, 0.0f);


	m_Vertices[20].position = Vector3(-1.0f, -1.0f, 1.0f);
	m_Vertices[21].position = Vector3( 1.0f, -1.0f, 1.0f);
	m_Vertices[22].position = Vector3(-1.0f, -1.0f, -1.0f);
	m_Vertices[23].position = Vector3(1.0f, -1.0f, -1.0f);

	m_Vertices[20].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[21].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[22].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[23].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[20].uv = Vector2(0.66f, 0.5f);
	m_Vertices[21].uv = Vector2(1.0f, 0.5f);
	m_Vertices[22].uv = Vector2(0.66f, 1.0f);
	m_Vertices[23].uv = Vector2(1.0f, 1.0f);

	m_Vertices[20].normal = Vector3(0.0f, -1.0f, 0.0f);
	m_Vertices[21].normal = Vector3(0.0f, -1.0f, 0.0f);
	m_Vertices[22].normal = Vector3(0.0f, -1.0f, 0.0f);
	m_Vertices[23].normal = Vector3(0.0f, -1.0f, 0.0f);

	return m_Vertices;
}

std::vector<unsigned int> CubeMesh::CreateMeshIndices() {
	// インデックスバッファ生成

	m_Indices.resize(36);

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