#pragma once
#include "Component.h"
class FighterComponent : public Component
{
private:
	int m_atk = 0; // 攻撃力
	int m_hp = 0; // ヒットポイント
	int m_maxHp = 0; // 最大ヒットポイント
	int m_totalDamage = 0;	// ダメージ

public:
	FighterComponent(GameObject& obj);
	~FighterComponent() = default;
	
	void Update() override;

	inline void SetAtk(const int atk) { m_atk = atk; } // 攻撃力を設定
	inline void SetHp(const int hp) { m_hp = hp; } // ヒットポイントを設定
	inline void SetMaxHp(const int maxHp) { m_maxHp = maxHp; } // 最大ヒットポイントを設定
	inline void SetDamage(const int damage) { m_totalDamage = damage; };

	inline int GetAtk() const { return m_atk; } // 攻撃力を取得
	inline int GetHp() const { return m_hp; } // ヒットポイントを取得
	inline int GetMaxHp() const { return m_maxHp; } // 最大ヒットポイントを取得
	inline int GetDamage() const { return m_totalDamage; };


	inline void AddAtk(const int atk) { m_atk += atk; } // 攻撃力を加算
	inline void AddHp(const int hp) { m_hp += hp; } // ヒットポイントを加算
	inline void AddDamage(const int damage) { m_totalDamage += damage; };
};

