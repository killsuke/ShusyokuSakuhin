#include "Render3D.h"
#include "DirectXRender.h"
#include "Camera.h"
#include "Game.h"
#include "Transform.h"

Render3DComponent::Render3DComponent(GameObject& obj) : RenderComponent(obj) {

}

void Render3DComponent::Update()
{
	auto transform = p_object->GetComponent<TransformComponent>();

	if (transform != nullptr) {
		//定数バッファを更新
		ConstBuffer cb;

		cb.matrixWorld = transform->GetWorldMatrix();

		cb.color = DirectX::XMFLOAT4(m_Color);

		auto deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();
		m_Texture->SetGPU();

		auto camera = Game::GetInstance()->GetObjects<Camera>();

		cb.matrixView = camera[0]->GetViewMtx3D();
		cb.matrixProj = camera[0]->GetProjMtx3D();

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

		deviceContext->DrawIndexed(
			m_IndexBuffer.GetIndexSize(),							// 描画するインデックス数（立方体なんで36）
			0,							// 最初のインデックスバッファの位置
			0);

	}
}