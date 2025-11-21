#pragma once
#include "Mesh.h"
class CircleMesh : public Mesh
{
private:
	const int numSegments = 32;
	float radius = 1.0f;
public:
	CircleMesh();
	~CircleMesh() = default;

	std::vector<VERTEX_3D> CreateMeshVertices()override;
	std::vector<unsigned int> CreateMeshIndices()override;
};

