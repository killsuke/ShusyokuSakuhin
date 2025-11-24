#include "RenderLineComponent.h"
#include "DirectXRender.h"
#include "Transform.h"
#include "GameObjectManager.h"
#include "RenderBillboard.h"
#include "Mesh/CircleMesh.h"
#include "Mesh/LineMesh.h"
#include "input.h"
#include "FollowPositionComponent.h"

using namespace DirectX::SimpleMath;

RenderLineComponent::RenderLineComponent(GameObject& obj) : RenderComponent(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	//m_Texture = std::make_unique<Texture>();

	CreateMesh<LineMesh>();
	// 専用のインプットレイアウトもここで作成予定
	SetShader("shader/LineVS.hlsl", "shader/unlitTexturePS.hlsl","unlitTexture_GS.hlsl");

	p_startObj = GameObjectManager::AddAbsFront("startPoint", "LineObj");
	auto transS = p_startObj->AddComponent<TransformComponent>();
	transS->SetScale({10.0f,10.0f,10.0f});
	transS->SetPosition({ 0.0f,50.0f,0.0f });
	auto rendS = p_startObj->AddComponent<RenderBillboardComponent>();
	
	rendS->CreateMesh<CircleMesh>();
	rendS->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	rendS->ChangeTexture("assets/texture/NoTexture.png");

	p_endObj = GameObjectManager::AddAbsFront("endPoint","LineObj");
	auto transE = p_endObj->AddComponent<TransformComponent>();
	transE->SetScale({ 10.0f,10.0f,10.0f });
	transE->SetPosition({ 0.0f,-50.0f,0.0f });
	auto rendE = p_endObj->AddComponent<RenderBillboardComponent>();
	
	rendE->CreateMesh<CircleMesh>();
	rendE->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	rendE->ChangeTexture("assets/texture/NoTexture.png");
}

void RenderLineComponent::Update()
{
	auto transform = m_Object->GetComponent<TransformComponent>();	

	if (transform != nullptr && m_Mesh != nullptr) {
		auto deviceContext = DirectXRender::GetDeviceContext();

		D3D11_MAPPED_SUBRESOURCE mapped = {};
		deviceContext->Map(m_VertexBuffer.GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

		VERTEX_3D* vtx = reinterpret_cast<VERTEX_3D*>(mapped.pData);

		// ここで全頂点データを更新
		auto pos_S = p_startObj->GetComponent<TransformComponent>()->GetPosition();
		auto pos_E = p_endObj->GetComponent<TransformComponent>()->GetPosition();
		auto start_Q = p_startObj->GetComponent<TransformComponent>()->GetQuaternion();

		float length = Vector3::Distance(pos_S, pos_E);

		Vector3 rotation = transform->GetRotation();

		Vector3 dir = Vector3::Transform(Vector3(1.0f, 0.0f, 0.0f), start_Q);


		pos_S.z -= 4.0f;
		vtx[0].position = pos_S;
		vtx[0].normal = Vector3(0.0f, 1.0f, 0.0f);
		vtx[0].color = m_Color;
		vtx[0].uv = Vector2(0.0f, 0.0f);

		pos_E.z -= 4.0f;
		vtx[1].position = pos_E;
		vtx[1].normal = Vector3(0.0f, 1.0f, 0.0f);
		vtx[1].color = m_Color;
		vtx[1].uv = Vector2(1.0f, 0.0f);

		deviceContext->Unmap(m_VertexBuffer.GetBuffer(), 0);

		LineThickness thick;
		thick.thickness = m_thickness;

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
		for (int i = 0; i < subsets.size(); i++)
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

void RenderLineComponent::SetStartPosition(const DirectX::SimpleMath::Vector3& startPos) {
	auto transS = p_startObj->GetComponent<TransformComponent>();

	transS->SetPosition(startPos);
}

void RenderLineComponent::SetEndPosition(const DirectX::SimpleMath::Vector3& endPos) {
	auto transE = p_endObj->GetComponent<TransformComponent>();

	transE->SetPosition(endPos);
}

void RenderLineComponent::SetStartAndEndPosition(const DirectX::SimpleMath::Vector3& startPos, const DirectX::SimpleMath::Vector3& endPos) {
	auto transS = p_startObj->GetComponent<TransformComponent>();
	auto transE = p_endObj->GetComponent<TransformComponent>();

	transS->SetPosition(startPos);
	transE->SetPosition(endPos);
}

void RenderLineComponent::SetStartAndEndFollowObject(GameObject* objS,GameObject* objE) {

	auto followS = p_startObj->AddComponent<FollowPositionComponent>();
	followS->SetFollowObject(objS);
	auto followE = p_endObj->AddComponent<FollowPositionComponent>();
	followE->SetFollowObject(objE);
}