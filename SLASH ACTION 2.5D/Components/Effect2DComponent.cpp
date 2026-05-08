#include "Effect2DComponent.h"
#include "RenderBillboardComponent.h"
#include "Manager/TimeManager.h"

Effect2DComponent::Effect2DComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("EFFECT"); // ソート番号を設定
}

void Effect2DComponent::Update() {
	
	RenderBillboardComponent* rendBill = m_Object->GetComponent<RenderBillboardComponent>();

	if(rendBill == nullptr) {
		return;
	}

	const float deltaTime = TimeManager::GetFixedDeltaTime();

	m_CutRecordTime += deltaTime;
	m_RecordTime += deltaTime;

	if (m_RecordTime > m_MaxTime) {
		m_Object->SetDeleteFg(true);
		return;
	}

	// テクスチャの分割数を変更
	if(rendBill != nullptr) {
		if (m_CutRecordTime > m_OnePieceTime) {
			m_NowCutNumX += 1.0f;

			Mesh* mesh = rendBill->GetMesh();
			
			if(mesh == nullptr) {
				return;
			}

			mesh->SetCutNum(m_NowCutNumX, 1.0f);
			m_CutRecordTime = 0.0f;
		}
	}
}