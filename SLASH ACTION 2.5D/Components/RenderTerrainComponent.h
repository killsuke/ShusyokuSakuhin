#pragma once
#include "RenderComponent.h"
#include "System/DirectXRender.h"
#include "Mesh/TerrainMesh.h"
#include <array>

class RenderTerrainComponent : public RenderComponent
{
private:
	std::array<ID3D11ShaderResourceView*, 2> m_ShaderResource{ nullptr };
	std::array<ID3D11SamplerState*, 2> m_SamplerState{ nullptr };
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShadowSRV = nullptr;
	TerrainMesh* m_TerrainMesh = nullptr;

public:

	RenderTerrainComponent(GameObject& obj);
	~RenderTerrainComponent();
	void Update() override;
	void SetUVMagnification(const DirectX::XMFLOAT3& magnification) {
		if (m_TerrainMesh != nullptr) {
			m_TerrainMesh->SetUVMagnification(magnification);
		}
	};
};