#include "RenderMotionBlurComponent.h"
#include "Application.h"
#include "DirectXRender.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

RenderMotionBlurComponent::RenderMotionBlurComponent(GameObject& obj) : RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();

	UINT width = (UINT)Application::GetWidth();
	UINT height = (UINT)Application::GetHeight();
	ID3D11Device* device = DirectXRender::GetDevice();

	// まずポストエフェクト用のテクスチャ作成
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&texDesc, nullptr, &m_BlurTexture);

	if (FAILED(hr)) {
		return;
	}

	// レンダーターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(m_BlurTexture, &rtvDesc, &m_BlurRTV);

	if (FAILED(hr)) {
		return;
	}

	// シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.TextureCube.MipLevels = 1;
	srvDesc.TextureCube.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(m_BlurTexture, &srvDesc, &m_BlurSRV);

	if (FAILED(hr)) {
		return;
	}

	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = device->CreateTexture2D(&depthDesc, nullptr, &m_DepthTexture);

	if (FAILED(hr)) {
		return;
	}

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	// DSV作成
	hr = device->CreateDepthStencilView(m_DepthTexture, nullptr, &m_BlurDSV);

	if (FAILED(hr)) {
		return;
	}
}

RenderMotionBlurComponent::~RenderMotionBlurComponent() {

	SAFE_RELEASE(m_BlurTexture);
	SAFE_RELEASE(m_DepthTexture);
	SAFE_RELEASE(m_BlurRTV);
	SAFE_RELEASE(m_BlurSRV);
	SAFE_RELEASE(m_BlurDSV);
}

void RenderMotionBlurComponent::Update() {

	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr && m_Mesh != nullptr) {

		ID3D11RenderTargetView* oldRTV = nullptr;
		ID3D11DepthStencilView* oldDSV = nullptr;
		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();
		const XMFLOAT4 clearColor = DirectXRender::GetClearColor();

		deviceContext->OMGetRenderTargets(1, &oldRTV,&oldDSV);

		deviceContext->OMSetRenderTargets(1, &m_BlurRTV, m_BlurDSV);

		//定数バッファを更新
		ConstBuffer cb;

		cb.matrixWorld = XMMatrixTranspose(transform->GetWorldMatrix());

		cb.color = Vector4(m_Color);

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();
		//m_Texture->SetGPU();

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

		const std::vector<SUBSET> subsets = m_Mesh->GetSubsets();

		const std::vector<MATERIAL> materials = m_Mesh->GetMaterials();

		std::vector<Texture> textures = m_Mesh->GetTextures();

		//マテリアル数分ループ 
		for (int i = 0; i < subsets.size(); i++)
		{
			// ここ使う
			MATERIAL material = materials[subsets[i].MaterialIdx];

			deviceContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &material, 0, 0);

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