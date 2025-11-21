#include "PlaneMesh.h"

using namespace DirectX::SimpleMath;

namespace {
	unsigned int vertexSize = 4;
	unsigned int indexSize = 6;
}

PlaneMesh::PlaneMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
	DefaultSetMaterial();
	DefaultSetTexture();
}

std::vector<VERTEX_3D> PlaneMesh::CreateMeshVertices() {
	// 頂点データ

	m_Vertices.resize(vertexSize);

	m_Vertices[0].position = Vector3(-1.0f, 0.0f, -1.0f);
	m_Vertices[1].position = Vector3( 1.0f, 0.0f, -1.0f);
	m_Vertices[2].position = Vector3( 1.0f, 0.0f, 1.0f);
	m_Vertices[3].position = Vector3(-1.0f, 0.0f, 1.0f);


	m_Vertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);


	m_Vertices[0].uv = Vector2(0.0f, 1.0f);	// 右下
	m_Vertices[1].uv = Vector2(1.0f, 1.0f);	// 左下
	m_Vertices[2].uv = Vector2(1.0f, 0.0f);	// 左上
	m_Vertices[3].uv = Vector2(0.0f, 0.0f);	// 右上


	m_Vertices[0].normal = Vector3(0.0f, 1.0f, 0.0f);
	m_Vertices[1].normal = Vector3(0.0f, 1.0f, 0.0f);
	m_Vertices[2].normal = Vector3(0.0f, 1.0f, 0.0f);
	m_Vertices[3].normal = Vector3(0.0f, 1.0f, 0.0f);

	return m_Vertices;
}

std::vector<unsigned int> PlaneMesh::CreateMeshIndices() {
	// インデックス情報生成

	m_Indices.resize(indexSize);

	m_Indices = {
		2,1,0,
		0,3,2,
	};

	return m_Indices;
}