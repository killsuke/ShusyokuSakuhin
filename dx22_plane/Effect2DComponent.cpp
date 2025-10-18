#include "Effect2DComponent.h"
#include "RenderBillboard.h"

Effect2DComponent::Effect2DComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("EFFECT"); // ソート番号を設定
}

void Effect2DComponent::Update() {
	auto rendBill = p_object->GetComponent<RenderBillboardComponent>();

	m_cutRecordTime += m_deltaTime;
	m_recordTime += m_deltaTime;

	if (m_recordTime > m_maxTime) {
		p_object->SetDeleteFg(true);
		return;
	}

	// テクスチャの分割数を変更
	if(rendBill != nullptr) {
		if (m_cutRecordTime > m_onePieceTime) {
			m_nowCutNumX += 1.0f;
			/*if (m_nowCutNumX > m_maxCutNumX) {
				m_nowCutNumX = 1.0f;
			}*/

			auto mesh = rendBill->GetMesh();
			
			mesh->SetCutNum(m_nowCutNumX, 1.0f);
			m_cutRecordTime = 0.0f;
		}
	}
}