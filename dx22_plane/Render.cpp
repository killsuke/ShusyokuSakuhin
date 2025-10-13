#include "Render.h"
#include "Transform.h"

RenderComponent::RenderComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ƒ\[ƒg”Ô†‚ğİ’è
}


void RenderComponent::SetMesh(Mesh& mesh) {

	//m_Mesh = &mesh;
	m_VertexBuffer.Create(mesh.GetVertices());
	m_IndexBuffer.Create(mesh.GetIndices());
};