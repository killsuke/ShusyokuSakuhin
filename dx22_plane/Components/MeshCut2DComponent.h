#pragma once
#include "Component.h"
#include <SimpleMath.h>
#include <algorithm>

enum class CutDirection
{
	VERTICAL,	// 垂直方向
	HORIZONTAL	// 水平方向
};

class MeshCut2DComponent : public Component
{
private:
	CutDirection m_CutDirection = CutDirection::VERTICAL;
	// カットする時の割合
	float m_CutRatio1 = 0.5f;
	float m_CutRatio2 = 0.5f;

	void MakeCutPoints(float& vL, float& vR,const float ratio);
public:

	MeshCut2DComponent(GameObject& obj);
	~MeshCut2DComponent() = default;
	void Update() override;

	// カット設定の初期化
	// VERTICALの時、ratio1が上側、ratio2が下側の割合
	// HORIZONTALの時、ratio1が左側、ratio2が右側の割合
	void InitCutSettings(const CutDirection direction, const float ratio1, const float ratio2) {
		m_CutDirection = direction;
		m_CutRatio1 = std::clamp(ratio1, 0.0f, 1.0f);
		m_CutRatio2 = std::clamp(ratio2, 0.0f, 1.0f);
	}

	void MeshCutAction();
};