#pragma once
#include "Mesh.h"
class BonePartsMesh : public Mesh
{
public:
	BonePartsMesh();
	~BonePartsMesh() = default;

	std::vector<VERTEX_3D> CreateMeshVertices()  override { return m_Vertices; };
	std::vector<unsigned int> CreateMeshIndices() override { return m_Indices; };
};

