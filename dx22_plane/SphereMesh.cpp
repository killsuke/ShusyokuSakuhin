#include "SphereMesh.h"

using namespace DirectX::SimpleMath;

SphereMesh::SphereMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
}

std::vector<VERTEX_3D> SphereMesh::CreateMeshVertices() {
	// 頂点データ

	m_vertices.resize(numSegments + 1);

	m_vertices[0].position = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[0].uv = Vector2(0.0f, 0.0f);
	m_vertices[0].normal = Vector3(0.0f, 0.0f, -1.0f);

	for (int i = 1; i < numSegments + 1; ++i) {
		float theta = (DirectX::XM_2PI * i) / numSegments;
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);

		m_vertices[i].position = Vector3(x, y, 0.0f);
		m_vertices[i].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		m_vertices[i].uv = Vector2(0.0f, 1.0f);
		m_vertices[i].normal = Vector3(0.0f, 0.0f, -1.0f);
	}

	return m_vertices;

	//// 頂点バッファ生成
	//m_VertexBuffer.Create(vertices);
}

std::vector<unsigned int> SphereMesh::CreateMeshIndices() {
	// インデックス情報生成
	m_indices.clear(); // 念のためクリア

	for (unsigned int i = 1; i < numSegments; ++i) {
		m_indices.push_back(0);
		m_indices.push_back(i + 1);
		m_indices.push_back(i);
	}

	// 最後の三角形で円周を閉じる
	m_indices.push_back(0);
	m_indices.push_back(1);
	m_indices.push_back(numSegments);

	return m_indices;
}
