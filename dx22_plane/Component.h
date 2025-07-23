#pragma once

#include <cstdint>
#include "GameObject.h"

class GameObject; // 前方宣言

class Component
{
protected:
	uint16_t m_sortNum = 0;	// 更新処理でどれを優先して更新するかソートをする
	GameObject* p_object = nullptr; // このコンポーネントが所属するGameObjectへのポインタ

	Component() = default;

public:
	virtual ~Component() = default;

	virtual void Update() = 0;
	inline uint16_t GetSortNum() { return m_sortNum; } // ソート番号を返す（GameObject側でソート処理を作っておく）
};
