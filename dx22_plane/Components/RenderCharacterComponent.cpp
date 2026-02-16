#include "RenderCharacterComponent.h"
#include "Transform.h"
#include "System/DirectXRender.h"
#include "Structs/HPParam.h"

RenderCharacterComponent::RenderCharacterComponent(GameObject& obj) : RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
}

void RenderCharacterComponent::Update()
{
	const TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr && m_Mesh != nullptr) {
		//定数バッファを更新
		ConstBuffer cb;

		cb.matrixWorld = XMMatrixTranspose(transform->GetWorldMatrix());

		cb.color = m_Color;

		cb.inverse = (m_Inversion == RightLeft::RIGHT) ? true : false;

		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		PrimitiveTypeUpdate(deviceContext);
		m_Shader->SetGPU(deviceContext);
		m_VertexBuffer.SetGPU(deviceContext);
		m_IndexBuffer.SetGPU(deviceContext);

		ID3D11Buffer* bufferDraw = DirectXRender::GetDefaultDrawBuffer();

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(bufferDraw, 0, NULL, &cb, 0, 0);

		std::vector<SUBSET> subsets = m_Mesh->GetSubsets();

		std::vector<MATERIAL> materials = m_Mesh->GetMaterials();

		std::vector<Texture> textures = m_Mesh->GetTextures();

		ID3D11Buffer* bufferMaterial = DirectXRender::GetMaterialBuffer();

		OverVertexParam hpParam;

		hpParam.currentScale = m_CurrentScale;

		hpParam.baseScale = m_BaseScale;

		ID3D11Buffer* bufferHp = DirectXRender::GetOverVertexMoveBuffer();
		deviceContext->VSSetConstantBuffers(UINT(EBufferTypes::OVER_VERTEX), 1, &bufferHp);

		deviceContext->UpdateSubresource(bufferHp, 0, nullptr, &hpParam, 0, 0);

		//マテリアル数分ループ 
		for (size_t i = 0; i < subsets.size(); ++i)
		{
			// ここ使う
			const MATERIAL material = materials[subsets[i].MaterialIdx];

			deviceContext->UpdateSubresource(bufferMaterial, 0, NULL, &material, 0, 0);

			if (materials[subsets[i].MaterialIdx].TextureEnable == TRUE) {

				textures[subsets[i].MaterialIdx].SetGPU(deviceContext);
			}

			deviceContext->DrawIndexed(
				subsets[i].IndexNum,		// 描画するインデックス数
				subsets[i].IndexBase,		// 最初のインデックスバッファの位置	
				subsets[i].VertexBase);	// 頂点バッファの最初から使用
		}
	}
}