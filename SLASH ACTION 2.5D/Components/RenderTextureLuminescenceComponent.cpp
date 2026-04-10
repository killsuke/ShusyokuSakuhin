#include "RenderTextureLuminescenceComponent.h"
#include "TransformComponent.h"
#include "Manager/GameObjectManager.h"
#include "CameraComponent.h"

using namespace DirectX;

RenderTextureLuminescenceComponent::RenderTextureLuminescenceComponent(GameObject& obj) : RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	m_GlowShader = std::make_unique<Shader>();

	m_GlowShader->Create("unlitTextureVS.hlsl", "LuminescenceTexturePS.hlsl");
}

void RenderTextureLuminescenceComponent::Update()
{
	const TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr && m_Mesh != nullptr) {

		//定数バッファを更新
		ConstBuffer cb;

		const XMMATRIX transMtx = transform->GetWorldMatrix();
		const XMMATRIX renderMtx = MakeRenderMatrix(transMtx);

		cb.color = m_Color;

		cb.inverse = (m_Inversion == RightLeft::RIGHT) ? true : false;

		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則

		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();

		Texture texture = GetTexture();
		XMFLOAT4 uvs = texture.GetUVSets();

		uvs.x = uvs.x - 1;
		uvs.y = uvs.y - 1;
		uvs.z = 1 / uvs.z;
		uvs.w = 1 / uvs.w;

		cb.matrixTex = texture.MakeUV(uvs.x, uvs.y, uvs.z, uvs.w);

		const std::vector<SUBSET> subsets = m_Mesh->GetSubsets();

		std::vector<MATERIAL> materials = m_Mesh->GetMaterials();

		std::vector<Texture> textures = m_Mesh->GetTextures();

		ID3D11Buffer* bufferMaterial = DirectXRender::GetMaterialBuffer();

		ID3D11Buffer* bufferDefault = DirectXRender::GetDefaultDrawBuffer();

		// 通常描画 ========================================================
		{
			m_Shader->SetGPU();

			cb.matrixWorld = XMMatrixTranspose(renderMtx);

			deviceContext->VSSetConstantBuffers(UINT(EBufferTypes::DEFAULT_DRAW), 1, &bufferDefault);
			deviceContext->PSSetConstantBuffers(UINT(EBufferTypes::DEFAULT_DRAW), 1, &bufferDefault);

			deviceContext->UpdateSubresource(bufferDefault, 0, NULL, &cb, 0, 0);

			//マテリアル数分ループ 
			for (unsigned int i = 0; i < subsets.size(); i++)
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
		// =================================================================


		// グロー用の描画 ==================================================

		{
			XMMATRIX glowMtx = XMMatrixScaling(m_Expand, m_Expand, 1.0f) * transMtx;

			glowMtx = MakeRenderMatrix(glowMtx);

			cb.matrixWorld = XMMatrixTranspose(glowMtx);

			//deviceContext->UpdateSubresource(bufferDefault, 0, NULL, &cb, 0, 0);

			const EBlendState currentBlendState = DirectXRender::GetCurrentBlendState();
			DirectXRender::SetBlendState(EBlendState::BS_ADDITIVE);
			const bool prevDepthEnable = DirectXRender::GetIsDepthEnable();
			DirectXRender::SetDepthEnable(false);

			m_GlowShader->SetGPU();

			const XMFLOAT3 angles = transform->GetRotation();

			GlowBuffer gb;
			gb.glowColor = m_GlowColor;
			gb.ellipseScale = m_EllipseScale;
			gb.glowPower = m_GlowPower;
			gb.glowRadius = m_GlowRadius;
			gb.angleRadian = XMConvertToRadians(angles.z);
			gb.expand = m_Expand;

			ID3D11Buffer* bufferGlow = DirectXRender::GetGlowBuffer();

			deviceContext->VSSetConstantBuffers(UINT(EBufferTypes::GLOW), 1, &bufferGlow);
			deviceContext->PSSetConstantBuffers(UINT(EBufferTypes::GLOW), 1, &bufferGlow);

			deviceContext->UpdateSubresource(bufferGlow, 0, NULL, &gb, 0, 0);

			//マテリアル数分ループ 
			for (unsigned int i = 0; i < subsets.size(); i++)
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

			DirectXRender::SetDepthEnable(prevDepthEnable);
			DirectXRender::SetBlendState(currentBlendState);
		}
		// =================================================================	

		deviceContext->VSSetConstantBuffers(UINT(EBufferTypes::DEFAULT_DRAW), 1, &bufferDefault);
		deviceContext->PSSetConstantBuffers(UINT(EBufferTypes::DEFAULT_DRAW), 1, &bufferDefault);

	}
}
