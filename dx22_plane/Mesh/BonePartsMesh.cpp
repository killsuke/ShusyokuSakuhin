#include "BonePartsMesh.h"

BonePartsMesh::BonePartsMesh() {
	CreateMeshVertices();
	CreateMeshIndices();
	DefaultSetSubset();
	DefaultSetMaterial();
	DefaultSetTexture();
}

