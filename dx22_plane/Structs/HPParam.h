#pragma once

#include <SimpleMath.h>

struct HPParam
{
    float currentHP = 0.0f;     // 今のHP
    float hpScale = 0.0f;       // 1HPあたりの長さ（ピクセル換算）
    DirectX::XMFLOAT2 padding = DirectX::XMFLOAT2();      // パディング（16バイトアライメント用）
};