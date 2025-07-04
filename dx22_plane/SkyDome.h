#pragma once
#include "GameObject.h"
#include <d3d.h>
#include <SimpleMath.h>
#include  "Renderer.h"
#include "Shader.h"

class SkyDome : public GameObject
{
private:
    int m_IndexCount = 0;

public:
    SkyDome(Camera* cam);	// コンストラクタ
    ~SkyDome();	// デストラクタ

    void Init();

    void TextureLoadSkyBox();

    void Update();
    void Draw();
    void Uninit();
};
