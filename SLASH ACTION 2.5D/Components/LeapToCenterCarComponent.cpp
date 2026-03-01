#include "LeapToCenterCarComponent.h"
#include "JumpComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "Manager/TimeManager.h"
#include "Manager/GameObjectManager.h"
#include "TestExtrusionJudgeComponent.h"
#include <iostream>

using namespace DirectX;

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
	XMFLOAT3 start = transComp->GetPosition();

	const float time = TimeManager::GetDeltaTime();

	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();

	GameObject* car1 = GameObjectManager::GameObjectFindName("terrain1");
	GameObject* car2 = GameObjectManager::GameObjectFindName("terrain2");
	TransformComponent* car1Trans = car1->GetComponent<TransformComponent>();
	TransformComponent* car2Trans = car2->GetComponent<TransformComponent>();
	//RigidBodyComponent* myRigid = p_object->GetComponent<RigidBodyComponent>();
	XMFLOAT3 car1Pos = car1Trans->GetPosition();
	XMFLOAT3 car2Pos = car2Trans->GetPosition();

	XMFLOAT3 myPos = myTrans->GetPosition();

	bool jumpPowerOn = false;
	if (abs(car1Pos.x - myPos.x) < 1.0f && leapState == 0) {
		leapState = 1; // ジャンプ準備
		jumpPowerOn = true;
	}

	if (leapState == 1) {

		XMFLOAT3 startPos = myTrans->GetPosition();
		XMFLOAT3 endPos = car1Trans->GetPosition() + XMFLOAT3(0.0f, 2.0f, 0.0f);

		XMVECTOR startPosVec = XMLoadFloat3(&startPos);
		XMVECTOR endPosVec = XMLoadFloat3(&endPos);

		leapTime += time;

		float t = std::clamp(leapTime / leapDuration, 0.0f, 1.0f);

		float ease = t * t * (3.0f - 2.0f * t); // イージング関数（SineEaseOut）

		XMVECTOR newPosVec = XMVectorLerp(startPosVec, endPosVec, ease);
		XMFLOAT3 newPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMStoreFloat3(&newPos, newPosVec);
		myTrans->SetPosition(newPos);

		XMFLOAT3 targetPos = XMFLOAT3(endPos - newPos);
		XMVECTOR targetPosVec = XMLoadFloat3(&targetPos);
		const float length = XMVectorGetX(XMVector3Length(targetPosVec));

		if (length < Threshold) {
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