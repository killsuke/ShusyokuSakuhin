#pragma once
#include "Mesh.h"
#include <SimpleMath.h>

struct LineStatus
{
	DirectX::SimpleMath::Vector3 start; // 線の始点
	DirectX::SimpleMath::Vector3 end;   // 線の終点

	float width;                     // 線の太さ
};

class LineMesh : public Mesh
{

public:
	LineMesh();
	~LineMesh() = default;

	std::vector<VERTEX_3D> CreateMeshVertices()override;
	std::vector<unsigned int> CreateMeshIndices()override;
};

