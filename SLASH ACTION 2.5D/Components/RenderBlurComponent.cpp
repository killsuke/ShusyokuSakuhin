#include "RenderBlurComponent.h"
#include "TransformComponent.h"

using namespace DirectX;

RenderBlurComponent::RenderBlurComponent(GameObject& obj) :RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定

	m_Shader = std::make_unique<Shader>();
	//m_Texture = std::make_unique<Texture>();
}

void RenderBlurComponent::Update() {

	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr && m_Mesh != nullptr) {

		//定数バッファを更新
		ConstBuffer cb;

		cb.matrixWorld = XMMatrixTranspose(transform->GetWorldMatrix());

		cb.color = m_Color;

		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();

		ID3D11Buffer* bufferDraw = DirectXRender::GetDefaultDrawBuffer();

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(bufferDraw, 0, NULL, &cb, 0, 0);

		BlurBuffer blurSize;
		blurSize.textureSize = m_BlurSize;

		ID3D11Buffer* bufferBlur = DirectXRender::GetBlurBuffer();
		deviceContext->UpdateSubresource(bufferBlur, 0, NULL, &blurSize, 0, 0);

		const std::vector<SUBSET> subsets = m_Mesh->GetSubsets();

		const std::vector<MATERIAL> materials = m_Mesh->GetMaterials();

		std::vector<Texture> textures = m_Mesh->GetTextures();

		ID3D11Buffer* bufferMaterial = DirectXRender::GetMaterialBuffer();

		//マテリアル数分ループ 
		for (int i = 0; i < subsets.size(); i++)
		{
			// ここ使う
			const MATERIAL material = materials[subsets[i].MaterialIdx];

			deviceContext->UpdateSubresource(bufferMaterial, 0, NULL, &material, 0, 0);
			
			if (materials[subsets[i].MaterialIdx].TextureEnable == TRUE) {

				textures[subsets[i].MaterialIdx].SetGPU();
			}

			deviceContext->DrawIndexed(
				subsets[i].IndexNum,		// 描画するインデックス数
				subsets[i].IndexBase,		// 最初のインデックスバッファの位置	
				subsets[i].VertexBase);	// 頂点バッファの最初から使用
		}
	}
}