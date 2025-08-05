#pragma once

#include <cstdint>
#include "GameObject.h"

enum ComponentType {
	// コンポーネントの種類
	BASECOMPONENT = 0, // ベースコンポーネント（抽象クラス）
	TEST_MOVE,
	JUMP,
	TRANSFORM,
	SPRING,
	RIGIDBODY,
	COLLIDER,
	TEST_EXTRUSION,
	CAMERA_POINT,
	CAMERA_MOVE,
	CAMERA,
	BONE,
	ATTACK, // 攻撃コンポーネント
	ENEMY_DAMAGE, // 敵のダメージコンポーネント
	FIGHTER,
	RENDER,
	RENDER_ONE_SKIN_ANIMATION,
	COLLIDER_DEBUG,

	Max			// 最大値（コンポーネントの種類数）
};

class GameObject; // 前方宣言

class Component
{
protected:
	//ComponentType m_type = BASECOMPONENT; // コンポーネントの種類
	uint16_t m_sortNum = 0;	// 更新処理でどれを優先して更新するかソートをする
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
	inline uint16_t GetSortNum() const { return m_sortNum; };		 // ソート番号を返す
	inline GameObject* GetGameObject() { return p_object; }; // このコンポーネントが所属するGameObjectへのポインタを返す
	inline void SetActiveFlag(const bool flag) { m_activeFlag = flag; }; // コンポーネントの有効フラグをセット
	inline bool GetActiveFlag() const { return m_activeFlag; } // コンポーネントの有効フラグを取得
};
