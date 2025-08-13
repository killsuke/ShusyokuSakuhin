#pragma once
#include <SimpleMath.h>
#include <string>

struct CSV_Data
{
	std::string kind = ""; // 読み込んだオブジェクトの名前
	DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero; // 位置
};