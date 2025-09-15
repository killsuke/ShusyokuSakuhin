#pragma once
#include "Component.h"
class FighterComponent : public Component
{
private:
	int m_atk = 0; // 攻撃力
	int m_hp = 0; // ヒットポイント
	int m_maxHp = 0; // 最大ヒットポイント
	int m_totalDamage = 0;	// ダメージ
	const float m_deltaTime = 0.016f; // 仮のデルタタイム
	float m_recordTime = 0.0f; // 経過時間
	bool m_useInvincible = false; // 無敵を使うかどうか
	bool m_invincibleFlag = false; // 無敵フラグ

public:
	FighterComponent(GameObject& obj);
	~FighterComponent() = default;
	
	void Update() override;

	inline void SetAtk(const int atk) { m_atk = atk; } // 攻撃力を設定
	inline void SetHp(const int hp) { m_hp = hp; } // ヒットポイントを設定
	inline void SetMaxHp(const int maxHp) { m_maxHp = maxHp; } // 最大ヒットポイントを設定
	inline void SetDamage(const int damage) { m_totalDamage = damage; };
	inline void SetUseInvincible(const bool flag) { m_useInvincible = flag; }; // 無敵を使うかどうかを設定
	inline void SetInvincibleFlag(const bool flag) { m_invincibleFlag = flag; }; // 無敵フラグを設定

	inline int GetAtk() const { return m_atk; } // 攻撃力を取得
	inline int GetHp() const { return m_hp; } // ヒットポイントを取得
	inline int GetMaxHp() const { return m_maxHp; } // 最大ヒットポイントを取得
	inline int GetDamage() const { return m_totalDamage; };
	inline bool GetUseInvincible() const { return m_useInvincible; }; // 無敵を使うかどうかを取得
	inline bool GetInvincibleFlag() const { return m_invincibleFlag; }; // 無敵フラグを取得

	inline void AddAtk(const int atk) { m_atk += atk; } // 攻撃力を加算
	inline void AddHp(const int hp) { m_hp += hp; } // ヒットポイントを加算
	inline void AddDamage(const int damage) { m_totalDamage += damage; };
};

