#pragma once
#include "Mesh.h"

class CubeMesh final : public Mesh
{
public:
	CubeMesh();
	~CubeMesh() = default;

	std::vector<VERTEX_3D> CreateMeshVertices()override;
	std::vector<unsigned int> CreateMeshIndices()override;
};

