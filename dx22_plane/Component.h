#include <cstdint>

#pragma once
class Component
{
protected:
	uint16_t sortNum = 0;	// 更新処理でどれを優先して更新するかソートをする
	
	Component() = default;
	virtual ~Component() = default;

public:
	virtual void Update() = 0;
};

