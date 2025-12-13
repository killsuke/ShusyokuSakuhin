#include "TriangleMesh.h"

using namespace DirectX::SimpleMath;

namespace {
	unsigned int vertexSize = 3;
	unsigned int indexSize = 3;
}

TriangleMesh::TriangleMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
	DefaultSetMaterial();
	DefaultSetTexture();
}

std::vector<VERTEX_3D> TriangleMesh::CreateMeshVertices() {
	// 頂点データ

	m_Vertices.resize(vertexSize);

	m_Vertices[0].position = Vector3(-1.0f, -1.0f, 0.0f);
	m_Vertices[1].position = Vector3(0.0f, 1.0f, 0.0f);
	m_Vertices[2].position = Vector3(1.0f, -1.0f, 0.0f);


	m_Vertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);


	m_Vertices[0].uv = Vector2(0.0f, 1.0f);	// 左下
	m_Vertices[1].uv = Vector2(0.5f, 0.0f);	// 真ん中上
	m_Vertices[2].uv = Vector2(1.0f, 1.0f);	// 右下


	m_Vertices[0].normal = Vector3(0.0f, 0.0f, -1.0f);
	m_Vertices[1].normal = Vector3(0.0f, 0.0f, -1.0f);
	m_Vertices[2].normal = Vector3(0.0f, 0.0f, -1.0f);

	return m_Vertices;
}

std::vector<unsigned int> TriangleMesh::CreateMeshIndices() {
	// インデックス情報生成

	m_Indices.resize(indexSize);

	m_Indices = {
		0,1,2
	};

	return m_Indices;
}