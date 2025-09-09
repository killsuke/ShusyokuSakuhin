#include "TestSwordActionComponent.h"
#include "TestMoveComponent.h"
#include "GoAroundComponent.h"
#include "input.h"
#include "GameObjectManager.h"
#include "Collider.h"

TestSwordActionComponent::TestSwordActionComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("BONE"); // ソート番号を設定、のちに完成したらちゃんと変える
}

// 左右への位置を反転処理（入力があれば）
// 剣を振るアクション作成（入力があれば）

void TestSwordActionComponent::Update() {

	if (Input::GetKeyTrigger(VK_RETURN) == true || Input::GetButtonTrigger(XINPUT_X)) {
		m_swordAction = true;
	}

	if (m_swordAction == true) {
		SwordAction();
	}
}

void TestSwordActionComponent::SwordAction() {
	auto goAround = p_object->GetComponent<GoAroundComponent>();
	auto collider = p_object->GetComponent<ColliderComponent>();

	// ここのプレイヤー取得はのちに別のものに変更
	auto moveComp = GameObjectManager::GameObjectFindName("Player")->GetComponent<TestMoveComponent>();

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