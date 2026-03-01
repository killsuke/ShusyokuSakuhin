#include "RenderMotionBlurComponent.h"
#include "TransformComponent.h"

using namespace DirectX;

RenderMotionBlurComponent::RenderMotionBlurComponent(GameObject& obj) :RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();

	m_MotionBlurController = m_Object->AddComponent<MotionBlurControllerComponent>();
}

void RenderMotionBlurComponent::Update() {

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

		// モーションブラー用のステート設定
		// 半透明合成を有効にして、シェル同士が重なるよう深度書き込みをOFFに
		DirectXRender::SetDepthEnable(false);

		ID3D11Buffer* bufferDraw = DirectXRender::GetDefaultDrawBuffer();
		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(bufferDraw, 0, NULL, &cb, 0, 0);

		const DirectX::XMMATRIX prevMtx = m_MotionBlurController->GetPrevMatrix();
		const float blurStrength = m_MotionBlurController->GetBlurStrength();
		const int shellCount = m_MotionBlurController->GetShellCount();
		const bool isUse = m_MotionBlurController->GetIsUseBlur();

		MotionBlurBuffer motionBlur;
		motionBlur.matrixPrevWorld = prevMtx;
		motionBlur.BlurParams.x = 0.0f;
		motionBlur.BlurParams.y = (float)shellCount;
		motionBlur.BlurParams.z = blurStrength;
		motionBlur.BlurParams.w = (float)isUse;

		const std::vector<SUBSET> subsets = m_Mesh->GetSubsets();

		const std::vector<MATERIAL> materials = m_Mesh->GetMaterials();

		std::vector<Texture> textures = m_Mesh->GetTextures();

		ID3D11Buffer* bufferMotionBlur = DirectXRender::GetMotionBlurBuffer();
		deviceContext->VSSetConstantBuffers(7, 1, &bufferMotionBlur);
		deviceContext->PSSetConstantBuffers(7, 1, &bufferMotionBlur);

		// シェル分ループして描画
		for (int i = 0; i < shellCount; ++i)
		{
			// シェルインデックスをセット
			motionBlur.BlurParams.x = (float)i;
		
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
		}

		DirectXRender::SetDepthEnable(true);
	}
}