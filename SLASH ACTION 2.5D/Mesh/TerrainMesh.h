#pragma once
#include "Mesh.h"
#include <DirectXMath.h>

class TerrainMesh : public Mesh
{
private:

	DirectX::XMFLOAT3 m_UVMagnifications = {};	// UV‚ÌŠg‘å—¦

public:

	TerrainMesh();
	~TerrainMesh() = default;
	std::vector<VERTEX_3D> CreateMeshVertices() override;
	std::vector<unsigned int> CreateMeshIndices() override;

	void ChangeUV(const DirectX::XMFLOAT3& scale);
	void SetUVMagnification(const DirectX::XMFLOAT3& magnification) {
		m_UVMagnifications = magnification;
	}
};