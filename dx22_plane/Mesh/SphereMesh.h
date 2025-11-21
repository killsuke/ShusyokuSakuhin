#pragma once
#include "Mesh.h"
class SphereMesh : public Mesh
{
private:
	const int latitudeBands = 16;
	const int longitudeBands = 32;

public:
	SphereMesh();
	~SphereMesh() = default;

	std::vector<VERTEX_3D> CreateMeshVertices()override;
	std::vector<unsigned int> CreateMeshIndices()override;
};