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
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_Texture;
	VertexBuffer<VERTEX_3D> m_VertexBuffer = {};
	IndexBuffer m_IndexBuffer = {};
	DirectX::XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,1.0f }; // 色

	RenderComponent(GameObject& obj);
	~RenderComponent() = default;
public:

	virtual void Update() = 0;
	void SetShader(const std::string& vertex, const std::string& pixel) { m_Shader->Create(vertex, pixel); };
	void SetTexture(const std::string& fileName) { m_Texture->Load(fileName); };
	void SetTextureAndMask(const std::string& fileName, const std::string& maskFileName) {
		m_Texture->Load(fileName);
		m_Texture->LoadMask(maskFileName);
	};
	void SetMesh(Mesh& mesh);
	void SetColor(const DirectX::XMFLOAT4 color) { m_Color = color; };
};

