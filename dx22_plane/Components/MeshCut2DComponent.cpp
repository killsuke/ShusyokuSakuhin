#include "MeshCut2DComponent.h"
#include "Mesh/SquareMesh.h"
#include "Render3D.h"
#include "Transform.h"
#include "VectorMoveComponent.h"
#include "input.h"
#include "Manager/GameObjectManager.h"
#include "DirectXRender.h"

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

	const bool cutKey = Input::GetKeyTrigger(VK_RETURN);

	if (cutKey == false) {
		return;
	}

	Texture texture = rendComp->GetTexture();
	std::string texName = texture.GetTexname();

	// 取り敢えず縦に左右に半分にカットする処理を書く
	// こんどは頂点バッファを書き換える処理を書く
	TransformComponent* trans = m_Object->GetComponent<TransformComponent>();
	Vector3 pos = trans->GetPosition();
	Vector3 size = trans->GetScale();

	GameObject* leftObj = GameObjectManager::AddObject("CutLeft", "CutPart");
	TransformComponent* leftTrans = leftObj->AddComponent<TransformComponent>();
	leftTrans->SetPosition(pos + Vector3(-size.x * 0.5f, pos.y, pos.z));
	leftTrans->SetScale(Vector3(size.x, size.y, size.z));
	VectorMoveComponent* leftMove = leftObj->AddComponent<VectorMoveComponent>();
	leftMove->SetMoveDirection({ -1.0f,0.0f,0.0f });
	leftMove->SetMovePower(2.0f);
	Render3DComponent* leftRend = leftObj->AddComponent<Render3DComponent>();
	leftRend->CreateMesh<SquareMesh>();
	leftRend->SetShader("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");
	leftRend->ChangeTexture(texName);

	GameObject* rightObj = GameObjectManager::AddObject("CutRight", "CutPart");
	TransformComponent* rightTrans = rightObj->AddComponent<TransformComponent>();
	rightTrans->SetPosition(pos + Vector3(size.x * 0.5f, pos.y, pos.z));
	rightTrans->SetScale(Vector3(size.x, size.y, size.z));
	VectorMoveComponent* rightMove = rightObj->AddComponent<VectorMoveComponent>();
	rightMove->SetMoveDirection({ 1.0f,0.0f,0.0f });
	rightMove->SetMovePower(2.0f);
	Render3DComponent* rightRend = rightObj->AddComponent<Render3DComponent>();
	rightRend->CreateMesh<SquareMesh>();
	rightRend->SetShader("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");
	rightRend->ChangeTexture(texName);

	m_Object->SetActiveState(ActiveState::ALL_STOP);

	std::vector<VERTEX_3D> leftVertices;
	std::vector<VERTEX_3D> rightVertices;

	leftVertices.resize(4);
	rightVertices.resize(4);

	leftVertices[0].position = Vector3(-0.5f,-1.0f, 0.0f);
	leftVertices[1].position = Vector3( 0.5f,-1.0f, 0.0f);
	leftVertices[2].position = Vector3( 0.5f, 1.0f, 0.0f);
	leftVertices[3].position = Vector3(-0.5f, 1.0f, 0.0f);

	leftVertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	leftVertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	leftVertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	leftVertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	leftVertices[0].uv = Vector2(0.0f, 1.0f);
	leftVertices[1].uv = Vector2(0.5f, 1.0f);
	leftVertices[2].uv = Vector2(0.5f, 0.0f);
	leftVertices[3].uv = Vector2(0.0f, 0.0f);

	leftVertices[0].normal = Vector3(0.0f, 0.0f, -1.0f);
	leftVertices[1].normal = Vector3(0.0f, 0.0f, -1.0f);
	leftVertices[2].normal = Vector3(0.0f, 0.0f, -1.0f);
	leftVertices[3].normal = Vector3(0.0f, 0.0f, -1.0f);

	VertexBuffer<VERTEX_3D>* vLBuffer = leftRend->GetVertexBuffer();
	vLBuffer->Modify(leftVertices);

	rightVertices[0].position = Vector3(-0.5f, -1.0f, 0.0f);
	rightVertices[1].position = Vector3( 0.5f, -1.0f, 0.0f);
	rightVertices[2].position = Vector3( 0.5f, 1.0f, 0.0f);
	rightVertices[3].position = Vector3(-0.5f, 1.0f, 0.0f);

	rightVertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	rightVertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	rightVertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	rightVertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	rightVertices[0].uv = Vector2(0.5f, 1.0f);
	rightVertices[1].uv = Vector2(1.0f, 1.0f);
	rightVertices[2].uv = Vector2(1.0f, 0.0f);
	rightVertices[3].uv = Vector2(0.5f, 0.0f);

	rightVertices[0].normal = Vector3(0.0f, 0.0f, -1.0f);
	rightVertices[1].normal = Vector3(0.0f, 0.0f, -1.0f);
	rightVertices[2].normal = Vector3(0.0f, 0.0f, -1.0f);
	rightVertices[3].normal = Vector3(0.0f, 0.0f, -1.0f);

	VertexBuffer<VERTEX_3D>* vRBuffer = rightRend->GetVertexBuffer();
	vRBuffer->Modify(rightVertices);


}