#pragma once
#include "Component.h"
class Effect2DComponent : public Component
{
private:
	const float m_deltaTime = 0.016f;
	float m_cutRecordTime = 0.0f;
	float m_recordTime = 0.0f;	// 経過時間
	float m_onePieceTime = 0.0f;	// 分割数切り替えの時の１枚辺りの時間
	float m_maxCutNumX = 1.0f;
	float m_nowCutNumX = 1.0f;
	float m_maxTime = 0.0f; // エフェクトが消えるまでの時間

public:
	Effect2DComponent(GameObject& obj);
	~Effect2DComponent() = default;
	void Update() override;
	/*void EffectPlay();
	void EffectStop();*/

	//inline void SetOnePieceTime(const float time) { m_onePieceTime = time; };
	//inline void SetMaxCutNumX(const float num) { m_maxCutNumX = num; };
	inline void SetMaxTimeAndCut_X(const float time,const float cut) { 
		m_maxTime = time; 
		m_maxCutNumX = cut;

		m_onePieceTime = m_maxTime / m_maxCutNumX;	// １枚辺りの時間を計算
	};
};