#include "RenderLineComponent.h"
#include "DirectXRender.h"
#include "Transform.h"
#include "GameObjectManager.h"
#include "RenderBillboard.h"
#include "CircleMesh.h"
#include "LineMesh.h"
#include "input.h"

using namespace DirectX::SimpleMath;

RenderLineComponent::RenderLineComponent(GameObject& obj) : RenderComponent(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	m_Texture = std::make_unique<Texture>();

	m_thickness = 300.0f;

	LineMesh line;
	SetMesh(line);
	// 専用のインプットレイアウトもここで作成予定
	SetShader("LineVS.hlsl", "shader/unlitTexturePS.hlsl","unlitTexture_GS.hlsl");

	p_startObj = GameObjectManager::AddAbsFront("startPoint", "LineObj");
	auto transS = p_startObj->AddComponent<TransformComponent>();
	transS->SetScale({10.0f,10.0f,10.0f});
	transS->SetPosition({ 0.0f,50.0f,0.0f });
	auto rendS = p_startObj->AddComponent<RenderBillboardComponent>();
	CircleMesh cirS;
	rendS->SetMesh(cirS);
	rendS->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	rendS->SetTexture("assets/texture/NoTexture.png");

	p_endObj = GameObjectManager::AddAbsFront("endPoint","LineObj");
	auto transE = p_endObj->AddComponent<TransformComponent>();
	transE->SetScale({ 10.0f,10.0f,10.0f });
	transE->SetPosition({ 0.0f,-50.0f,0.0f });
	auto rendE = p_endObj->AddComponent<RenderBillboardComponent>();
	CircleMesh cirE;
	rendE->SetMesh(cirE);
	rendE->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	rendE->SetTexture("assets/texture/NoTexture.png");
}

void RenderLineComponent::Update()
{
	auto transform = p_object->GetComponent<TransformComponent>();

	if(Input::GetKeyPress(VK_D)){
		p_startObj->GetComponent<TransformComponent>()->AddPosition({10.0f,0.0f,0.0f});
	}
	if (Input::GetKeyPress(VK_A)) {
		p_startObj->GetComponent<TransformComponent>()->AddPosition({ -10.0f,0.0f,0.0f });
	}
	if (Input::GetKeyPress(VK_S)) {
		p_startObj->GetComponent<TransformComponent>()->AddPosition({ 0.0f,-10.0f,0.0f });
	}
	if (Input::GetKeyPress(VK_W)) {
		p_startObj->GetComponent<TransformComponent>()->AddPosition({ 0.0f,10.0f,0.0f });
	}
	if (Input::GetKeyPress(VK_R)) {
		m_thickness += 10.0f;
	}
	if (Input::GetKeyPress(VK_E)) {
		m_thickness -= 10.0f;
	}

	if (transform != nullptr) {
		auto deviceContext = DirectXRender::GetDeviceContext();

		D3D11_MAPPED_SUBRESOURCE mapped = {};
		deviceContext->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

		VERTEX_3D* vtx = reinterpret_cast<VERTEX_3D*>(mapped.pData);

		// ここで全頂点データを更新
		
		vtx[0].position = p_startObj->GetComponent<TransformComponent>()->GetPosition();
		vtx[0].normal = Vector3(0.0f, 1.0f, 0.0f);
		vtx[0].color = m_Color;
		vtx[0].uv = Vector2(0.0f, 0.0f);

		vtx[1].position = p_endObj->GetComponent<TransformComponent>()->GetPosition();
		vtx[1].normal = Vector3(0.0f, 1.0f, 0.0f);
		vtx[1].color = m_Color;
		vtx[1].uv = Vector2(1.0f, 0.0f);

		deviceContext->Unmap(m_VertexBuffer.Get(), 0);

		LineThickness thick;
		thick.thickness = m_thickness;

		deviceContext->UpdateSubresource(DirectXRender::GetLineThicknessBuffer(), 0, NULL, &thick, 0, 0);

		//定数バッファを更新
		/*ConstBuffer cb;

		cb.matrixWorld = transform->GetWorldMatrix().Transpose();

		cb.color = Vector4(m_Color);*/

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();
		m_Texture->SetGPU();

		// 行列をシェーダーに渡す
		//deviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

		deviceContext->DrawIndexed(
			m_IndexBuffer.GetIndexSize(),							// 描画するインデックス数（立方体なんで36）
			0,							// 最初のインデックスバッファの位置
			0);
	}
}

void RenderLineComponent::SetStartAndEndPosition(const DirectX::SimpleMath::Vector3& startPos, const DirectX::SimpleMath::Vector3& endPos) {
	auto transS = p_startObj->GetComponent<TransformComponent>();
	auto transE = p_endObj->GetComponent<TransformComponent>();

	transS->SetPosition(startPos);
	transE->SetPosition(endPos);
}