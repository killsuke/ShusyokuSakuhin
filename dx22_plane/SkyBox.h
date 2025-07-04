#pragma once
#include <d3d.h>
#include <SimpleMath.h>
#include  "Renderer.h"
#include "Shader.h"
#include "GameObject.h"
#include "Camera.h"

class SkyBox : public GameObject
{
private:
   

public:
    SkyBox(Camera* cam);	// コンストラクタ
    ~SkyBox();	// デストラクタ

    void Init();

    void TextureLoadSkyBox();

    void Update();
    void Draw();
    void Uninit();
};