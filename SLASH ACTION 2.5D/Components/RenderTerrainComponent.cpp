#include "RenderTerrainComponent.h"
#include "TransformComponent.h"
#include "Manager/TextureManager.h"
#include "Manager/GameObjectManager.h"
#include "Manager/ShadowManager.h"

using namespace DirectX;

RenderTerrainComponent::RenderTerrainComponent(GameObject& obj) : RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();

	m_ShadowSRV = TextureManager::GetTexture("bullet.png");
	m_ShaderResource[1] = m_ShadowSRV.Get();

	m_TerrainMesh = CreateMesh<TerrainMesh>();

	ID3D11Device* device = DirectXRender::GetDevice();

	HRESULT hr = S_OK;

	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&samplerDesc, &m_SamplerState[1]);
	if (FAILED(hr)) {
		m_SamplerState[1] = nullptr;
		MessageBoxW(nullptr, L"CreateSamplerState error", L"Error", MB_OK);
		return;
	}
}

RenderTerrainComponent::~RenderTerrainComponent() {
	SAFE_RELEASE(m_SamplerState[1]);
}

void RenderTerrainComponent::Update() {

	const TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr && m_TerrainMesh != nullptr && m_ShaderResource[1] != nullptr && m_SamplerState[1] != nullptr) {
		//定数バッファを更新
		ConstBuffer cb;

		const XMMATRIX transMtx = transform->GetWorldMatrix();
		const XMMATRIX renderMtx = MakeRenderMatrix(transMtx);

		cb.matrixWorld = XMMatrixTranspose(renderMtx);

		cb.color = m_Color;

		cb.inverse = (m_Inversion == RightLeft::RIGHT) ? true : false;

		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

		// ここで頂点バッファを書き換える
		const XMFLOAT3 pos = transform->GetPosition();
		const XMFLOAT3 scale = transform->GetScale();
		
		m_TerrainMesh->ChangeUV(scale);
		m_VertexBuffer.Modify(m_TerrainMesh->GetVertices());

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		PrimitiveTypeUpdate(deviceContext);
		m_Shader->SetGPU(deviceContext);
		m_VertexBuffer.SetGPU(deviceContext);
		m_IndexBuffer.SetGPU(deviceContext);

		ID3D11Buffer* bufferDraw = DirectXRender::GetDefaultDrawBuffer();

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(bufferDraw, 0, NULL, &cb, 0, 0);

		std::vector<SUBSET> subsets = m_TerrainMesh->GetSubsets();

		std::vector<MATERIAL> materials = m_TerrainMesh->GetMaterials();

		std::vector<Texture> textures = m_TerrainMesh->GetTextures();

		ID3D11Buffer* bufferMaterial = DirectXRender::GetMaterialBuffer();

		ID3D11Buffer* shadowBuffer = DirectXRender::GetShadowBuffer();

		ID3D11SamplerState* currentSampler = DirectXRender::GetSampler();

		if (currentSampler == nullptr) {
			return;
		}

		m_SamplerState[0] = currentSampler;

		// 現在のサンプラーステートを保存してから、描画に必要なサンプラーステートに切り替える
		const SamplerState currentSamplerState = DirectXRender::GetCurrentSamplerState();

		ID3D11SamplerState* newSampler = DirectXRender::SetSamplerState(SamplerState::WRAP);

		std::array<ID3D11SamplerState*, 2> previousSamplers{};

		previousSamplers[0] = newSampler;
		previousSamplers[1] = m_SamplerState[1];

		deviceContext->PSSetSamplers(0, 2, previousSamplers.data());

		//マテリアル数分ループ 
		for (size_t i = 0; i < subsets.size(); ++i)
		{
			const MATERIAL material = materials[subsets[i].MaterialIdx];

			deviceContext->UpdateSubresource(bufferMaterial, 0, NULL, &material, 0, 0);

			if (materials[subsets[i].MaterialIdx].TextureEnable == TRUE) {

				//				textures[subsets[i].MaterialIdx].SetGPU(deviceContext);

				ID3D11ShaderResourceView* tex = textures[subsets[i].MaterialIdx].GetShaderResourceView();
				m_ShaderResource[0] = tex;

				deviceContext->PSSetShaderResources(0, 2, m_ShaderResource.data());
			}

			deviceContext->DrawIndexed(
				subsets[i].IndexNum,		// 描画するインデックス数
				subsets[i].IndexBase,		// 最初のインデックスバッファの位置	
				subsets[i].VertexBase);	// 頂点バッファの最初から使用
		}

		DirectXRender::SetSamplerState(currentSamplerState);
	}
}