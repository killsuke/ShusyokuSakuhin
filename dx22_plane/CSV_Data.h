#pragma once
#include <DirectXMath.h>
#include <string>

struct CSV_Data
{
	std::string kind = ""; // 読み込んだオブジェクトの名前
	DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(0.0f,0.0f); // 位置
};