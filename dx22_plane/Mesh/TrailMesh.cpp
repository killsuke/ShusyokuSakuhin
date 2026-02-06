#include "TrailMesh.h"

using namespace DirectX;

namespace {
	constexpr size_t VERTEX_NUM = 4;
	constexpr size_t INDEX_NUM = 6;
}

TrailMesh::TrailMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
	DefaultSetMaterial();
	DefaultSetTexture();
}

std::vector<VERTEX_3D> TrailMesh::CreateMeshVertices() {
	// 頂点データ

	m_Vertices.resize(MaxTrailBufferSize::MAX_TRAIL_SEGMENTS * VERTEX_NUM);

	for (size_t i = 0; i < m_Vertices.size(); ++i) {
		m_Vertices[i].position = XMFLOAT3();
		m_Vertices[i].color = XMFLOAT4();
		m_Vertices[i].uv = XMFLOAT2();
		m_Vertices[i].normal = XMFLOAT3();
	}

	return m_Vertices;
}

std::vector<unsigned int> TrailMesh::CreateMeshIndices() {
	// インデックス情報生成

	m_Indices.resize(MaxTrailBufferSize::MAX_TRAIL_SEGMENTS * INDEX_NUM);

	for (size_t i = 0; i < m_Indices.size(); ++i) {
		m_Indices[i] = 0;
	}

	return m_Indices;
}

std::vector<VERTEX_3D> TrailMesh::CreateMeshVertices(const unsigned int size)
{
	size_t newSize = static_cast<size_t>(size);
	m_Vertices.clear();
	m_Vertices.resize(newSize);

	for (size_t i = 0; i < m_Vertices.size(); ++i) {
		m_Vertices[i].position = XMFLOAT3();
		m_Vertices[i].color = XMFLOAT4();
		m_Vertices[i].uv = XMFLOAT2();
		m_Vertices[i].normal = XMFLOAT3();
	}

	return m_Vertices;
}

std::vector<unsigned int> TrailMesh::CreateMeshIndices(const unsigned int size) {

	size_t newSize = static_cast<size_t>(size);
	m_Indices.clear();
	m_Indices.resize(newSize);

	for (size_t i = 0; i < m_Indices.size(); ++i) {
		m_Indices[i] = 0;
	}

	return m_Indices;
}