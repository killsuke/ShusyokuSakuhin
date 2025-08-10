#include "GoAroundComponent.h"
#include "Transform.h"
#include <SimpleMath.h>
#include "GameObjectManager.h"

using namespace DirectX::SimpleMath;

GoAroundComponent::GoAroundComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = GO_AROUND; // ƒ\[ƒg”Ô†‚ðÝ’è
}

void GoAroundComponent::Update()
{
	m_deltaTime = 0.016f; // XVŠÔŠuŽžŠÔ‚ð16ms‚¸‚Â‰ÁŽZ

	auto transform = p_object->GetComponent<TransformComponent>();
	auto rollingObjPos = transform->GetPosition();

	auto obj = GameObjectManager::GameObjectFindName("Player");

	auto centerTrans = m_CenterObject->GetComponent<TransformComponent>();
	auto centerPos = centerTrans->GetPosition();

	auto delta = rollingObjPos - centerPos;
	auto length = delta.Length();

	auto angle = m_deltaTime * 0.5f; // Šp“x‚ðXVŠÔŠuŽžŠÔ‚ÉŠî‚Ã‚¢‚ÄŒvŽZ

	Quaternion rotation = Quaternion::CreateFromAxisAngle(Vector3::Forward, angle);

	Vector3 rotatedOffset = Vector3::Transform(delta, rotation);

	transform->SetPosition(centerPos + rotatedOffset);
}