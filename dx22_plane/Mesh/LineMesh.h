#pragma once
#include "Mesh/Mesh.h"
#include <DirectXMath.h>

struct LineStatus
{
	DirectX::XMFLOAT3 start = DirectX::XMFLOAT3(); // 線の始点
	DirectX::XMFLOAT3 end = DirectX::XMFLOAT3();   // 線の終点

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

