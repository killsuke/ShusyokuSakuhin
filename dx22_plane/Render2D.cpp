#include "Render2D.h"
#include "DirectXRender.h"
#include "Transform.h"
#include "GameObjectManager.h"

using namespace DirectX::SimpleMath;

Render2DComponent::Render2DComponent(GameObject& obj) : RenderComponent(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	m_Texture = std::make_unique<Texture>();
}

void Render2DComponent::Update()
{
	auto transform = p_object->GetComponent<TransformComponent>();

	if (transform != nullptr) {
		//定数バッファを更新
		ConstBuffer cb;

		cb.matrixWorld = transform->GetWorldMatrix().Transpose();

		cb.color = Vector4(m_Color);

		auto deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();
		m_Texture->SetGPU();

	//	m_Texture->SetUV(1.0f, 1.0f, 2.0f, 2.0f);

		auto uvs = m_Texture->GetUVSets();

		uvs.x = uvs.x - 1;
		uvs.y = uvs.y - 1;
		uvs.z = 1 / uvs.z;
		uvs.w = 1 / uvs.w;

		cb.matrixTex = m_Texture->MakeUV(uvs.x,uvs.y, uvs.z, uvs.w);

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

		deviceContext->DrawIndexed(
			m_IndexBuffer.GetIndexSize(),							// 描画するインデックス数（立方体なんで36）
			0,							// 最初のインデックスバッファの位置
			0);
	}
}