#include "RenderComponent.h"
#include "TransformComponent.h"

using namespace DirectX;

RenderComponent::RenderComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
}

StaticMesh* RenderComponent::LoadModelMesh(const std::string& filename, const std::string& texturedirectory) {

	m_Mesh = std::make_unique<StaticMesh>();

	StaticMesh* staticMesh = static_cast<StaticMesh*>(m_Mesh.get());

	staticMesh->Load(filename, texturedirectory);

	const std::vector<VERTEX_3D>& vertices = staticMesh->CreateMeshVertices();
	const std::vector<unsigned int>& indices = staticMesh->CreateMeshIndices();

	if (vertices.empty() == true || indices.empty() == true) {
		std::cerr << "Error: Mesh loading failed. Vertices or indices are empty." << std::endl;
		return nullptr;
	}

	m_VertexBuffer.Create(vertices);
	m_IndexBuffer.Create(indices);

	return staticMesh;
}

void RenderComponent::PrimitiveTypeUpdate(ID3D11DeviceContext* context) {
	// 要求されたプリミティブタイプが現在のプリミティブタイプと異なる場合、更新する
	if (m_PrimitiveType != m_RequestPrimitive) {
		m_PrimitiveType = m_RequestPrimitive;

		context->IASetPrimitiveTopology(m_PrimitiveType); // プリミティブタイプを設定
	}
}

DirectX::XMMATRIX RenderComponent::MakeRenderMatrix(const DirectX::XMMATRIX& mtx) {

	DirectX::XMMATRIX renderMatrix = XMMatrixIdentity();

	// SRT情報作成
	// 各行列を生成
	const XMMATRIX r = XMMatrixRotationQuaternion(m_RenderOffset.quaternion);
	const XMVECTOR scale = XMVectorSet(m_RenderOffset.scale.x, m_RenderOffset.scale.y, m_RenderOffset.scale.z, 1.0f);
	const XMMATRIX s = XMMatrixScalingFromVector(scale);
	const XMVECTOR pos = XMVectorSet(m_RenderOffset.position.x, m_RenderOffset.position.y, m_RenderOffset.position.z, 1.0f);
	const XMMATRIX t = XMMatrixTranslationFromVector(pos);

	renderMatrix = s * r * t * mtx; // ワールド行列にSRTを掛ける

	return renderMatrix;
}