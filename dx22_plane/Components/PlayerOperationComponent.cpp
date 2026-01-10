#include "PlayerOperationComponent.h"
#include "RigidBodyComponent.h"
#include "FighterComponent.h"
#include "Render2D.h"
#include "Render3D.h"
#include "HPBarMoveComponent.h"
#include "JumpComponent.h"
#include "Manager/GameObjectManager.h"
#include "DoorFadeComponent.h"
#include "Transform.h"
#include "input.h"
#include "TestSwordActionComponent.h"
#include "ArbitraryRotationComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include <vector>
#include <iostream>

using namespace DirectX::SimpleMath;

namespace {
	constexpr float DeltaTime = 0.016f; // 仮のデルタタイム
}

PlayerOperationComponent::PlayerOperationComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定
}

// 更新処理
void PlayerOperationComponent::Update() {
	const bool keyLeft = Input::GetKeyPress(VK_A) || Input::GetKeyPress(VK_LEFT) || Input::GetButtonPress(XINPUT_LEFT);
	const bool keyRight = Input::GetKeyPress(VK_D) || Input::GetKeyPress(VK_RIGHT) || Input::GetButtonPress(XINPUT_RIGHT);
	const bool keyUp = Input::GetKeyPress(VK_SPACE) || Input::GetKeyPress(VK_UP) || Input::GetButtonPress(XINPUT_A);

	FighterComponent* fighter = m_Object->GetComponent<FighterComponent>();
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	Render2DComponent* rend = m_Object->GetComponent<Render2DComponent>();
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (fighter == nullptr || rigid == nullptr)
	{
		return;
	}

	if (transform->GetPosition().y < -500.0f) {
		if (fighter != nullptr) {
			fighter->SetHp(0); // 落下したら体力0
		}
	}

	GameObject* fadeObj = GameObjectManager::GameObjectFindNameUI("fade");

	if (fadeObj != nullptr) {

		DoorFadeComponent* fade = fadeObj->GetComponent<DoorFadeComponent>();

		if (fighter->GetDeadFlag() == true) {
			fade->SetWinLoseFlag(false);
			fade->SetNextSceneName("ResultScene");
			fade->SetBootDoor(true);
		}
	}

	rend->SetInversionFlag(m_CurrentRightLeft); // 向きに合わせて反転

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

	if (rigid != nullptr) {

		// 後で減速の仕方を考える
		if (!keyLeft && !keyRight) {
			rigid->ReduceVelocity_X(0.5f);
		}
	}

	GameObject* hpUI = GameObjectManager::GameObjectFindNameUI("hpUI");
	if (hpUI != nullptr) {

		Render3DComponent* hpRend = hpUI->GetComponent<Render3DComponent>();

		const int maxHp = fighter->GetMaxHp();
		const int hp = fighter->GetHp();

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

	StateUpdate();
}

void PlayerOperationComponent::ChangeState(const PlayerState& state) {

	// 変更前に行う
	switch (m_CurrentState)
	{
	case PlayerState::NONE:
		break;
	case PlayerState::MOVE:
		break;
	case PlayerState::AIR:
		break;
	case PlayerState::GROUND:
		break;
	case PlayerState::JUMP:
		break;
	case PlayerState::ATTACK:
		break;
	case PlayerState::DAMAGE:
		break;
	case PlayerState::DEAD:
		break;
	default:
		break;
	}

	m_CurrentState = state;

	// 変更後すぐに行う処理
	switch (m_CurrentState)
	{
	case PlayerState::NONE:
		break;
	case PlayerState::MOVE:
		break;
	case PlayerState::AIR:
		break;
	case PlayerState::GROUND:
		break;
	case PlayerState::JUMP:
		break;
	case PlayerState::ATTACK:
		break;
	case PlayerState::DAMAGE:
		break;
	case PlayerState::DEAD:
		break;
	default:
		break;
	}
}

void PlayerOperationComponent::StateUpdate() {

	const bool keyLeft = Input::GetKeyPress(VK_A) || Input::GetKeyPress(VK_LEFT) || Input::GetButtonPress(XINPUT_LEFT);
	const bool keyRight = Input::GetKeyPress(VK_D) || Input::GetKeyPress(VK_RIGHT) || Input::GetButtonPress(XINPUT_RIGHT);
	const bool keyUp = Input::GetKeyPress(VK_SPACE) || Input::GetKeyPress(VK_UP) || Input::GetButtonPress(XINPUT_A);
	const bool keyBack = Input::GetKeyPress(VK_BACK) || Input::GetButtonPress(XINPUT_RIGHT_SHOULDER);
	const bool keyAttack = Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_X);
	TestExtrusionJudgeComponent* extrusion = m_Object->GetComponent<TestExtrusionJudgeComponent>();
	bool isGround = false;

	// ここで地面か空中か判断
	if (extrusion != nullptr) {
		isGround = extrusion->GetIsGround();
	}

	bool isMove = false;

	m_isJump = false;
	if (keyUp) {
		m_isJump = true;
	}

	m_IsMoveFlag = false; // 毎フレーム初期化	

	switch (m_CurrentState)
	{
	case PlayerState::NONE:

		if (keyLeft || keyRight || keyBack)
		{
			ChangeState(PlayerState::MOVE);
		}
		if (keyUp) {
			ChangeState(PlayerState::JUMP);
		}
		if (keyAttack) {
			ChangeState(PlayerState::ATTACK);
		}

		break;
	case PlayerState::MOVE:

		Move(keyRight, keyLeft, keyBack, &isMove);
		break;
	case PlayerState::AIR:
		break;
	case PlayerState::GROUND:
		break;
	case PlayerState::JUMP:

		Move(keyRight, keyLeft, keyBack, &isMove);
		break;
	case PlayerState::ATTACK:

		// 空中か地上かでフラグいるかな
		Attack(keyAttack, isGround);
		break;
	case PlayerState::DAMAGE:
		break;
	case PlayerState::DEAD:
		break;
	default:
		break;
	}

	JumpComponent* jumpComp = m_Object->GetComponent<JumpComponent>();
	if (jumpComp != nullptr) {
		jumpComp->SetJumpPress(keyUp);
	}

	m_beforeMove = isMove;
}

void PlayerOperationComponent::Move(const bool right, const bool left, const bool dash, bool* isMove) {

	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();

	// 移動しない
	if (rigid == nullptr || (right && left) || (!right && !left)) {
		ChangeState(PlayerState::NONE);
		return;
	}

	*isMove = true;
	m_IsMoveFlag = true;

	// 向きの更新
	if (left) {
		m_CurrentRightLeft = RightLeft::LEFT;
	}
	else if (right) {
		m_CurrentRightLeft = RightLeft::RIGHT;
	}

	// ダッシュ
	if (dash) {

		if (m_CurrentRightLeft == RightLeft::LEFT) {
			rigid->AddVelocity(Vector3(-200.0f, 0.0f, 0.0f));
			rigid->SetLimitVelocity_X(200.0f);
		}
		else if (m_CurrentRightLeft == RightLeft::RIGHT) {
			rigid->AddVelocity(Vector3(200.0f, 0.0f, 0.0f));
			rigid->SetLimitVelocity_X(200.0f);
		}
	}
	// 普通の移動
	else {
		if (left) {
			//rigid->ConstantVelocity_X(-150.0f);
			rigid->AddVelocity(Vector3(-40.0f, 0.0f, 0.0f));
			rigid->SetLimitVelocity_X(80.0f);
		}
		if (right) {
			//rigid->ConstantVelocity_X(150.0f);
			rigid->AddVelocity(Vector3(40.0f, 0.0f, 0.0f));
			rigid->SetLimitVelocity_X(80.0f);
		}
	}
}

void PlayerOperationComponent::Attack(const bool attack, const bool isGround) {

	if (m_WeaponObject == nullptr) {
		ChangeState(PlayerState::NONE);
		return;
	}

	TestSwordActionComponent* swordAction = m_WeaponObject->GetComponent<TestSwordActionComponent>();
	ArbitraryRotationComponent* swordArb = m_WeaponObject->GetComponent<ArbitraryRotationComponent>();

	if (swordAction == nullptr || swordArb == nullptr) {
		ChangeState(PlayerState::NONE);
		return;
	}

	// 剣を振る
	swordAction->SetIsAction(true);
	bool isFinish = swordArb->GetIsFinished();
	// ここで再発動
	if (isFinish == true) {
		swordArb->SetRollingActive(false);
		ChangeState(PlayerState::ATTACK);
		return;
	}

	ChangeState(PlayerState::NONE);
}