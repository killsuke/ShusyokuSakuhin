#pragma once
#include "Mesh.h"

class TriangularPrismMesh : public Mesh
{
private:

public:
	TriangularPrismMesh();
	~TriangularPrismMesh() = default;

	std::vector<VERTEX_3D> CreateMeshVertices() override;
	std::vector<unsigned int> CreateMeshIndices() override;

};