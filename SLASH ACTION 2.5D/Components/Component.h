#pragma once

#include <cstdint>
#include "GameObject/GameObject.h"
#include "Manager/ComponentTypeManager.h"

class GameObject; // 前方宣言

class Component
{
protected:
	uint32_t m_SortNum = 0;	// 更新処理でどれを優先して更新するかソートをする
	GameObject* m_Object = nullptr; // このコンポーネントが所属するGameObjectへのポインタ
	bool m_IsActiveFlag = true; // コンポーネントが有効かどうかのフラグ

	Component() = default;
	Component(const Component&) = delete;			 // コピーコンストラクタ禁止
	Component(Component&&) = delete;				 // ムーブコンストラクタ禁止
	Component& operator=(const Component&) = delete; // コピー代入禁止
	Component& operator=(Component&&) = delete;		 // ムーブ代入禁止

	Component(GameObject& obj) :m_Object(&obj) {};
public:
	virtual ~Component() = default;

	virtual void Update() = 0;
	uint32_t GetSortNum() const { return m_SortNum; };		 // ソート番号を返す
	GameObject* GetGameObject() { return m_Object; }; // このコンポーネントが所属するGameObjectへのポインタを返す
	void SetActiveFlag(const bool flag) { m_IsActiveFlag = flag; }; // コンポーネントの有効フラグをセット
	bool GetActiveFlag() const { return m_IsActiveFlag; } // コンポーネントの有効フラグを取得
};
