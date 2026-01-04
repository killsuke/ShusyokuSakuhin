#pragma once
#include "Component.h"
#include <SimpleMath.h>
#include <algorithm>

enum class CutDirection
{
	VERTICAL,	// 垂直方向
	HORIZONTAL	// 水平方向
};

struct CutEvent {

	uint32_t targetID = 0;   // 被攻撃者のインスタンスID
	CutDirection cutDirection = CutDirection::VERTICAL;
	float ratio1 = 0.0f;
	float ratio2 = 0.0f;
};

class MeshCut2DComponent : public Component
{
private:
	CutDirection m_CutDirection = CutDirection::VERTICAL;
	// カットする時の割合
	float m_CutRatio1 = 0.5f;
	float m_CutRatio2 = 0.5f;
	uint64_t m_listenerID_CutEvent = 0; // ヒットイベントのリスナーID
	GameObject* m_CutObj1 = nullptr;
	GameObject* m_CutObj2 = nullptr;
	uint32_t m_CutObj1ID = 0;
	uint32_t m_CutObj2ID = 0;

	void MakeCutPoints(float& vL, float& vR,const float ratio);
public:

	MeshCut2DComponent(GameObject& obj);
	~MeshCut2DComponent();
	void Update() override;

	// カット設定の初期化
	// VERTICALの時、ratio1が上側、ratio2が下側の割合
	// HORIZONTALの時、ratio1が左側、ratio2が右側の割合
	void InitCutSettings(const CutDirection direction, const float ratio1, const float ratio2) {
		m_CutDirection = direction;
		m_CutRatio1 = std::clamp(ratio1, 0.0f, 1.0f);
		m_CutRatio2 = std::clamp(ratio2, 0.0f, 1.0f);
	}

	void MeshCutAction(const CutEvent& event);

	// 外部からアクセスして切断後のオブジェクトを消去する
	void DeleteCutObjs() {
		m_CutObj1->SetDeleteFg(true);
		m_CutObj2->SetDeleteFg(true);
	}

	// 切断後のオブジェクトを取得
	GameObject* GetCutObj1()const { return m_CutObj1; };
	GameObject* GetCutObj2()const { return m_CutObj2; };

	uint32_t GetCutObj1ID()const { return m_CutObj1ID; };
	uint32_t GetCutObj2ID()const { return m_CutObj2ID; };
};