#include "PlayerOperationComponent.h"
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
#include "TestSwordActionComponent.h"
#include <vector>

using namespace DirectX::SimpleMath;

namespace{
	constexpr float DeltaTime = 0.016f; // 仮のデルタタイム
}

PlayerOperationComponent::PlayerOperationComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定
}

// 更新処理
void PlayerOperationComponent::Update() {
	bool keyJ = false;
	bool keyL = false;
	bool keyW = false;
	bool keyBack = false;

	auto fighter = m_Object->GetComponent<FighterComponent>();
	auto rigid = m_Object->GetComponent<RigidBodyComponent>();
	auto rend = m_Object->GetComponent<Render2DComponent>();

	auto transform = m_Object->GetComponent<TransformComponent>();

	if (transform->GetPosition().y < -500.0f) {
		if (fighter != nullptr) {
			fighter->SetHp(0); // 落下したら体力0
		}
	}

	bool nowMove = false;

	Vector3 rogg = transform->GetRotation();
	auto forward = rogg.Forward;
	auto up = rogg.Up;
	auto right = rogg.Right;

	auto fadeObj = GameObjectManager::GameObjectFindNameUI("fade");

	if (fadeObj != nullptr) {

		auto fade = fadeObj->GetComponent<DoorFadeComponent>();

		if (fighter->GetDeadFlag() == true) {
			fade->SetWinLoseFlag(false);
			fade->SetNextSceneName("ResultScene");
			fade->SetBootDoor(true);
		}
	}

	if (/*fighter == nullptr || */rigid == nullptr)
	{
		return;
	}

	m_moveFlag = false; // 毎フレーム初期化	
	if ((Input::GetKeyPress(VK_A) || Input::GetButtonPress(XINPUT_LEFT)) == true)
	{
		m_rightLeft = false; // 左向き
		keyJ = true;
		nowMove = true;
		m_moveFlag = true;
	}
	if ((Input::GetKeyPress(VK_D) || Input::GetButtonPress(XINPUT_RIGHT)) == true) {
		m_rightLeft = true; // 右向き
		keyL = true;
		nowMove = true;
		m_moveFlag = true;
	}
	// ダッシュ
	if ((Input::GetKeyPress(VK_BACK) || Input::GetButtonPress(XINPUT_RIGHT_SHOULDER)) == true) {
		keyBack = true;
		m_moveFlag = true;
	}

	if(Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_X)) {
		std::vector<GameObject*> swords = GameObjectManager::GameObjectFindTag("Sword");

		if(swords.size() > 0) {
			TestSwordActionComponent* swordAction = swords[0]->GetComponent<TestSwordActionComponent>();
			if (swordAction != nullptr) {
				swordAction->SetIsAction(true);
			}
		}
	}

	rend->SetInversionFlag(!m_rightLeft); // 向きに合わせて反転

	if (fighter->GetInvincibleFlag() == true) {
		// 無敵状態の時間を計測
		m_RecordTime += DeltaTime;

		if (m_RecordTime >= 0.1f) {
			// 無敵状態なら透明にする
			rend->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
			m_RecordTime = 0.0f;
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

	if ((Input::GetKeyPress(VK_SPACE) || Input::GetButtonPress(XINPUT_A)) == true) {
		keyW = true;
	}

	m_isJump = false;
	if ((Input::GetKeyTrigger(VK_SPACE) || Input::GetButtonTrigger(XINPUT_A)) == true) {
		m_isJump = true;
	}

	auto jump = m_Object->GetComponent<JumpComponent>();
	if (jump != nullptr) {
		jump->SetJumpPress(keyW);
	}

	if (rigid != nullptr) {

		if (keyBack == true && m_rightLeft == false) {
			rigid->AddVelocity(Vector3(-200.0f, 0.0f, 0.0f));
			rigid->SetLimitVelocity_X(200.0f);

		}
		if (keyBack == true && m_rightLeft == true) {
			rigid->AddVelocity(Vector3(200.0f, 0.0f, 0.0f));
			rigid->SetLimitVelocity_X(200.0f);

		}

		if (keyBack == false) {
			if (keyJ == true && keyL == false) {
				//rigid->ConstantVelocity_X(-150.0f);
				rigid->AddVelocity(Vector3(-40.0f, 0.0f, 0.0f));
				rigid->SetLimitVelocity_X(80.0f);
			}
			if (keyL == true && keyJ == false) {
				//rigid->ConstantVelocity_X(150.0f);
				rigid->AddVelocity(Vector3(40.0f, 0.0f, 0.0f));
				rigid->SetLimitVelocity_X(80.0f);
			}
		}

		/*if (m_beforeMove == true && nowMove == false) {
			rigid->ReduceVelocity_X(0.5f);
		}*/

		// 後で減速の仕方を考える
		if (keyL == false && keyJ == false) {
			rigid->ReduceVelocity_X(0.5f);
		}
	}

	auto hpUI = GameObjectManager::GameObjectFindNameUI("hpUI");
	if (hpUI != nullptr) {

		auto hpRend = hpUI->GetComponent<Render3DComponent>();

		auto maxHp = fighter->GetMaxHp();
		auto hp = fighter->GetHp();

		// HPのカラー変更
		if (hp == 0) {
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
	}
	m_beforeMove = nowMove;
}