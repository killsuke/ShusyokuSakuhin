#include "TestMoveComponent.h"
#include "RigidBodyComponent.h"
#include "FighterComponent.h"
#include "Render2D.h"
#include "Render3D.h"
#include "HPBarMoveComponent.h"
#include "JumpComponent.h"
#include "GameObjectManager.h"
#include "DoorFadeComponent.h"
#include "Transform.h"
#include "input.h"

TestMoveComponent::TestMoveComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定
}

// 更新処理
void TestMoveComponent::Update() {
	bool keyJ = false;
	bool keyL = false;
	bool keyW = false;

	auto fighter = p_object->GetComponent<FighterComponent>();
	auto rigid = p_object->GetComponent<RigidBodyComponent>();
	auto rend = p_object->GetComponent<Render2DComponent>();

	auto transform = p_object->GetComponent<TransformComponent>();

	if(transform->GetPosition().y < -500.0f) {
		if(fighter != nullptr) {
			fighter->SetHp(0); // 落下したら体力0
		}
	}

	bool nowMove = false;

	auto fadeObj = GameObjectManager::GameObjectFindNameUI("fade");
	auto fade = fadeObj->GetComponent<DoorFadeComponent>();

	if (fighter->GetDeadFlag() == true) {
		fade->SetWinLoseFlag(false);
		fade->SetNextSceneName("ResultScene");
		fade->SetBootDoor(true);
	}

	if (/*fighter == nullptr || */rigid == nullptr)
	{
		return;
	}

	if (Input::GetKeyPress(VK_A) == true )
	{
		m_rightLeft = false; // 左向き
		keyJ = true;
		nowMove = true;
	}
	if (Input::GetKeyPress(VK_D) == true ) {
		m_rightLeft = true; // 右向き
		keyL = true;
		nowMove = true;
	}

	rend->SetInversionFlag(!m_rightLeft); // 向きに合わせて反転

	if(fighter->GetInvincibleFlag() == true) {
		// 無敵状態の時間を計測
		m_recordTime += m_deltaTime;

		if (m_recordTime >= 0.1f) {
			// 無敵状態なら透明にする
			rend->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
			m_recordTime = 0.0f;
		}
		else {
			rend->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	else {
		rend->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)); // 元に戻す
	}

	//bool isTrigger = false;

	//if (Input::GetKeyTrigger(VK_W) == true || Input::GetButtonTrigger(XINPUT_A) == true) {
	//	isTrigger = true;
	//}

	if (Input::GetKeyPress(VK_W) == true) {
		keyW = true;
	}

	auto jump = p_object->GetComponent<JumpComponent>();
	if (jump != nullptr) {
		jump->SetJumpPress(keyW);
	}

	if (rigid != nullptr) {
		if (keyJ == true && keyL == false) {
			rigid->ConstantVelocity_X(-150.0f);
		}
		if (keyL == true && keyJ == false) {
			rigid->ConstantVelocity_X(150.0f);
		}


		/*if (m_beforeMove == true && nowMove == false) {
			rigid->ReduceVelocity_X(0.5f);
		}*/
		if (keyL == false && keyJ == false) {
			rigid->ReduceVelocity_X(0.5f);
		}
	}

	/*if (Input::GetKeyTrigger(VK_RETURN)) {
		fighter->AddDamage(1);
	}*/

	auto hpUI = GameObjectManager::GameObjectFindNameUI("hpUI");
	auto hpRend = hpUI->GetComponent<Render3DComponent>();

	auto maxHp = fighter->GetMaxHp();
	auto hp = fighter->GetHp();

	// HPのカラー変更
	if(hp == 0) {
		hpRend->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)); // 透明
	}
	else if (hp < (maxHp * 0.3f)) {
		hpRend->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)); // 赤色
	}
	else if (hp < (maxHp * 0.5f)) {
		hpRend->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)); // 黄色
	}
	else {
		hpRend->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)); // 緑色
	}
	m_beforeMove = nowMove;
}