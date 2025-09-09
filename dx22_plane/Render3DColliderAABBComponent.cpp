#include "Render3DColliderAABBComponent.h"
#include "DirectXRender.h"
#include "Collider.h"
#include "GameObjectManager.h"

using namespace DirectX::SimpleMath;

Render3DColliderAABBComponent::Render3DColliderAABBComponent(GameObject& obj) : RenderComponent(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("COLLIDER_DEBUG_RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	m_Texture = std::make_unique<Texture>();
}

void Render3DColliderAABBComponent::Update()
{
	auto colliderAABB = p_object->GetComponent<ColliderComponent>();
	auto cameraobj = GameObjectManager::GameObjectFindName("camera");

	if (colliderAABB != nullptr && cameraobj != nullptr) {
		//定数バッファを更新
		ConstBuffer cb;

		cb.matrixWorld = colliderAABB->GetWorldAABBMatrix().Transpose();

		cb.color = Vector4(m_Color);

		auto deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();
		m_Texture->SetGPU();

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

		deviceContext->DrawIndexed(
			m_IndexBuffer.GetIndexSize(),							// 描画するインデックス数（立方体なんで36）
			0,							// 最初のインデックスバッファの位置
			0);
	}
}