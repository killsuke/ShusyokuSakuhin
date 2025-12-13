#pragma once
#include "Mesh.h"

class TriangleMesh : public Mesh
{

public:
	TriangleMesh();
	~TriangleMesh() = default;

	// 頂点データ作成
	std::vector<VERTEX_3D> CreateMeshVertices() override;
	// インデックスデータ作成
	std::vector<unsigned int> CreateMeshIndices() override;
};

