#pragma once

#include <SimpleMath.h>

struct OverVertexParam
{
    float currentScale = 0.0f;     // 今のHP
    float baseScale = 0.0f;       // 1HPあたりの長さ（ピクセル換算）
    DirectX::XMFLOAT2 padding = DirectX::XMFLOAT2();      // パディング（16バイトアライメント用）
};