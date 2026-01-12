#pragma once
#include "Component.h"
#include "RightLeft.h"
#include <DirectXMath.h>

enum class ESwordActionState
{
	NONE,
	SLASH_1ST,
	SLASH_2ND,
	SLASH_3RD,
};

enum class SlashPattern
{
	GROUND_SLASH,
	AIR_SLASH,
	MAX_SLASH_PATTERN
};

struct SwordActionPattern
{
	float startAngle = 0.0f;
	float endAngle = 0.0f;
	DirectX::XMFLOAT3 arbitraryAxis = { 0.0f,0.0f,1.0f };
	DirectX::XMFLOAT3 lockAngle = { 0.0f,0.0f,0.0f };
};

class TestSwordActionComponent : public Component
{
private:
	RightLeft m_BeforeDirection = RightLeft::RIGHT;	// 前フレームのプレイヤーの向き
	RightLeft m_RightLeft = RightLeft::RIGHT; // プレイヤーの向き、trueが右、falseが左
	bool m_IsAction = false; // 剣を振っているかどうか
	bool m_IsUseTrailFlag = false; // トレイルエフェクトを使うかどうか
	bool m_IsSwordAction = false;

	int m_TestSlashCount = 0;
	float m_DeltaTime = 0.016f;
	GameObject* m_Holder = nullptr; // 持ち主のオブジェクト
	ESwordActionState m_SwordActionState = ESwordActionState::NONE;
	SwordActionPattern m_SwordActionPattern = {};

public:
	TestSwordActionComponent(GameObject& obj);
	~TestSwordActionComponent() = default;

	void Update()override;

	void CreateSwordEffect(); // 剣のエフェクトを生成
	void ChoiceSlashPattern(const RightLeft& horizontalAxis); // 振り方パターンを選択

	void SwordAction();
	void SetHolder(GameObject* holder) { m_Holder = holder; };
	void SetIsAction(const bool action) { m_IsAction = action; };
	void SetIsAction(const bool action, const SlashPattern& pattern) {
		m_IsAction = action;

		switch (pattern)
		{
		case SlashPattern::GROUND_SLASH:
			m_SwordActionState = ESwordActionState::SLASH_1ST;
			break;
		case SlashPattern::AIR_SLASH:
			m_SwordActionState = ESwordActionState::SLASH_1ST;
			break;
		default:
			break;
		}

		m_TestSlashCount = 4;
	};
	void SetSwordActionState(const ESwordActionState& state) { m_SwordActionState = state; };

	GameObject* GetHolder() { return m_Holder; };
	ESwordActionState GetSwordActionState() const { return m_SwordActionState; };
	RightLeft GetRightLeft()const { return m_RightLeft; };
	bool GetIsSwordAction()const { return m_IsSwordAction; };
};

