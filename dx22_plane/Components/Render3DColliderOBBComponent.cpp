#include "Render3DColliderOBBComponent.h"
#include "System/DirectXRender.h"
#include "Collider.h"
#include "Manager/GameObjectManager.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

Render3DColliderOBBComponent::Render3DColliderOBBComponent(GameObject& obj) : RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER_DEBUG"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	//m_Texture = std::make_unique<Texture>();
}

void Render3DColliderOBBComponent::Update()
{
#ifdef _DEBUG
	auto colliderOBB = m_Object->GetComponent<ColliderComponent>();

	if (colliderOBB != nullptr && m_Mesh != nullptr) {
		//定数バッファを更新
		ConstBuffer cb;

		cb.matrixWorld = XMMatrixTranspose(colliderOBB->GetWorldOBBMatrix());

		cb.color = m_Color;

		auto deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();
		//m_Texture->SetGPU();

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

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
#endif
}