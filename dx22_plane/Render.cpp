#include "Render.h"
#include "Transform.h"

RenderComponent::RenderComponent(GameObject& obj) :Component(obj) {

}

void RenderComponent::Update()
{

}

void RenderComponent::SetMesh(Mesh& mesh) {
	m_VertexBuffer.Create(mesh.GetVertices());
	m_IndexBuffer.Create(mesh.GetIndices());
};