// ============================================
// RenderComponent　
// いずれ切り分けを沢山して、
// 最低限の機能のみを持たせるようにする
// 汎用性を上げるために
// ============================================

#pragma once
#include "Component.h"
#include "Structs/RenderElement.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Shader/Shader.h"
#include "Texture/Texture.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/IndexBuffer.h"
#include "Mesh/Mesh.h"
#include "Structs/BoneData.h"
#include "Mesh/StaticMesh.h"
#include "Structs/RightLeft.h"
#include "System/DirectXRender.h"

//外部ライブラリ
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

struct RenderOffset {
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3();
	DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3();
	DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionIdentity();
};

class RenderComponent : public Component
{
protected:
	std::unique_ptr<Shader> m_Shader = nullptr;
	std::unique_ptr<Mesh> m_Mesh = nullptr;
	VertexBuffer<VERTEX_3D> m_VertexBuffer = {};
	IndexBuffer m_IndexBuffer = {};
	DirectX::XMFLOAT4 m_Color = { 1.0f,1.0f, 1.0f, 1.0f }; // 色
	RightLeft m_Inversion = RightLeft::RIGHT;
	D3D_PRIMITIVE_TOPOLOGY m_PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;		// 現在のプリミティブタイプ
	D3D_PRIMITIVE_TOPOLOGY m_RequestPrimitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	// 要求されたプリミティブタイプ
	RenderOffset m_RenderOffset = {};

	RenderComponent(GameObject& obj);
	~RenderComponent() = default;

	void PrimitiveTypeUpdate(ID3D11DeviceContext* context);

public:

	virtual void Update() = 0;
	void SetShader(const std::string& vertex, const std::string& pixel, const std::string& geometry = "", std::vector<D3D11_INPUT_ELEMENT_DESC> lay = std::vector<D3D11_INPUT_ELEMENT_DESC>{}) { m_Shader->Create(vertex, pixel, geometry, lay); };

	// テクスチャを変更する（メッシュに装備されたテクスチャが１枚なら）
	void ChangeTexture(const std::string& fileName = "NoTexture.png") {
		if (m_Mesh == nullptr) return;

		m_Mesh->ChangeTexture(fileName);
	};
	void SetColor(const DirectX::XMFLOAT4& color) { m_Color = color; };
	void SetInversionFlag(const RightLeft& flag) { m_Inversion = flag; };
	void SetPrimitiveType(const D3D_PRIMITIVE_TOPOLOGY& type) { m_RequestPrimitive = type; };
	void SetRenderOffset(const RenderOffset& offset) { m_RenderOffset = offset; };
	void SetRenderOffset(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const DirectX::XMFLOAT3& sca) {
		m_RenderOffset.position = pos;
		SetRenderOffsetRotation(rot); // 回転をセットする際にクォータニオンも更新する
		m_RenderOffset.scale = sca;
	};
	void SetRenderOffset(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const DirectX::XMFLOAT3& sca, const DirectX::XMVECTOR& qua) {
		m_RenderOffset.position = pos;
		SetRenderOffsetRotation(rot); // 回転をセットする際にクォータニオンも更新する
		m_RenderOffset.scale = sca;
	};
	void SetRenderOffsetPosition(const DirectX::XMFLOAT3& pos) { m_RenderOffset.position = pos; };
	void SetRenderOffsetRotation(const DirectX::XMFLOAT3& rot) { 
		m_RenderOffset.rotation = rot;
	
		const float pitch = DirectX::XMConvertToRadians(m_RenderOffset.rotation.x);
		const float yaw = DirectX::XMConvertToRadians(m_RenderOffset.rotation.y);
		const float roll = DirectX::XMConvertToRadians(m_RenderOffset.rotation.z);

		m_RenderOffset.quaternion = DirectX::XMQuaternionRotationRollPitchYaw(
			pitch,
			yaw,
			roll
		);
	};
	void SetRenderOffsetScale(const DirectX::XMFLOAT3& sca) { m_RenderOffset.scale = sca; };
	void SetRenderOffsetQuaternion(const DirectX::XMVECTOR& qua) { m_RenderOffset.quaternion = qua; };

	void AddRenderOffsetPosition(const DirectX::XMFLOAT3& pos) {
		m_RenderOffset.position.x += pos.x;
		m_RenderOffset.position.y += pos.y;
		m_RenderOffset.position.z += pos.z;
	};
	void AddRenderOffsetRotation(const DirectX::XMFLOAT3& rot) {
		m_RenderOffset.rotation.x += rot.x;
		m_RenderOffset.rotation.y += rot.y;
		m_RenderOffset.rotation.z += rot.z;
		const float pitch = DirectX::XMConvertToRadians(m_RenderOffset.rotation.x);
		const float yaw = DirectX::XMConvertToRadians(m_RenderOffset.rotation.y);
		const float roll = DirectX::XMConvertToRadians(m_RenderOffset.rotation.z);
		m_RenderOffset.quaternion = DirectX::XMQuaternionRotationRollPitchYaw(
			pitch,
			yaw,
			roll
		);
	};
	void AddRenderOffsetScale(const DirectX::XMFLOAT3& sca) {
		m_RenderOffset.scale.x += sca.x;
		m_RenderOffset.scale.y += sca.y;
		m_RenderOffset.scale.z += sca.z;
	};

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
	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveType()const { return m_PrimitiveType; };

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

	DirectX::XMMATRIX MakeRenderMatrix(const DirectX::XMMATRIX& mtx);

	template<class T>
	T* CreateMesh() {
		static_assert(std::is_base_of_v<Mesh, T>, "T must inherit from Mesh");
		std::unique_ptr<T> mesh = std::make_unique<T>();
		const std::vector<VERTEX_3D>& vertices = mesh->GetVertices();
		const std::vector<unsigned int>& indices = mesh->GetIndices();
		if (vertices.empty() == true || indices.empty() == true) {
			// メッシュの作成に失敗した場合はエラーメッセージを出力してnullptrを返す
			MessageBoxW(nullptr, L"Mesh creation failed: vertices or indices are empty", L"Error", MB_OK | MB_ICONWARNING);
			return nullptr;
		}
		m_VertexBuffer.Create(vertices);
		m_IndexBuffer.Create(indices);
		T* raw = mesh.get();
		m_Mesh = std::move(mesh);
		return raw;
	}
};
