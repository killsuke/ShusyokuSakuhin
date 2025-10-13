// ============================================
// RenderComponent　
// いずれ切り分けを沢山して、
// 最低限の機能のみを持たせるようにする
// 汎用性を上げるために
// ============================================

#pragma once
#include "Component.h"
#include "RenderElement.h"

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<SimpleMath.h>
#include	<map>
#include	<iostream>
//#include	<io.h>
#include	<string>
#include	<vector>
#include	<memory>
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "BoneData.h"

//外部ライブラリ
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

class RenderComponent : public Component
{
protected:
	std::unique_ptr<Shader> m_Shader = nullptr;
	std::unique_ptr<Texture> m_Texture = nullptr;
	VertexBuffer<VERTEX_3D> m_VertexBuffer = {};
	IndexBuffer m_IndexBuffer = {};
	std::unique_ptr<Mesh> m_Mesh = nullptr;
	DirectX::SimpleMath::Vector4 m_Color = DirectX::SimpleMath::Vector4::One; // 色
	bool m_inversionFlag = false;

	RenderComponent(GameObject& obj);
	~RenderComponent() = default;
public:

	virtual void Update() = 0;
	void SetShader(const std::string& vertex, const std::string& pixel, const std::string& geometry = "", std::vector<D3D11_INPUT_ELEMENT_DESC> lay = std::vector<D3D11_INPUT_ELEMENT_DESC>{}) { m_Shader->Create(vertex, pixel, geometry, lay); };
	void SetTexture(const std::string& fileName = "assets/texture/NoTexture.png") { m_Texture->LoadTexture(fileName); };
	void SetTextureAndMask(const std::string& fileName, const std::string& maskFileName) {
		m_Texture->Load(fileName);
		m_Texture->LoadMask(maskFileName);
	};
	void SetMesh(Mesh& mesh);
	void SetColor(const DirectX::SimpleMath::Vector4 color) { m_Color = color; };
	void SetInversionFlag(const bool flag) { m_inversionFlag = flag; };

	Mesh* GetMesh() { return m_Mesh.get(); };
	Texture* GetTexture() { return m_Texture.get(); };


	template<class T>
	void CreateMesh() {
		static_assert(std::is_base_of_v<Mesh, T>, "T must inherit from Mesh");
		m_Mesh = std::make_unique<T>();
		m_VertexBuffer.Create(m_Mesh->GetVertices());
		m_IndexBuffer.Create(m_Mesh->GetIndices());
	}
};

