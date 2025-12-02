#pragma once
#include "Render.h"

class RenderMotionBlurComponent : public RenderComponent
{
private:
	ID3D11Texture2D* m_BlurTexture = nullptr;
	ID3D11Texture2D* m_DepthTexture = nullptr;
	ID3D11RenderTargetView* m_BlurRTV = nullptr;
	ID3D11ShaderResourceView* m_BlurSRV = nullptr;
	ID3D11DepthStencilView* m_BlurDSV = nullptr;

public:
	RenderMotionBlurComponent(GameObject& obj);
	~RenderMotionBlurComponent();

	void Update()override;
};