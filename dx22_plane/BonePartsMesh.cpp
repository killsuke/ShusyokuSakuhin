#include "BonePartsMesh.h"

using namespace DirectX::SimpleMath;

BonePartsMesh::BonePartsMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
}

