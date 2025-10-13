#pragma once
#include	<vector>
#include	<SimpleMath.h>
#include	<memory>
#include	<string>
#include	"RenderElement.h"

class Mesh {
protected:
	std::vector<VERTEX_3D>		m_Vertices;		// 頂点座標群
	std::vector<unsigned int>	m_Indices;		// インデックスデータ群
	std::vector<std::unique_ptr<SUBSET>> m_Subset; // サブセット情報
	std::vector<std::unique_ptr<MATERIAL>> m_Materiales; // マテリアル情報

public:
	Mesh() = default;
	~Mesh() = default;

	virtual std::vector<VERTEX_3D> CreateMeshVertices() = 0;
	virtual std::vector<unsigned int> CreateMeshIndices() = 0;

	void DefaultSetSubset();

	// 頂点データ取得
	const std::vector<VERTEX_3D>& GetVertices() {
		return m_Vertices;
	}

	// インデックスデータ取得
	const std::vector<unsigned int>& GetIndices() {
		return m_Indices;
	}

	// サブセット情報取得
	std::vector<SUBSET> GetSubsets() const {
		std::vector<SUBSET> subsets;
		for (const auto& ptr : m_Subset) {
			subsets.push_back(*ptr.get());
		}
		
		return subsets;
	}
};