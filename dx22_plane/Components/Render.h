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
#include	<map>
#include	<iostream>
#include	<string>
#include	<vector>
#include	<memory>
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh/Mesh.h"
#include "BoneData.h"
#include "StaticMesh.h"
#include "RightLeft.h"

//外部ライブラリ
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

class RenderComponent : public Component
{
protected:
	std::unique_ptr<Shader> m_Shader = nullptr;
	std::unique_ptr<Mesh> m_Mesh = nullptr;
	VertexBuffer<VERTEX_3D> m_VertexBuffer = {};
	IndexBuffer m_IndexBuffer = {};
	DirectX::XMFLOAT4 m_Color = { 1.0f,1.0f, 1.0f, 1.0f }; // 色
	RightLeft m_Inversion = RightLeft::RIGHT;

	RenderComponent(GameObject& obj);
	~RenderComponent() = default;
public:

	virtual void Update() = 0;
	void SetShader(const std::string& vertex, const std::string& pixel, const std::string& geometry = "", std::vector<D3D11_INPUT_ELEMENT_DESC> lay = std::vector<D3D11_INPUT_ELEMENT_DESC>{}) { m_Shader->Create(vertex, pixel, geometry, lay); };

	// テクスチャを変更する（メッシュに装備されたテクスチャが１枚なら）
	void ChangeTexture(const std::string& fileName = "assets/texture/NoTexture.png") {
		if (m_Mesh == nullptr) return;

		m_Mesh->ChangeTexture(fileName);
	};
	void SetColor(const DirectX::XMFLOAT4& color) { m_Color = color; };
	void SetInversionFlag(const RightLeft& flag) { m_Inversion = flag; };

	Mesh* GetMesh() { return m_Mesh.get(); };
	Texture GetTexture() {

		const std::vector<Texture> texs = m_Mesh->GetTextures();
		// 単一テクスチャの場合のみ返す
		if (texs.size() == 0) {
			return Texture();
		}
		return texs[0];
	};

	VertexBuffer<VERTEX_3D>* GetVertexBuffer() { return &m_VertexBuffer; };
	IndexBuffer* GetIndexBuffer() { return &m_IndexBuffer; };
	RightLeft GetInversionFlag()const { return m_Inversion; };
	Shader* GetShader()const { return m_Shader.get(); };
	DirectX::XMFLOAT4 GetColor()const { return m_Color; };

	void AddColor(const DirectX::XMFLOAT4& addcolor) {
		m_Color.x += addcolor.x;
		m_Color.y += addcolor.y;
		m_Color.z += addcolor.z;
		m_Color.w += addcolor.w;
	};

	void AddColor_X(const float color) { m_Color.x += color; };
	void AddColor_Y(const float color) { m_Color.y += color; };
	void AddColor_Z(const float color) { m_Color.z += color; };
	void AddColor_A(const float color) { m_Color.w += color; };

	StaticMesh* LoadModelMesh(const std::string& filename, const std::string& texturedirectory);

	template<class T>
	T* CreateMesh() {
		static_assert(std::is_base_of_v<Mesh, T>, "T must inherit from Mesh");
		std::unique_ptr<T> mesh = std::make_unique<T>();
		const std::vector<VERTEX_3D>& vertices = mesh->CreateMeshVertices();
		const std::vector<unsigned int>& indices = mesh->CreateMeshIndices();
		if (vertices.empty() == true || indices.empty() == true) {
			std::cerr << "Error: Mesh creation failed. Vertices or indices are empty." << std::endl;
			return nullptr;
		}
		m_VertexBuffer.Create(vertices);
		m_IndexBuffer.Create(indices);
		T* raw = mesh.get();
		m_Mesh = std::move(mesh);
		return raw;
	}
};
