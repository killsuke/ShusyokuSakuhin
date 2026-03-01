#pragma once
#include "Mesh.h"

namespace MaxTrailBufferSize{

	constexpr size_t MAX_TRAIL_SEGMENTS = 64;
}

class TrailMesh :  public Mesh
{
public:

	TrailMesh();
	~TrailMesh() = default;

	std::vector<VERTEX_3D> CreateMeshVertices()override;
	std::vector<unsigned int> CreateMeshIndices()override;
	std::vector<VERTEX_3D> CreateMeshVertices(const unsigned int size);
	std::vector<unsigned int> CreateMeshIndices(const unsigned int size);
};