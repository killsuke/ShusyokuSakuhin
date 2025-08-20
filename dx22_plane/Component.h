#pragma once

#include <cstdint>
#include "GameObject.h"
#include "ComponentTypeManager.h"

class GameObject; // 前方宣言

class Component
{
protected:
	uint32_t m_sortNum = 0;	// 更新処理でどれを優先して更新するかソートをする
	GameObject* p_object = nullptr; // このコンポーネントが所属するGameObjectへのポインタ
	bool m_activeFlag = true; // コンポーネントが有効かどうかのフラグ

	Component() = default;
	Component(const Component&) = delete;			 // コピーコンストラクタ禁止
	Component(Component&&) = delete;				 // ムーブコンストラクタ禁止
	Component& operator=(const Component&) = delete; // コピー代入禁止
	Component& operator=(Component&&) = delete;		 // ムーブ代入禁止

	Component(GameObject& obj) :p_object(&obj) {};
public:
	virtual ~Component() = default;

	virtual void Update() = 0;
	inline uint32_t GetSortNum() const { return m_sortNum; };		 // ソート番号を返す
	inline GameObject* GetGameObject() { return p_object; }; // このコンポーネントが所属するGameObjectへのポインタを返す
	inline void SetActiveFlag(const bool flag) { m_activeFlag = flag; }; // コンポーネントの有効フラグをセット
	inline bool GetActiveFlag() const { return m_activeFlag; } // コンポーネントの有効フラグを取得
};
