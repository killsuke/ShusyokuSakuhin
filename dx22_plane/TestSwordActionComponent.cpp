#include "TestSwordActionComponent.h"
#include "TestMoveComponent.h"
#include "GoAroundComponent.h"
#include "Transform.h"
#include "input.h"
#include "GameObjectManager.h"
#include "Collider.h"

TestSwordActionComponent::TestSwordActionComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("BONE"); // ソート番号を設定、のちに完成したらちゃんと変える
}

// 左右への位置を反転処理（入力があれば）
// 剣を振るアクション作成（入力があれば）

void TestSwordActionComponent::Update() {

	if (m_holder == nullptr) {
		return;
	}

	auto goAround = p_object->GetComponent<GoAroundComponent>();
	auto moveComp = m_holder->GetComponent<TestMoveComponent>();

	bool keyEnter = Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_X);

	if (keyEnter == true) {
		m_swordAction = true;
		goAround->SetActiveFlag(true);
		p_object->SetDrawContainerChangeFlag(DrawContainer::AbsFfont, true);
	}
	else if(keyEnter == false && m_swordAction == false){
		p_object->SetDrawContainerChangeFlag(DrawContainer::Default, true);
		goAround->SetActiveFlag(false);
		auto holderTrans = m_holder->GetComponent<TransformComponent>();
		auto holderPos = holderTrans->GetPosition();
		auto objTrans = p_object->GetComponent<TransformComponent>();

		if (moveComp->GetRightLeft() == true) {
			objTrans->SetRotation({ 0.0f,0.0f,130.0f });
			objTrans->SetPosition({ holderPos.x - 3.0f,holderPos.y + 3.0f,objTrans->GetPosition().z });
		}
		else {
			objTrans->SetRotation({ 0.0f,0.0f,50.0f });
			objTrans->SetPosition({ holderPos.x + 3.0f,holderPos.y + 3.0f,objTrans->GetPosition().z });
		}
	}

	if (m_swordAction == true) {
		SwordAction();
	}
}

void TestSwordActionComponent::SwordAction() {
	auto goAround = p_object->GetComponent<GoAroundComponent>();
	auto collider = p_object->GetComponent<ColliderComponent>();

	// ここのプレイヤー取得はのちに別のものに変更
	auto moveComp = m_holder->GetComponent<TestMoveComponent>();

	if (goAround == nullptr || collider == nullptr) {
		return;
	}

	collider->SetActiveColliderFlag(true);

	goAround->SetRollingActive(true);

	// 左右の向き変わったら、現在角度に＋90度して左右反転、上記の止める処理もちょいと変える？タイム方式とかに
	bool direction = moveComp->GetRightLeft();

	// ここで止める処理の制限がない
	if ((goAround->GetNowAngleDegree() < 0 && direction == true) || (goAround->GetNowAngleDegree() > 180.0f && direction == false)) {
		goAround->SetRollingActive(false);
		goAround->ResetAngle();
		collider->SetActiveColliderFlag(false);
		m_swordAction = false;
		p_object->SetDrawContainerChangeFlag(DrawContainer::Default,true);
	}

	if (direction == true && m_beforeDirection == false) { // 右向き
		goAround->SetClockwise(true);
		goAround->SetFlipRequested(true);
	}
	else if (direction == false && m_beforeDirection == true) { // 左向き
		goAround->SetClockwise(false);
		goAround->SetFlipRequested(true);
	}

	m_beforeDirection = direction;
}