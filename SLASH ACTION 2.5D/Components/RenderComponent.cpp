#include "RenderComponent.h"
#include "TransformComponent.h"

RenderComponent::RenderComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
}

StaticMesh* RenderComponent::LoadModelMesh(const std::string& filename, const std::string& texturedirectory) {
	
	m_Mesh = std::make_unique<StaticMesh>();

	StaticMesh* staticMesh = static_cast<StaticMesh*>(m_Mesh.get());
	
	if(staticMesh == nullptr) {
		std::cerr << "Error: Failed to create StaticMesh." << std::endl;
		return nullptr;
	}

	staticMesh->Load(filename, texturedirectory);
	const std::vector<VERTEX_3D>& vertices = staticMesh->CreateMeshVertices();
	const std::vector<unsigned int>& indices = staticMesh->CreateMeshIndices();

	if(vertices.empty() == true || indices.empty() == true) {
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