#pragma once
#include "Component.h"

// テストコード用のヒットイベント構造体
struct HitEvent {
	uint32_t attackerID; // 攻撃者のインスタンスID
	uint32_t targetID;   // 被攻撃者のインスタンスID
};

struct DamageEvent {
	uint32_t attackerID; // 攻撃者のインスタンスID
	uint32_t targetID;   // 被攻撃者のインスタンスID
	int damage;			// ダメージ量
};

struct FallHitEvent {

	uint32_t attackerID; // 攻撃者のインスタンスID
	uint32_t targetID; // 被攻撃者のインスタンスID
	int damage;			// ダメージ量
};

struct FallDamageEvent {
	uint32_t attackerID; // 攻撃者のインスタンスID
	uint32_t targetID; // 被攻撃者のインスタンスID
	int damage;			// ダメージ量
};

class FighterComponent final : public Component
{
private:
	int m_Atk = 0; // 攻撃力
	int m_Hp = 0; // ヒットポイント
	int m_MaxHp = 0; // 最大ヒットポイント
	int m_TotalDamage = 0;	// ダメージ
	float m_recordTime = 0.0f; // 経過時間
	float m_DeadRecordTime = 0.0f;
	bool m_UseInvincible = false; // 無敵を使うかどうか
	bool m_InvincibleFlag = false; // 無敵フラグ
	bool m_DeadFlag = false; // 死亡フラグ
	bool m_UseDeadFlag = true; // 死亡フラグそのものを使うか
	uint64_t m_listenerID_HitEvent = 0; // ヒットイベントのリスナーID
	uint64_t m_listenerID_FallHitEvent = 0; // 落下ヒットイベントのリスナーID

public:
	FighterComponent(GameObject& obj);
	~FighterComponent();

	void Update() override;

	inline void SetAtk(const int atk) { m_Atk = atk; } // 攻撃力を設定
	inline void SetHp(const int hp) { m_Hp = hp; } // ヒットポイントを設定
	inline void SetMaxHp(const int maxHp) { m_MaxHp = maxHp; } // 最大ヒットポイントを設定
	inline void SetDamage(const int damage) {
		if (m_InvincibleFlag == false) {
			m_TotalDamage = damage;
		}
	};
	inline void SetUseInvincible(const bool flag) { m_UseInvincible = flag; }; // 無敵を使うかどうかを設定
	inline void SetInvincibleFlag(const bool flag) { m_InvincibleFlag = flag; }; // 無敵フラグを設定
	inline void SetUseDeadFlag(const bool flag) { m_UseDeadFlag = flag; }; // 死亡フラグを設定

	inline int GetAtk() const { return m_Atk; } // 攻撃力を取得
	inline int GetHp() const { return m_Hp; } // ヒットポイントを取得
	inline int GetMaxHp() const { return m_MaxHp; } // 最大ヒットポイントを取得
	inline int GetTotalDamage() const { return m_TotalDamage; };
	inline bool GetUseInvincible() const { return m_UseInvincible; }; // 無敵を使うかどうかを取得
	inline bool GetInvincibleFlag() const { return m_InvincibleFlag; }; // 無敵フラグを取得
	inline bool GetDeadFlag() const { return m_DeadFlag; }; // 死亡フラグを取得

	inline void AddAtk(const int atk) { m_Atk += atk; } // 攻撃力を加算
	inline void AddHp(const int hp) { m_Hp += hp; } // ヒットポイントを加算
	inline void AddDamage(const int damage) {
		if (m_InvincibleFlag == false) {
			m_TotalDamage += damage;
		}
	};

	void DamageProcess(const HitEvent& event);
	void FallDamageProcess(const FallHitEvent& event);

	// ヒットした際のイベント
	void OnHit(const HitEvent& event);

	void OnFallHit(const FallHitEvent& event); // 落下してヒットした際のイベント
};