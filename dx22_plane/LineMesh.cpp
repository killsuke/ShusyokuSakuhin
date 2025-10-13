#include "LineMesh.h"

using namespace DirectX::SimpleMath;

LineMesh::LineMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
}

std::vector<VERTEX_3D> LineMesh::CreateMeshVertices() {
	// 頂点データ

	m_Vertices.resize(2);

	m_Vertices[0].position = Vector3(-1.0f, 1.0f, 1.0f);
	m_Vertices[1].position = Vector3(1.0f, 1.0f, 1.0f);
	/*m_vertices[2].position = Vector3(-1.0f, 1.0f, -1.0f);
	m_vertices[3].position = Vector3(1.0f, 1.0f, -1.0f);*/

	m_Vertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	//m_vertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	//m_vertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_Vertices[0].uv = Vector2(0.0f, 0.0f);
	m_Vertices[1].uv = Vector2(1.0f, 0.0f);
	/*m_vertices[2].uv = Vector2(0.0f, 0.5f);
	m_vertices[3].uv = Vector2(0.33f, 0.5f);*/

	m_Vertices[0].normal = Vector3(0.0f, 1.0f, 0.0f);
	m_Vertices[1].normal = Vector3(0.0f, 1.0f, 0.0f);
	//m_vertices[2].normal = Vector3(0.0f, 1.0f, 0.0f);
	//m_vertices[3].normal = Vector3(0.0f, 1.0f, 0.0f);


	/*m_vertices[4].position = Vector3(-1.0f, 1.0f, -1.0f);
	m_vertices[5].position = Vector3(1.0f, 1.0f, -1.0f);
	m_vertices[6].position = Vector3(-1.0f, -1.0f, -1.0f);
	m_vertices[7].position = Vector3(1.0f, -1.0f, -1.0f);

	m_vertices[4].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[5].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[6].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[7].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_vertices[4].uv = Vector2(0.33f, 0.0f);
	m_vertices[5].uv = Vector2(0.66f, 0.0f);
	m_vertices[6].uv = Vector2(0.33f, 0.5f);
	m_vertices[7].uv = Vector2(0.66f, 0.5f);

	m_vertices[4].normal = Vector3(0.0f, 0.0f, -1.0f);
	m_vertices[5].normal = Vector3(0.0f, 0.0f, -1.0f);
	m_vertices[6].normal = Vector3(0.0f, 0.0f, -1.0f);
	m_vertices[7].normal = Vector3(0.0f, 0.0f, -1.0f);


	m_vertices[8].position = Vector3(1.0f, -1.0f, 1.0f);
	m_vertices[9].position = Vector3(1.0f, 1.0f, 1.0f);
	m_vertices[10].position = Vector3(-1.0f, -1.0f, 1.0f);
	m_vertices[11].position = Vector3(-1.0f, 1.0f, 1.0f);

	m_vertices[8].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[9].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[10].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[11].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_vertices[8].uv = Vector2(0.33f, 0.5f);
	m_vertices[9].uv = Vector2(0.66f, 0.5f);
	m_vertices[10].uv = Vector2(0.33f, 1.0f);
	m_vertices[11].uv = Vector2(0.66f, 1.0f);

	m_vertices[8].normal = Vector3(0.0f, 0.0f, 1.0f);
	m_vertices[9].normal = Vector3(0.0f, 0.0f, 1.0f);
	m_vertices[10].normal = Vector3(0.0f, 0.0f, 1.0f);
	m_vertices[11].normal = Vector3(0.0f, 0.0f, 1.0f);


	m_vertices[12].position = Vector3(-1.0f, -1.0f, 1.0f);
	m_vertices[13].position = Vector3(-1.0f, 1.0f, 1.0f);
	m_vertices[14].position = Vector3(-1.0f, -1.0f, -1.0f);
	m_vertices[15].position = Vector3(-1.0f, 1.0f, -1.0f);

	m_vertices[12].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[13].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[14].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[15].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_vertices[12].uv = Vector2(0.66f, 0.0f);
	m_vertices[13].uv = Vector2(1.0f, 0.0f);
	m_vertices[14].uv = Vector2(0.66f, 0.5f);
	m_vertices[15].uv = Vector2(1.0f, 0.5f);

	m_vertices[12].normal = Vector3(-1.0f, 0.0f, 0.0f);
	m_vertices[13].normal = Vector3(-1.0f, 0.0f, 0.0f);
	m_vertices[14].normal = Vector3(-1.0f, 0.0f, 0.0f);
	m_vertices[15].normal = Vector3(-1.0f, 0.0f, 0.0f);


	m_vertices[16].position = Vector3(1.0f, -1.0f, -1.0f);
	m_vertices[17].position = Vector3(1.0f, 1.0f, -1.0f);
	m_vertices[18].position = Vector3(1.0f, -1.0f, 1.0f);
	m_vertices[19].position = Vector3(1.0f, 1.0f, 1.0f);

	m_vertices[16].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[17].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[18].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[19].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_vertices[16].uv = Vector2(0.0f, 0.5f);
	m_vertices[17].uv = Vector2(0.33f, 0.5f);
	m_vertices[18].uv = Vector2(0.0f, 1.0f);
	m_vertices[19].uv = Vector2(0.33f, 1.0f);

	m_vertices[16].normal = Vector3(1.0f, 0.0f, 0.0f);
	m_vertices[17].normal = Vector3(1.0f, 0.0f, 0.0f);
	m_vertices[18].normal = Vector3(1.0f, 0.0f, 0.0f);
	m_vertices[19].normal = Vector3(1.0f, 0.0f, 0.0f);


	m_vertices[20].position = Vector3(-1.0f, -1.0f, 1.0f);
	m_vertices[21].position = Vector3(1.0f, -1.0f, 1.0f);
	m_vertices[22].position = Vector3(-1.0f, -1.0f, -1.0f);
	m_vertices[23].position = Vector3(1.0f, -1.0f, -1.0f);

	m_vertices[20].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[21].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[22].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[23].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_vertices[20].uv = Vector2(0.66f, 0.5f);
	m_vertices[21].uv = Vector2(1.0f, 0.5f);
	m_vertices[22].uv = Vector2(0.66f, 1.0f);
	m_vertices[23].uv = Vector2(1.0f, 1.0f);

	m_vertices[20].normal = Vector3(0.0f, -1.0f, 0.0f);
	m_vertices[21].normal = Vector3(0.0f, -1.0f, 0.0f);
	m_vertices[22].normal = Vector3(0.0f, -1.0f, 0.0f);
	m_vertices[23].normal = Vector3(0.0f, -1.0f, 0.0f);*/

	return m_Vertices;
}

std::vector<unsigned int> LineMesh::CreateMeshIndices() {
	// インデックスバッファ生成

	m_Indices.resize(2);

	m_Indices = {
		0,1,/*2,
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
		23,21,22,*/
	};

	return m_Indices;
}