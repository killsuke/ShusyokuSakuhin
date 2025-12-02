#include "MeshCut2DComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

MeshCut2DComponent::MeshCut2DComponent(GameObject& obj) : Component(obj)
{
	// コンポーネントのソート番号を設定（あとで）
	m_SortNum = ComponentTypeManager::GetID_FromName("MESH_CUT_2D"); // ソート番号を設定

	
}

void MeshCut2DComponent::Update()
{



}