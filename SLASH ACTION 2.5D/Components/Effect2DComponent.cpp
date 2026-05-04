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

	m_cutRecordTime += deltaTime;
	m_recordTime += deltaTime;

	if (m_recordTime > m_maxTime) {
		m_Object->SetDeleteFg(true);
		return;
	}

	// テクスチャの分割数を変更
	if(rendBill != nullptr) {
		if (m_cutRecordTime > m_onePieceTime) {
			m_nowCutNumX += 1.0f;

			Mesh* mesh = rendBill->GetMesh();
			
			if(mesh == nullptr) {
				return;
			}

			mesh->SetCutNum(m_nowCutNumX, 1.0f);
			m_cutRecordTime = 0.0f;
		}
	}
}