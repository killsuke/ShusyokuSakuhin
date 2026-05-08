#pragma once
#include "Component.h"
class Effect2DComponent : public Component
{
private:
	float m_CutRecordTime = 0.0f;
	float m_RecordTime = 0.0f;	// 経過時間
	float m_OnePieceTime = 0.0f;	// 分割数切り替えの時の１枚辺りの時間
	float m_MaxCutNumX = 1.0f;
	float m_NowCutNumX = 1.0f;
	float m_MaxTime = 0.0f; // エフェクトが消えるまでの時間

public:

	Effect2DComponent(GameObject& obj);
	~Effect2DComponent() = default;
	void Update() override;

	// テクスチャの最大時間と分割数を設定
	void SetMaxTimeAndCut_X(const float time, const float cut) {
		m_MaxTime = time;
		m_MaxCutNumX = cut;

		m_OnePieceTime = m_MaxTime / m_MaxCutNumX;	// １枚辺りの時間を計算
	};
};