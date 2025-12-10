#include "MeshCut2DComponent.h"
#include "Mesh/SquareMesh.h"
#include "Components/Render3D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

MeshCut2DComponent::MeshCut2DComponent(GameObject& obj) : Component(obj)
{
	// コンポーネントのソート番号を設定（あとで）
	m_SortNum = ComponentTypeManager::GetID_FromName("MESH_CUT_2D"); // ソート番号を設定

	m_StartCutPoint = { 0.0,70.0f,0.0f };
	m_GoalCutPoint = { 0.0f,-70.0f,0.0f };
}

void MeshCut2DComponent::Update()
{
	Render3DComponent* rendComp = m_Object->GetComponent<Render3DComponent>();
	if (rendComp == nullptr) {
		return;
	}

	Mesh* mesh = rendComp->GetMesh();
	if(mesh == nullptr) {
		return;
	}

	SquareMesh* squareMesh = dynamic_cast<SquareMesh*>(mesh);
	if (squareMesh == nullptr) {
		return;
	}




}