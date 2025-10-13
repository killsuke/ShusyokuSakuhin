#include "Mesh.h"

void Mesh::DefaultSetSubset() {

	std::unique_ptr<SUBSET> subset = std::make_unique<SUBSET>();

	subset->IndexNum = static_cast<unsigned int>(m_Indices.size());
	subset->VertexNum = static_cast<unsigned int>(m_Vertices.size());
	subset->IndexBase = 0;
	subset->VertexBase = 0;
	subset->MaterialIdx = 0;	// 単一マテリアルなので０
	subset->MtrlName = "Default";

	m_Subset.push_back(std::move(subset));
}