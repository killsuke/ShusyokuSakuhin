#include "RenderMotionBlurCircularComponent.h"
#include "System/DirectXRender.h"
#include "Manager/GameObjectManager.h"

using namespace DirectX;

RenderMotionBlurCircularComponent::RenderMotionBlurCircularComponent(GameObject& obj) :RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();

	// 子オブジェクトを作成
	// 先端側
	m_ChildTip = GameObjectManager::AddChild("tip", "Children");
	TransformComponent* tipTransform = m_ChildTip->AddComponent<TransformComponent>();
	tipTransform->SetLocalPosition({ 0.0f,1.0f,0.0f });

	// 根元側
	m_ChildRoot = GameObjectManager::AddChild("root", "Children");
	TransformComponent* rootTransform = m_ChildRoot->AddComponent<TransformComponent>();
	rootTransform->SetLocalPosition({ 0.0f,-1.0f,0.0f });

	// 子オブジェクトをセット
	m_Object->SetChild(m_ChildTip);
	m_Object->SetChild(m_ChildRoot);
}

void RenderMotionBlurCircularComponent::Update() {

	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr && m_Mesh != nullptr) {

		//定数バッファを更新
		ConstBuffer cb;

		const XMMATRIX transMtx = transform->GetWorldMatrix();
		const XMMATRIX renderMtx = MakeRenderMatrix(transMtx);

		cb.matrixWorld = XMMatrixTranspose(renderMtx);

		cb.color = m_Color;

		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();

		// モーションブラー用のステート設定
		// 半透明合成を有効にして、シェル同士が重なるよう深度書き込みをOFFに
//		DirectXRender::SetDepthEnable(false);

		ID3D11Buffer* bufferDraw = DirectXRender::GetDefaultDrawBuffer();
		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(bufferDraw, 0, NULL, &cb, 0, 0);

		MotionBlurCircularBuffer motionBlur;

		TransformComponent* tipTransform = m_ChildTip->GetComponent<TransformComponent>();
		TransformComponent* rootTransform = m_ChildRoot->GetComponent<TransformComponent>();

		TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();

		motionBlur.centerPos = XMFLOAT3();
		const XMFLOAT3 tipPos = myTrans->GetPosition();
		const XMFLOAT3 delta = motionBlur.centerPos - tipPos;
		const XMVECTOR deltaVec = XMLoadFloat3(&delta);
		motionBlur.length = XMVectorGetX(XMVector3Length(deltaVec));

		motionBlur.currRot = transform->GetQuaternion();
		motionBlur.prevRot = m_PrevQuat;
		motionBlur.shellCount = 10;

		//DirectXRender::SetCullingState(ECullingState::CULLING_NONE);

		const std::vector<SUBSET> subsets = m_Mesh->GetSubsets();

		const std::vector<MATERIAL> materials = m_Mesh->GetMaterials();

		std::vector<Texture> textures = m_Mesh->GetTextures();

		ID3D11Buffer* bufferMotionBlur = DirectXRender::GetMotionBlurCircularBuffer();

		deviceContext->VSSetConstantBuffers(7, 1, &bufferMotionBlur);
		deviceContext->PSSetConstantBuffers(7, 1, &bufferMotionBlur);
		deviceContext->GSSetConstantBuffers(7, 1, &bufferMotionBlur);

		deviceContext->UpdateSubresource(bufferMotionBlur, 0, NULL, &motionBlur, 0, 0);

		ID3D11Buffer* bufferMaterial = DirectXRender::GetMaterialBuffer();

		//マテリアル数分ループ 
		for (int i = 0; i < subsets.size(); ++i)
		{
			// ここ使う
			MATERIAL material = materials[subsets[i].MaterialIdx];

			deviceContext->UpdateSubresource(bufferMaterial, 0, NULL, &material, 0, 0);

			if (materials[subsets[i].MaterialIdx].TextureEnable == TRUE) {

				textures[subsets[i].MaterialIdx].SetGPU();
			}

			deviceContext->DrawIndexed(
				subsets[i].IndexNum,		// 描画するインデックス数
				subsets[i].IndexBase,		// 最初のインデックスバッファの位置	
				subsets[i].VertexBase);	// 頂点バッファの最初から使用
		}

		//		DirectXRender::SetDepthEnable(true);
		m_PrevQuat = transform->GetQuaternion();
	}
}