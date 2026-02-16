#include "RenderHpComponent.h"
#include "System/DirectXRender.h"
#include "Transform.h"
#include "Structs/HPParam.h"
#include "FighterComponent.h"

using namespace DirectX;

RenderHpComponent::RenderHpComponent(GameObject& obj) : RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
}

void RenderHpComponent::Update()
{
	const TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if(referenceHpObj == nullptr ) {
		return; // 参照するHPオブジェクトが設定されていない場合は何もしない
	}

	FighterComponent* hpObj = referenceHpObj->GetComponent<FighterComponent>();

	if (transform != nullptr && m_Mesh != nullptr && hpObj != nullptr) {

		const int nowHP = hpObj->GetHp();
		if(nowHP <= 0) {
			return; // HPが0以下の場合は描画しない
		}

		//定数バッファを更新
		ConstBuffer cb;

		cb.matrixWorld = XMMatrixTranspose(transform->GetWorldMatrix());

		cb.color = m_Color;

		cb.inverse = (m_Inversion == RightLeft::RIGHT) ? true : false;

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

		OverVertexParam hpParam;

		// HPの量を入れる
		hpParam.currentScale = float(nowHP);

		// HPの値「１」辺りの縦のサイズを入れる
		hpParam.baseScale = hpScale;

		ID3D11Buffer* bufferHp = DirectXRender::GetOverVertexMoveBuffer();
		deviceContext->VSSetConstantBuffers(UINT(EBufferTypes::OVER_VERTEX), 1, &bufferHp);

		deviceContext->UpdateSubresource(bufferHp, 0, nullptr, &hpParam, 0, 0);

		const std::vector<SUBSET> subsets = m_Mesh->GetSubsets();

		const std::vector<MATERIAL> materials = m_Mesh->GetMaterials();

		std::vector<Texture> textures = m_Mesh->GetTextures();

		ID3D11Buffer* bufferMaterial = DirectXRender::GetMaterialBuffer();

		//マテリアル数分ループ 
		for (int i = 0; i < subsets.size(); i++)
		{
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