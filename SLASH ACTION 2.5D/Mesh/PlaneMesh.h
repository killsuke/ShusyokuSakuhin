#pragma once
#include "Mesh.h"

class PlaneMesh :  public Mesh
{
public:
	PlaneMesh();
	~PlaneMesh() = default;

	std::vector<VERTEX_3D> CreateMeshVertices() override;
	std::vector<unsigned int> CreateMeshIndices() override;
};