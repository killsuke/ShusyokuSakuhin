#pragma once
#include "Component.h"
#include <DirectXMath.h>

enum class ESwordActionState
{
	NONE,
	SLASH_1ST,
	SLASH_2ND,
	SLASH_3RD,
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
	bool m_IsSwordAction = false;
	bool m_BeforeDirection = true;	// 前フレームのプレイヤーの向き
	bool m_RightLeft = true; // プレイヤーの向き、trueが右、falseが左
	bool m_IsAction = false; // 剣を振っているかどうか

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
	void ChoiceSlashPattern(const bool horizontalAxis); // 振り方パターンを選択

	void SwordAction();
	void SetHolder(GameObject* holder) { m_Holder = holder; };
	void SetIsAction(const bool action) { m_IsAction = action; };

	GameObject* GetHolder() { return m_Holder; };
};

