#pragma once
#include	<vector>
#include	<memory>
#include	<string>
#include	"Structs/RenderElement.h"
#include "Texture/Texture.h"

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

	Mesh(const Mesh& other);

	virtual std::vector<VERTEX_3D> CreateMeshVertices() = 0;
	virtual std::vector<unsigned int> CreateMeshIndices() = 0;

	void DefaultSetSubset();
	void DefaultSetMaterial();
	void DefaultSetTexture();

	void SetTextureUV(const float nu,const float nv,const float sx,const float sy) {
		// 単一テクスチャの場合のみ設定可能
		if(m_Textures.size() == 1) {
			m_Textures[0]->SetUV(nu, nv, sx, sy);
		}
	}

	void SetInitialCut(const float sx,const float sy) {
		// 単一テクスチャの場合のみ設定可能
		if (m_Textures.size() == 1) {
			m_Textures[0]->SetInitialCut(sx, sy);
		}
	}

	void SetInitialCut(const DirectX::XMFLOAT2& scut) {
		// 単一テクスチャの場合のみ設定可能
		if (m_Textures.size() == 1) {
			m_Textures[0]->SetInitialCut(scut.x, scut.y);
		}
	}

	void SetCutNum(const float nu,const float nv) {
		// 単一テクスチャの場合のみ設定可能
		if (m_Textures.size() == 1) {
			m_Textures[0]->SetCutNum(nu, nv);
		}
	}

	void SetCutNum(const DirectX::XMFLOAT2& ncut) {
		// 単一テクスチャの場合のみ設定可能
		if (m_Textures.size() == 1) {
			m_Textures[0]->SetCutNum(ncut.x, ncut.y);
		}
	}

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
		for (const std::unique_ptr<SUBSET>& ptr : m_Subset) {
			subsets.push_back(*ptr.get());
		}

		return subsets;
	}

	std::vector<SUBSET*> GetTestSubsets() {
		std::vector<SUBSET*> subsets;
		for (const std::unique_ptr<SUBSET>& ptr : m_Subset) {
			subsets.push_back(ptr.get());
		}

		return subsets;
	}

	// マテリアル情報取得
	std::vector<MATERIAL> GetMaterials() const {
		std::vector<MATERIAL> materials;
		for (const std::unique_ptr<MATERIAL>& ptr : m_Materiales) {
			materials.push_back(*ptr.get());
		}
		return materials;
	}

	std::vector<MATERIAL*> GetTestMaterials() const {
		std::vector<MATERIAL*> materials;
		for (const std::unique_ptr<MATERIAL>& ptr : m_Materiales) {
			materials.push_back(ptr.get());
		}
		return materials;
	}

	// テクスチャ情報取得
	std::vector<Texture> GetTextures() const {
		std::vector<Texture> textures;
		for (const std::unique_ptr<Texture>& ptr : m_Textures) {
			if (ptr == nullptr) {
				textures.push_back(Texture());
				continue;
			}

			textures.push_back(*ptr.get());
		}
		return textures;
	}

	DirectX::XMFLOAT4 GetUVSets() const {
		if (m_Textures.size() == 1) {
			return m_Textures[0]->GetUVSets();
		}
		return DirectX::XMFLOAT4();
	}

	void ChangeTexture(const std::string& filename) {
		if (m_Textures.size() == 1) {
			// 単一テクスチャの場合のみ変更可能
			m_Textures[0]->LoadTexture(filename);
		}
	}
};