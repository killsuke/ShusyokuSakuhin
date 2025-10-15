#pragma once
#include	<vector>
#include	<SimpleMath.h>
#include	<memory>
#include	<string>
#include	"RenderElement.h"
#include "Texture.h"

class Mesh {
protected:
	std::vector<VERTEX_3D>		m_Vertices;		// 頂点座標群
	std::vector<unsigned int>	m_Indices;		// インデックスデータ群
	std::vector<std::unique_ptr<SUBSET>> m_Subset; // サブセット情報
	std::vector<std::unique_ptr<MATERIAL>> m_Materiales; // マテリアル情報
	std::vector<std::unique_ptr<Texture>> m_Textures;	// テクスチャ情報

public:
	Mesh() = default;
	~Mesh() = default;

	virtual std::vector<VERTEX_3D> CreateMeshVertices() = 0;
	virtual std::vector<unsigned int> CreateMeshIndices() = 0;

	void DefaultSetSubset();
	void DefaultSetMaterial();
	void DefaultSetTexture();

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

	// マテリアル情報取得
	std::vector<MATERIAL> GetMaterials() const {
		std::vector<MATERIAL> materials;
		for (const auto& ptr : m_Materiales) {
			materials.push_back(*ptr.get());
		}
		return materials;
	}

	// テクスチャ情報取得
	std::vector<Texture*> GetTextures() const {
		std::vector<Texture*> textures;
		for (const auto& ptr : m_Textures) {
			textures.push_back(ptr.get());
		}
		return textures;
	}

	void ChangeTexture(const std::string& filename) {
		if (m_Textures.size() == 1) {
			// 単一テクスチャの場合のみ変更可能
			m_Textures[0]->LoadTexture(filename);
		}
	}
};