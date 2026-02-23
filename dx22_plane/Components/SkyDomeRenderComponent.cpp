#include "SkyDomeRenderComponent.h"
#include "System/DirectXRender.h"
#include "TransformComponent.h"
#include "Manager/GameObjectManager.h"
#include <DDSTextureLoader.h>

using namespace DirectX;

SkyDomeRenderComponent::SkyDomeRenderComponent(GameObject& obj) : RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	//m_Texture = std::make_unique<Texture>();
}

void SkyDomeRenderComponent::Update() {

	auto transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr) {

		transform->AddRotation({ 0.0f, 0.05f, 0.0f });

		//定数バッファを更新
		ConstBuffer cb;

		cb.matrixWorld = XMMatrixTranspose(transform->GetWorldMatrix());

		cb.color = m_Color;

		auto deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();
	//	m_Texture->SetGPU();

		deviceContext->PSSetShaderResources(0, 1, &m_pTextureView);

		ID3D11Buffer* bufferDraw = DirectXRender::GetDefaultDrawBuffer();
		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(bufferDraw, 0, NULL, &cb, 0, 0);

		deviceContext->DrawIndexed(
			m_IndexBuffer.GetIndexSize(),							// 描画するインデックス数（立方体なんで36）
			0,							// 最初のインデックスバッファの位置
			0);
	}
}

void SkyDomeRenderComponent::TextureLoadSkyDome(const wchar_t* tex) {
	auto device = DirectXRender::GetDevice();

	auto hr = DirectX::CreateDDSTextureFromFile(device, tex, nullptr, &m_pTextureView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "DDSファイルの読み込みに失敗しました。", "エラー", MB_OK | MB_ICONERROR);
	}
}