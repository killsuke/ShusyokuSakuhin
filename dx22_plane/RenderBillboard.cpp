#include "RenderBillboard.h"
#include "DirectXRender.h"
#include "Transform.h"
#include "GameObjectManager.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

RenderBillboardComponent::RenderBillboardComponent(GameObject& obj) : RenderComponent(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	m_Texture = std::make_unique<Texture>();
}

void RenderBillboardComponent::Update()
{
	auto transform = p_object->GetComponent<TransformComponent>();
	auto camera = GameObjectManager::GameObjectFindName("camera");

	if (transform != nullptr && camera != nullptr) {
		//定数バッファを更新
		ConstBuffer cb;

		auto pos = transform->GetPosition();
		auto scale = transform->GetScale();
		auto cameraView3D = camera->GetComponent<Camera>()->GetView3D();
		
		cameraView3D = cameraView3D.Invert();

		cameraView3D._41 = 0.0f; // ビルボードの位置をカメラの位置に合わせる
		cameraView3D._42 = 0.0f;
		cameraView3D._43 = 0.0f;

		Matrix t = Matrix::CreateTranslation(pos.x, pos.y, pos.z);
		Matrix s = Matrix::CreateScale(scale.x, scale.y, scale.z);

		cb.matrixWorld = (s * cameraView3D * t).Transpose();

		cb.color = Vector4(m_Color);

		auto deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();
		m_Texture->SetGPU();

		auto uvs = m_Texture->GetUVSets();

		uvs.x = uvs.x - 1;
		uvs.y = uvs.y - 1;
		uvs.z = 1 / uvs.z;
		uvs.w = 1 / uvs.w;

		cb.matrixTex = m_Texture->MakeUV(uvs.x, uvs.y, uvs.z, uvs.w);

		cb.inverse = m_inversionFlag;

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

		deviceContext->DrawIndexed(
			m_IndexBuffer.GetIndexSize(),							// 描画するインデックス数（立方体なんで36）
			0,							// 最初のインデックスバッファの位置
			0);
	}
}