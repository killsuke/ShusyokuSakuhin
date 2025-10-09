#include "Render3DColliderOBBComponent.h"
#include "DirectXRender.h"
#include "Collider.h"
#include "GameObjectManager.h"

using namespace DirectX::SimpleMath;

Render3DColliderOBBComponent::Render3DColliderOBBComponent(GameObject& obj) : RenderComponent(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("RENDER_DEBUG"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	m_Texture = std::make_unique<Texture>();
}

void Render3DColliderOBBComponent::Update()
{
#ifdef _DEBUG
	GameObjectManager& gameObjectManager = GameObjectManager::GetInstance();
	auto colliderOBB = p_object->GetComponent<ColliderComponent>();
	auto cameraobj = gameObjectManager.GameObjectFindName("camera");

	if (colliderOBB != nullptr && cameraobj != nullptr) {
		//定数バッファを更新
		ConstBuffer cb;

		cb.matrixWorld = colliderOBB->GetWorldOBBMatrix().Transpose();

		cb.color = Vector4(m_Color);

		DirectXRender& dxRender = DirectXRender::GetInstance();
		auto deviceContext = dxRender.GetDeviceContext();

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
#endif
}