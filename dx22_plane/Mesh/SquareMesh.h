#pragma once
#include "Mesh.h"
class SquareMesh : public Mesh
{
public:
	SquareMesh();
	~SquareMesh() = default;

	std::vector<VERTEX_3D> CreateMeshVertices() override;
	std::vector<unsigned int> CreateMeshIndices() override;
};
