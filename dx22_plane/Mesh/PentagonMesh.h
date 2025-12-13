#pragma once
#include "Mesh.h"
class PentagonMesh : public Mesh
{
public:

	PentagonMesh();
	~PentagonMesh() = default;

	std::vector<VERTEX_3D> CreateMeshVertices() override;
	std::vector<unsigned int> CreateMeshIndices() override;
};