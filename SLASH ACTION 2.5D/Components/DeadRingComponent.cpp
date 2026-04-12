#include "DeadRingComponent.h"
#include "TransformComponent.h"
#include "TimeLineComponent.h"
#include "RenderTextureLuminescenceComponent.h"

DeadRingComponent::DeadRingComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定
}

void DeadRingComponent::Update()
{
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	RenderTextureLuminescenceComponent* rend = m_Object->GetComponent<RenderTextureLuminescenceComponent>();

	if (transform == nullptr || rend == nullptr) {
		return;
	}

	transform->AddScale({ m_ScaleUpSpeed, m_ScaleUpSpeed, 0.0f });

	m_ScaleUpSpeed *= 0.95f; // 徐々に拡大スピードを減らす

	rend->AddColor_A(-m_DeleteSpeed);
	rend->AddGlowColor_A(-m_DeleteSpeed);
}