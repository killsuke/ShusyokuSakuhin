#pragma once
#include "Mesh.h"

class SkyDomeMesh : public Mesh
{
private:
	const int latitudeBands = 16;
	const int longitudeBands = 32;
	float radius = 1.0f;

public:
	SkyDomeMesh();
	~SkyDomeMesh() = default;

	std::vector<VERTEX_3D> CreateMeshVertices()override;
	std::vector<unsigned int> CreateMeshIndices()override;
};
