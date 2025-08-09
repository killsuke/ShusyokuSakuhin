#include "RenderOneSkinAnimation.h"
#include "DirectXRender.h"
#include "BoneData.h"
#include "Transform.h"
#include "GameObjectManager.h"

RenderOneSkinAnimation::RenderOneSkinAnimation(GameObject& obj) : RenderComponent(obj) {
	m_sortNum = RENDER_ONE_SKIN_ANIMATION;
	m_Shader = std::make_unique<Shader>();
	m_Texture = std::make_unique<Texture>();
}

void RenderOneSkinAnimation::Update()
{
	auto transform = p_object->GetComponent<TransformComponent>();

	if (transform != nullptr) {
		//定数バッファを更新
		CBBoneMatrix cb;

		cb.matrixWorld = transform->GetWorldMatrix().Transpose();

		//cb.color = DirectX::XMFLOAT4(m_Color);

		auto deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_AnimationVertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();
		m_Texture->SetGPU();


		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(g_pBoneConstantBuffer, 0, NULL, &cb, 0, 0);

		deviceContext->DrawIndexed(
			m_IndexBuffer.GetIndexSize(),							// 描画するインデックス数（立方体なんで36）
			0,							// 最初のインデックスバッファの位置
			0);

	}
}