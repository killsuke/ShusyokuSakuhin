#include "LeapToCenterCarComponent.h"
#include "JumpComponent.h"
#include "Transform.h"
#include "RigidBodyComponent.h"
#include "TimeManager.h"
#include "GameObjectManager.h"
#include "TestExtrusionJudgeComponent.h"
#include <iostream>

using namespace DirectX::SimpleMath;

namespace {
	constexpr float Threshold = 1.0f;
}

LeapToCenterCarComponent::LeapToCenterCarComponent(GameObject& obj) :Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定、仮にMOVE_TERRAINを設置
}

void LeapToCenterCarComponent::Update() {

	JumpComponent* jumpComp = m_Object->GetComponent<JumpComponent>();
	TransformComponent* transComp = m_Object->GetComponent<TransformComponent>();
	Vector3 start = transComp->GetPosition();

	float time = TimeManager::GetDeltaTime();

	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();

	GameObject* car1 = GameObjectManager::GameObjectFindName("terrain1");
	GameObject* car2 = GameObjectManager::GameObjectFindName("terrain2");
	TransformComponent* car1Trans = car1->GetComponent<TransformComponent>();
	TransformComponent* car2Trans = car2->GetComponent<TransformComponent>();
	//RigidBodyComponent* myRigid = p_object->GetComponent<RigidBodyComponent>();
	Vector3 car1Pos = car1Trans->GetPosition();
	Vector3 car2Pos = car2Trans->GetPosition();

	Vector3 myPos = myTrans->GetPosition();

	bool jumpPowerOn = false;
	if (abs(car1Pos.x - myPos.x) < 1.0f && leapState == 0) {
		leapState = 1; // ジャンプ準備
		jumpPowerOn = true;
	}

	if (leapState == 1) {

		Vector3 startPos = myTrans->GetPosition();
		Vector3 endPos = car1Trans->GetPosition() + Vector3(0.0f, 2.0f, 0.0f);

		leapTime += time;

		float t = std::clamp(leapTime / leapDuration, 0.0f, 1.0f);

		float ease = t * t * (3.0f - 2.0f * t); // イージング関数（SineEaseOut）

		Vector3 newPos = DirectX::XMVectorLerp(startPos, endPos, ease);
		myTrans->SetPosition(newPos);

		if ((endPos - newPos).Length() < Threshold) {
			leapState = 2; // ジャンプ中
			leapTime = 0.0f;
		}

		if (jumpPowerOn == true) {
			jumpComp->SetJumpPower(90.0f);
		}

		jumpComp->SetJumpPress(true);

	}

	if (leapState == 2) {

	}

}