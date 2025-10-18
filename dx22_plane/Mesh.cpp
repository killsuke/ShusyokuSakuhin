#include "Mesh.h"

Mesh::Mesh(const Mesh& other) : m_Vertices(other.m_Vertices),m_Indices(other.m_Indices) {

	// サブセット情報のコピー
	for (const auto& subsetPtr : other.m_Subset) {
		m_Subset.emplace_back(subsetPtr->Clone());
	}
	// マテリアル情報のコピー
	for (const auto& materialPtr : other.m_Materiales) {
		m_Materiales.emplace_back(materialPtr->Clone());
	}
	// テクスチャ情報のコピー
	for (const auto& texturePtr : other.m_Textures) {
		if (texturePtr) {
			m_Textures.emplace_back(texturePtr->Clone());
		}
		else {
			m_Textures.emplace_back(nullptr);
		}
	}
}

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

void Mesh::DefaultSetMaterial() {
	std::unique_ptr<MATERIAL> material = std::make_unique<MATERIAL>();
	material->Ambient = DirectX::SimpleMath::Vector4(0.3f, 0.3f, 0.3f, 1.0f);
	material->Diffuse = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material->Specular = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 16.0f); // wはスぺキュラーの強さ
	material->Emission = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	material->Shiness = 16.0f;
	m_Materiales.push_back(std::move(material));
}

void Mesh::DefaultSetTexture() {
	std::unique_ptr<Texture> texture = std::make_unique<Texture>();
	texture->LoadTexture("assets/texture/NoTexture.png");
	m_Textures.push_back(std::move(texture));
}