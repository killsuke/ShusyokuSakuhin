#pragma once
#include "Render.h"
#include <d3d11.h>

class SkyDomeRenderComponent : public RenderComponent
{
private:
	ID3D11ShaderResourceView* m_pTextureView = nullptr;

public:
	SkyDomeRenderComponent(GameObject& obj);
	~SkyDomeRenderComponent() = default;

	void Update() override;

	void TextureLoadSkyDome(const wchar_t* tex);

};
