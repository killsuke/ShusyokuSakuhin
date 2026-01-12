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

using namespace DirectX;

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

	PlayerState previousState = m_CurrentState;

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
	case PlayerState::CHARGE:
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
	case PlayerState::CHARGE:
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
	const bool keyCharge = Input::GetKeyPress(VK_RETURN) || Input::GetButtonPress(XINPUT_X);
	const bool keyCAttack = Input::GetKeyRelease(VK_RETURN) || Input::GetButtonRelease(XINPUT_X);

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
		if(keyCharge)
		{
			ChangeState(PlayerState::CHARGE);
		}
		// ここで攻撃するか否かを判断
		Attack(keyAttack, isGround);

		break;
	case PlayerState::MOVE:

		// ここで攻撃するか否かを判断
		Attack(keyAttack, isGround);
		Move(keyRight, keyLeft, keyBack, keyAttack, isGround, &isMove);
		break;
	case PlayerState::AIR:
		break;
	case PlayerState::GROUND:
		break;
	case PlayerState::JUMP:

		//Move(keyRight, keyLeft, keyBack, keyAttack, isGround, &isMove);
		break;
	case PlayerState::ATTACK:
		break;
	case PlayerState::CHARGE:
		Charge(keyCharge,keyCAttack);
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

	if (!keyLeft && !keyRight && !keyUp && !keyBack && !keyAttack && !keyCharge && !keyCAttack) {
		ChangeState(PlayerState::NONE);
	}
}

void PlayerOperationComponent::Move(const bool right, const bool left, const bool dash, const bool attack, const bool isGround, bool* isMove) {

	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	TestSwordActionComponent* swordAction = m_WeaponObject->GetComponent<TestSwordActionComponent>();

	// 地面の攻撃は移動しない
	const bool isAction = swordAction->GetIsSwordAction();
	if (isAction && isGround) {
		rigid->ClearVelocity_X();
		return;
	}

	// 移動しない
	if (rigid == nullptr || (right && left) || (!right && !left)) {
		ChangeState(PlayerState::NONE);
		return;
	}

	// 移動する
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
			rigid->AddVelocity(XMFLOAT3(-200.0f, 0.0f, 0.0f));
			rigid->SetLimitVelocity_X(200.0f);
		}
		else if (m_CurrentRightLeft == RightLeft::RIGHT) {
			rigid->AddVelocity(XMFLOAT3(200.0f, 0.0f, 0.0f));
			rigid->SetLimitVelocity_X(200.0f);
		}
	}
	// 普通の移動
	else {
		if (left) {
			//rigid->ConstantVelocity_X(-150.0f);
			rigid->AddVelocity(XMFLOAT3(-40.0f, 0.0f, 0.0f));
			rigid->SetLimitVelocity_X(80.0f);
		}
		if (right) {
			//rigid->ConstantVelocity_X(150.0f);
			rigid->AddVelocity(XMFLOAT3(40.0f, 0.0f, 0.0f));
			rigid->SetLimitVelocity_X(80.0f);
		}
	}
}

// これそのものを呼び出すと攻撃する
void PlayerOperationComponent::Attack(const bool attack, const bool isGround) {

	// 武器オブジェクトがない場合は終了
	if (m_WeaponObject == nullptr) {
		ChangeState(PlayerState::NONE);
		return;
	}

	TestSwordActionComponent* swordAction = m_WeaponObject->GetComponent<TestSwordActionComponent>();
	ArbitraryRotationComponent* swordArb = m_WeaponObject->GetComponent<ArbitraryRotationComponent>();

	// コンポーネントがない場合は終了
	if (swordAction == nullptr || swordArb == nullptr) {
		ChangeState(PlayerState::NONE);
		return;
	}

	if(isGround == true){
		// 地面の上での攻撃
		swordAction->SetIsAction(attack);
	}
	else {
		// 空中での攻撃
		swordAction->SetIsAction(attack,SlashPattern::AIR_SLASH);
	}

	// 攻撃が終了したかどうかを判定
	if (attack == true) {

		const bool isFinish = swordArb->GetIsFinished();

		// ここで再発動
		if (isFinish == true) {
			swordArb->SetRollingActive(false);
		}
	}
}

void PlayerOperationComponent::Charge(const bool charge, const bool attack) {

	if (charge) {
		if (m_ChargeTime >= m_ChargeCompleteTime) {

			if(attack)
			{
				// チャージ攻撃発動
				std::cout << "チャージ攻撃発動" << std::endl;
				m_ChargeTime = 0.0f;
			}
			else
			std::cout << "チャージ完了" << std::endl;
		}
		else {
			std::cout << "チャージ中" << std::endl;
			m_ChargeTime += DeltaTime;
		}
	}
	else {
		m_ChargeTime = 0.0f;
	}
}