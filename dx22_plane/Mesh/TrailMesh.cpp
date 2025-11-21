#include "TrailMesh.h"

using namespace DirectX::SimpleMath;

TrailMesh::TrailMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
	DefaultSetMaterial();
	DefaultSetTexture();
}

std::vector<VERTEX_3D> TrailMesh::CreateMeshVertices() {
	// 頂点データ

	m_Vertices.resize(4 * MaxTrailBufferSize::MAX_TRAIL_SEGMENTS);

	for (int i = 0; i < m_Vertices.size(); ++i) {
		m_Vertices[i].position = Vector3::Zero;
		m_Vertices[i].color = Vector4::Zero;
		m_Vertices[i].uv = Vector2::Zero;
		m_Vertices[i].normal = Vector3::Zero;
	}

	//m_Vertices[0].position = Vector3(-1.0f, -1.0f, 0.0f);
	//m_Vertices[1].position = Vector3(1.0f, -1.0f, 0.0f);
	//m_Vertices[2].position = Vector3(1.0f, 1.0f, 0.0f);
	//m_Vertices[3].position = Vector3(-1.0f, 1.0f, 0.0f);


	//m_Vertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Vertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Vertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Vertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);


	//m_Vertices[0].uv = Vector2(0.0f, 1.0f);	// 右下
	//m_Vertices[1].uv = Vector2(1.0f, 1.0f);	// 左下
	//m_Vertices[2].uv = Vector2(1.0f, 0.0f);	// 左上
	//m_Vertices[3].uv = Vector2(0.0f, 0.0f);	// 右上


	//m_Vertices[0].normal = Vector3(0.0f, 0.0f, -1.0f);
	//m_Vertices[1].normal = Vector3(0.0f, 0.0f, -1.0f);
	//m_Vertices[2].normal = Vector3(0.0f, 0.0f, -1.0f);
	//m_Vertices[3].normal = Vector3(0.0f, 0.0f, -1.0f);

	return m_Vertices;
}

std::vector<unsigned int> TrailMesh::CreateMeshIndices() {
	// インデックス情報生成

	m_Indices.resize(6 * MaxTrailBufferSize::MAX_TRAIL_SEGMENTS);

	for (int i = 0; i < m_Indices.size(); ++i) {
		m_Indices[i] = 0;
	}

	return m_Indices;
}

std::vector<VERTEX_3D> TrailMesh::CreateMeshVertices(const unsigned int size)
{
	m_Vertices.clear();
	m_Vertices.resize(size);

	for (int i = 0; i < m_Vertices.size(); ++i) {
		m_Vertices[i].position = Vector3::Zero;
		m_Vertices[i].color = Vector4::Zero;
		m_Vertices[i].uv = Vector2::Zero;
		m_Vertices[i].normal = Vector3::Zero;
	}

	return m_Vertices;
}

std::vector<unsigned int> TrailMesh::CreateMeshIndices(const unsigned int size) {

	m_Indices.clear();
	m_Indices.resize(size);

	for (int i = 0; i < m_Indices.size(); ++i) {
		m_Indices[i] = 0;
	}

	return m_Indices;
}