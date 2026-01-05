#include "RenderLine2DComponent.h"
#include "System/DirectXRender.h"
#include "Transform.h"
#include "Manager/GameObjectManager.h"
#include "RenderBillboard.h"
#include "Mesh/CircleMesh.h"
#include "Mesh/LineMesh.h"
#include "input.h"
#include "FollowPositionComponent.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

RenderLine2DComponent::RenderLine2DComponent(GameObject& obj) : RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	//m_Texture = std::make_unique<Texture>();

	CreateMesh<LineMesh>();
	// 専用のインプットレイアウトもここで作成予定
	SetShader("shader/LineVS.hlsl", "shader/unlitTexturePS.hlsl", "shader/unlitTexture2D_GS.hlsl");

	m_StartObj = GameObjectManager::AddAbsFront("startPoint", "LineObj");
	TransformComponent* transS = m_StartObj->AddComponent<TransformComponent>();
	transS->SetScale({ 10.0f,10.0f,10.0f });
	transS->SetPosition({ 0.0f,50.0f,0.0f });
	RenderBillboardComponent* rendS = m_StartObj->AddComponent<RenderBillboardComponent>();

	rendS->CreateMesh<CircleMesh>();
	rendS->SetShader("shader/unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
	rendS->ChangeTexture("assets/texture/NoTexture.png");

	m_EndObj = GameObjectManager::AddAbsFront("endPoint", "LineObj");
	TransformComponent* transE = m_EndObj->AddComponent<TransformComponent>();
	transE->SetScale({ 10.0f,10.0f,10.0f });
	transE->SetPosition({ 0.0f,-50.0f,0.0f });
	RenderBillboardComponent* rendE = m_EndObj->AddComponent<RenderBillboardComponent>();

	rendE->CreateMesh<CircleMesh>();
	rendE->SetShader("shader/unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
	rendE->ChangeTexture("assets/texture/NoTexture.png");
}

void RenderLine2DComponent::Update()
{
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr && m_Mesh != nullptr) {
		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

		D3D11_MAPPED_SUBRESOURCE mapped = {};
		deviceContext->Map(m_VertexBuffer.GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

		VERTEX_3D* vtx = reinterpret_cast<VERTEX_3D*>(mapped.pData);

		// ここで全頂点データを更新
		Vector3 pos_S = m_StartObj->GetComponent<TransformComponent>()->GetPosition();
		Vector3 pos_E = m_EndObj->GetComponent<TransformComponent>()->GetPosition();
		
		vtx[0].position = pos_S;
		vtx[0].normal = Vector3(0.0f, 1.0f, 0.0f);
		vtx[0].color = m_Color;
		vtx[0].uv = Vector2(0.0f, 0.0f);

		//		pos_E.z -= 4.0f;
		vtx[1].position = pos_E;
		vtx[1].normal = Vector3(0.0f, 1.0f, 0.0f);
		vtx[1].color = m_Color;
		vtx[1].uv = Vector2(1.0f, 0.0f);

		deviceContext->Unmap(m_VertexBuffer.GetBuffer(), 0);

		// 線の太さ
		LineThickness thick;
		thick.thickness = m_Thickness;

		deviceContext->UpdateSubresource(DirectXRender::GetLineThicknessBuffer(), 0, NULL, &thick, 0, 0);


		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();
		//	m_Texture->SetGPU();

		auto subsets = m_Mesh->GetSubsets();

		auto materials = m_Mesh->GetMaterials();

		auto textures = m_Mesh->GetTextures();

		//マテリアル数分ループ 
		for (int i = 0; i < subsets.size(); ++i)
		{
			// ここ使う
			MATERIAL material = materials[subsets[i].MaterialIdx];

			deviceContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &material, 0, 0);

			textures[subsets[i].MaterialIdx].SetGPU();

			deviceContext->DrawIndexed(
				subsets[i].IndexNum,		// 描画するインデックス数
				subsets[i].IndexBase,		// 最初のインデックスバッファの位置	
				subsets[i].VertexBase);	// 頂点バッファの最初から使用
		}
	}
}

void RenderLine2DComponent::SetStartPosition(const DirectX::SimpleMath::Vector3& startPos) {
	TransformComponent* transS = m_StartObj->GetComponent<TransformComponent>();

	transS->SetPosition(startPos);
}

void RenderLine2DComponent::SetEndPosition(const DirectX::SimpleMath::Vector3& endPos) {
	TransformComponent* transE = m_EndObj->GetComponent<TransformComponent>();

	transE->SetPosition(endPos);
}

void RenderLine2DComponent::SetStartAndEndPosition(const DirectX::SimpleMath::Vector3& startPos, const DirectX::SimpleMath::Vector3& endPos) {
	TransformComponent* transS = m_StartObj->GetComponent<TransformComponent>();
	TransformComponent* transE = m_EndObj->GetComponent<TransformComponent>();

	transS->SetPosition(startPos);
	transE->SetPosition(endPos);
}

void RenderLine2DComponent::SetStartAndEndFollowObject(GameObject* objS, GameObject* objE) {

	FollowPositionComponent* followS = m_StartObj->AddComponent<FollowPositionComponent>();
	followS->SetFollowObject(objS);
	FollowPositionComponent* followE = m_EndObj->AddComponent<FollowPositionComponent>();
	followE->SetFollowObject(objE);
}

void RenderLine2DComponent::SetStartAndEndDrawContainer(const DrawContainer& dcS, const DrawContainer& dcE) {

	m_StartObj->SetDrawContainerChangeFlag(dcS, true);
	m_EndObj->SetDrawContainerChangeFlag(dcE, true);
}