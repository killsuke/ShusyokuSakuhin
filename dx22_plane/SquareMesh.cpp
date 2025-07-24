#include "SquareMesh.h"
using namespace DirectX::SimpleMath;

SquareMesh::SquareMesh() {
	// 頂点データ

	m_vertices.resize(6);

	m_vertices[0].position = Vector3(-1.0f, -1.0f, 0.0f);
	m_vertices[1].position = Vector3(1.0f, -1.0f, 0.0f);
	m_vertices[2].position = Vector3(1.0f, 1.0f, 0.0f);
	m_vertices[3].position = Vector3(-1.0f, 1.0f, 0.0f);


	m_vertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);


	m_vertices[0].uv = Vector2(0.0f, 1.0f);	// 右下
	m_vertices[1].uv = Vector2(1.0f, 1.0f);	// 左下
	m_vertices[2].uv = Vector2(1.0f, 0.0f);	// 左上
	m_vertices[3].uv = Vector2(0.0f, 0.0f);	// 右上

	// インデックス情報生成

	m_indices.resize(6);

	m_indices = {
		2,1,0,
		0,3,2,
	};
}

std::vector<VERTEX_3D> SquareMesh::CreateMeshVertices() {
	// 頂点データ

	m_vertices.resize(6);

	m_vertices[0].position = Vector3(-1.0f, -1.0f, 0.0f);
	m_vertices[1].position = Vector3(1.0f, -1.0f, 0.0f);
	m_vertices[2].position = Vector3(1.0f, 1.0f, 0.0f);
	m_vertices[3].position = Vector3(-1.0f, 1.0f, 0.0f);


	m_vertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);


	m_vertices[0].uv = Vector2(0.0f, 1.0f);	// 右下
	m_vertices[1].uv = Vector2(1.0f, 1.0f);	// 左下
	m_vertices[2].uv = Vector2(1.0f, 0.0f);	// 左上
	m_vertices[3].uv = Vector2(0.0f, 0.0f);	// 右上

	return m_vertices;

	//// 頂点バッファ生成
	//m_VertexBuffer.Create(vertices);
}

std::vector<unsigned int> SquareMesh::CreateMeshIndices() {
	// インデックス情報生成

	m_indices.resize(6);

	m_indices = {
		2,1,0,
		0,3,2,
	};

	return m_indices;

	//// インデックスバッファ生成
	//m_IndexBuffer.Create(indices);
}