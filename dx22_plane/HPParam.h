#pragma once

#include <SimpleMath.h>

struct HPParam
{
    float currentHP;     // 今のHP
    float hpScale;       // 1HPあたりの長さ（ピクセル換算）
    DirectX::SimpleMath::Vector2 padding;      // パディング（16バイトアライメント用）
};