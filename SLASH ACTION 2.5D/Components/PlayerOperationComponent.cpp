#include "PlayerOperationComponent.h"
#include "RigidBodyComponent.h"
#include "Render2DComponent.h"
#include "Render3DComponent.h"
#include "JumpComponent.h"
#include "DoorFadeComponent.h"
#include "TransformComponent.h"
#include "Input/Input.h"
#include "TestSwordActionComponent.h"
#include "ArbitraryRotationComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include "RenderHpComponent.h"
#include "ChargePerformanceComponent.h"
#include "ColliderAttackComponent.h"
#include "VectorMoveComponent.h"
#include "EnemyDamageComponent.h"
#include "AttackOneTimeComponent.h"
#include "ChargeSlashComponent.h"
#include "SlashEffectComponent.h"
#include "RenderCharacterComponent.h"
#include "RenderLuminescenceBillboardComponent.h"
#include "TimeLineComponent.h"	// これがちゃんと動くのか明日テスト
#include "StretchingComponent.h"
#include "Mesh/SquareMesh.h"
#include "Manager/GameObjectManager.h"
#include "Manager/EventBusManager.h"
#include <iostream>

using namespace DirectX;

namespace {
	constexpr float DeltaTime = 0.016f; // 仮のデルタタイム
	constexpr float ChargeStartTime = 0.5f; // チャージエフェクト開始時間
	constexpr float ChargeSlashStopTime = 0.15f; // チャージスラッシュ終了時間
	constexpr float ChargeSlashEndTime = 0.45f; // チャージスラッシュ終了時間
	constexpr float DONT_MOVE_TIME = 0.3f; // ダメージを受けてから移動できない時間
	constexpr float DEFAULT_KNOCKBACK_POWER = 500.0f; // デフォルトのノックバックの力
	constexpr XMFLOAT3 MoveRightSpeed = XMFLOAT3(1.5f, 0.0f, 0.0f);
	constexpr XMFLOAT3 MoveLeftSpeed = XMFLOAT3(-1.5f, 0.0f, 0.0f);
	constexpr XMFLOAT3 DashRightSpeed = XMFLOAT3(5.0f, 0.0f, 0.0f);
	constexpr XMFLOAT3 DashLeftSpeed = XMFLOAT3(-5.0f, 0.0f, 0.0f);
	constexpr XMFLOAT3 DashRightSlashSpeed = XMFLOAT3(15.0f, 0.0f, 0.0f);	// 壁抜けしなくてちゃんと移動できる限界速度
	constexpr XMFLOAT3 DashLeftSlashSpeed = XMFLOAT3(-15.0f, 0.0f, 0.0f);	// 壁抜けしなくてちゃんと移動できる限界速度
}

PlayerOperationComponent::PlayerOperationComponent(GameObject& obj) :Component(obj) {

	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定

	m_KnockBackPower = DEFAULT_KNOCKBACK_POWER; // ノックバックの力を初期化

	m_listenerID_HitEvent = EventBusManager::Subscribe<DamageEvent>([&](const DamageEvent& e) {
		OnDamageHit(e);
		});
}

PlayerOperationComponent::~PlayerOperationComponent() {
	EventBusManager::Unsubscribe(m_listenerID_HitEvent);
}

void PlayerOperationComponent::TestProcess() {

	std::cout << "Start" << std::endl;
}

void PlayerOperationComponent::TestProcess2(float t) {

	std::cout << "TestProcess" << std::endl;
}

void PlayerOperationComponent::TestProcess3() {

	std::cout << "End" << std::endl;
}

// 更新処理
void PlayerOperationComponent::Update() {
	const bool keyLeft = Input::GetKeyPress(VK_A) || Input::GetKeyPress(VK_LEFT) || Input::GetButtonPress(XINPUT_LEFT);
	const bool keyRight = Input::GetKeyPress(VK_D) || Input::GetKeyPress(VK_RIGHT) || Input::GetButtonPress(XINPUT_RIGHT);

	FighterComponent* fighter = m_Object->GetComponent<FighterComponent>();
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	RenderCharacterComponent* rend = m_Object->GetComponent<RenderCharacterComponent>();
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
		m_BlinkingRecordTime += DeltaTime;

		if (m_BlinkingRecordTime >= 0.1f) {
			// 無敵状態なら透明にする
			rend->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
			m_BlinkingRecordTime = 0.0f;
		}
		else {
			rend->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	else {
		rend->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)); // 元に戻す
	}

	//if (rigid != nullptr) {

	//	// 後で減速の仕方を考える
	//	if (!keyLeft && !keyRight) {
	//		rigid->ReduceVelocity_X(0.5f);
	//	}
	//}

	GameObject* hpUI = GameObjectManager::GameObjectFindNameUI("hpUI");
	if (hpUI != nullptr) {

		RenderHpComponent* hpRend = hpUI->GetComponent<RenderHpComponent>();

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

	rigid->ReduceVelocity_X(0.5f); // 毎フレーム減速
}

void PlayerOperationComponent::ChangeState(const PlayerState& state) {

	PlayerState previousState = m_CurrentState;

	// 変更前に行う
	switch (m_CurrentState)
	{
	case PlayerState::NONE:
	{
		StretchingComponent* stretch = m_Object->GetComponent<StretchingComponent>();
		if (stretch != nullptr) {

			stretch->SetActiveFlag(false);
			stretch->SetHolderTime(0.0f);
			stretch->SetCurrentScale(0.0f);
		}
	}
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
	case PlayerState::CHARGE_SLASH:
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
	{
		StretchingComponent* stretch = m_Object->GetComponent<StretchingComponent>();
		if (stretch != nullptr) {

			stretch->SetActiveFlag(true);
			stretch->SetHolderTime(0.0f);
			stretch->SetCurrentScale(0.0f);
		}
	}
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
	case PlayerState::CHARGE_SLASH:
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

	const bool keyLeft = Input::GetKeyPress(VK_A) || Input::GetButtonPress(XINPUT_LEFT);
	const bool keyRight = Input::GetKeyPress(VK_D) || Input::GetButtonPress(XINPUT_RIGHT);
	const bool keyUp = Input::GetKeyPress(VK_SPACE) || Input::GetButtonPress(XINPUT_A);
	const bool keyBack = Input::GetKeyPress(VK_BACK) || Input::GetButtonPress(XINPUT_RIGHT_SHOULDER);
	const bool keyAttack = Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_X);
	const bool keyCharge = Input::GetKeyPress(VK_RETURN) || Input::GetButtonPress(XINPUT_X);
	const bool keyCAttack = Input::GetKeyRelease(VK_RETURN) || Input::GetButtonRelease(XINPUT_X);

	TestExtrusionJudgeComponent* extrusion = m_Object->GetComponent<TestExtrusionJudgeComponent>();
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	bool isGround = false;

	// ここで地面か空中か判断
	if (extrusion != nullptr) {
		isGround = extrusion->GetIsGround();
	}

	bool isMove = false;

	m_IsJump = false;
	if (keyUp) {
		m_IsJump = true;
	}

	m_IsMoveFlag = false; // 毎フレーム初期化	

	switch (m_CurrentState)
	{
	case PlayerState::NONE:

		if (keyLeft || keyRight || keyBack)
		{
			ChangeState(PlayerState::MOVE);
		}
		if (keyCharge)
		{
			//	ChangeState(PlayerState::CHARGE);
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
	case PlayerState::CHARGE_SLASH:
		// ここで一閃の動きにする
		FastChageSlash();
		break;
	case PlayerState::DAMAGE:

		if (rigid != nullptr) {

			m_KnockBackPower *= 0.9f; // ダメージを受けるたびにノックバックの力が増す
			if (m_KnockBackRightLeft == RightLeft::RIGHT) {
				// 左から攻撃されたら右に吹き飛ぶ
				rigid->AddVelocity_X(m_KnockBackPower);
			}
			else if (m_KnockBackRightLeft == RightLeft::LEFT) {
				// 右から攻撃されたら左に吹き飛ぶ
				rigid->AddVelocity_X(-m_KnockBackPower);
			}
		}

		m_DamageRecordTime += DeltaTime;
		if (m_DamageRecordTime >= DONT_MOVE_TIME) {
			m_DamageRecordTime = 0.0f;
			m_KnockBackPower = DEFAULT_KNOCKBACK_POWER;
			ChangeState(PlayerState::NONE);
		}
		break;
	case PlayerState::DEAD:
		break;
	default:
		break;
	}

	// チャージの処理
	Charge(keyCharge, keyCAttack);

	JumpComponent* jumpComp = m_Object->GetComponent<JumpComponent>();
	if (jumpComp != nullptr) {

		bool isNonDamage = true;
		if (m_CurrentState == PlayerState::DAMAGE) {
			isNonDamage = false;
		}
		jumpComp->SetJumpPress(keyUp && isNonDamage);
	}

	m_beforeMove = isMove;

	// どのキーも押されていないときは状態をNONEにする
	if (!keyLeft && !keyRight && !keyUp && !keyBack && !keyAttack && !keyCharge && !keyCAttack && m_CurrentState != PlayerState::DAMAGE) {
		ChangeState(PlayerState::NONE);
	}
}

void PlayerOperationComponent::Move(const bool right, const bool left, const bool dash, const bool attack, const bool isGround, bool* isMove) {

	//	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	TestSwordActionComponent* swordAction = m_WeaponObject->GetComponent<TestSwordActionComponent>();

	// 地面の攻撃は移動しない
	const bool isAction = swordAction->GetIsSwordAction();
	if (isAction && isGround) {
		return;
	}

	// 移動しない
	if (transform == nullptr) {
		ChangeState(PlayerState::NONE);
		return;
	}

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

			transform->AddPosition(DashLeftSpeed);
		}
		else if (m_CurrentRightLeft == RightLeft::RIGHT) {
			transform->AddPosition(DashRightSpeed);
		}
	}
	// 普通の移動
	else {

		// 移動しない
		if (right && left) {
			ChangeState(PlayerState::NONE);
			return;
		}

		if (left) {
			transform->AddPosition(MoveLeftSpeed);
		}
		if (right) {
			transform->AddPosition(MoveRightSpeed);
		}
	}

	// 移動する
	*isMove = true;
	m_IsMoveFlag = true;
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

	if (isGround == true) {
		// 地面の上での攻撃
		swordAction->SetIsAction(attack);
	}
	else {
		// 空中での攻撃
		swordAction->SetIsAction(attack, SlashPattern::AIR_SLASH);
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

	ChargePerformanceComponent* chargePerf = m_Object->GetComponent<ChargePerformanceComponent>();
	TestSwordActionComponent* swordAction = m_WeaponObject->GetComponent<TestSwordActionComponent>();

	if (chargePerf == nullptr) {
		return;
	}

	// チャージ攻撃発動
	if (attack && m_IsChargeComplete)
	{
		chargePerf->SetActiveFlag(false);
		chargePerf->ResetAllParticles();
		chargePerf->SetChargeCompleteFlag(false);
		swordAction->SetIsAction(attack, SlashPattern::CHARGE_SLASH);

		//ChangeState(PlayerState::CHARGE_SLASH);

		//CreateSlashEffect();

		CreateChargeSlash();
		m_ChargeTime = 0.0f;
		m_IsChargeComplete = false;
	}
	else if (attack && !m_IsChargeComplete)
	{
		StretchingComponent* stretch = m_Object->GetComponent<StretchingComponent>();
		if (stretch != nullptr) {

			stretch->SetActiveFlag(true);
			stretch->SetHolderTime(0.0f);
			stretch->SetCurrentScale(0.0f);
		}
	}


	// チャージ処理
	if (charge) {
		if (m_ChargeTime >= m_ChargeCompleteTime) {
			m_IsChargeComplete = true;
			// チャージ完了
			if (!attack) {
				chargePerf->SetChargeCompleteFlag(true);
			}
		}
		// まだチャージ中
		else {

			StretchingComponent* stretch = m_Object->GetComponent<StretchingComponent>();
			if (stretch != nullptr) {

				stretch->SetActiveFlag(false);
				stretch->SetHolderTime(0.0f);
				stretch->SetCurrentScale(0.0f);
			}

			// チャージエフェクト開始
			if (m_ChargeTime > ChargeStartTime) {
				chargePerf->SetActiveFlag(true);
			}
			m_ChargeTime += DeltaTime;
		}
	}
	else {
		m_IsChargeComplete = false;
		m_ChargeTime = 0.0f;
		chargePerf->SetActiveFlag(false);
		chargePerf->ResetAllParticles();
		chargePerf->SetChargeCompleteFlag(false);

	}
}

// チャージスラッシュエフェクト生成
void PlayerOperationComponent::CreateChargeSlash() {

	TransformComponent* playerTransform = m_Object->GetComponent<TransformComponent>();

	if (playerTransform == nullptr) {
		return;
	}

	XMFLOAT3 myPos = playerTransform->GetPosition();

	GameObject* chargeSlash = GameObjectManager::AddAbsFront("chargeSlash", "Effect");
	TransformComponent* slashTransform = chargeSlash->AddComponent<TransformComponent>();
	slashTransform->SetPosition(myPos);
	slashTransform->SetScale(XMFLOAT3(20.0f, 40.0f, 1.0f));
	//slashTransform->SetRotation(XMFLOAT3(45.0f,0.0f,0.0f));

	ColliderComponent* slashCollider = chargeSlash->AddComponent<ColliderComponent>();
	ColliderAttackComponent* colliderAttack = chargeSlash->AddComponent<ColliderAttackComponent>();
	VectorMoveComponent* slashMove = chargeSlash->AddComponent<VectorMoveComponent>();
	if (m_CurrentRightLeft == RightLeft::RIGHT) {
		slashMove->SetMoveDirection(XMFLOAT3(1.0f, 0.0f, 0.0f));
	}
	else {
		slashMove->SetMoveDirection(XMFLOAT3(-1.0f, 0.0f, 0.0f));
	}
	slashMove->SetMovePower(5.0f);
	AttackOneTimeComponent* slashAtk = chargeSlash->AddComponent<AttackOneTimeComponent>();

	EnemyDamageComponent* slashED = chargeSlash->AddComponent<EnemyDamageComponent>();
	FighterComponent* slashFT = chargeSlash->AddComponent<FighterComponent>();
	slashFT->SetAtk(10);
	slashFT->SetHp(50);

	ChargeSlashComponent* charge = chargeSlash->AddComponent<ChargeSlashComponent>();
	charge->SetRimitTime(1.0f);

	Render3DComponent* slashRender = chargeSlash->AddComponent<Render3DComponent>();
	slashRender->CreateMesh<SquareMesh>();
	slashRender->SetShader("Animation2DVS.hlsl", "unlitTexturePS.hlsl");
	slashRender->ChangeTexture("charge_slash1.png");
	slashRender->SetInversionFlag(m_CurrentRightLeft);
}

void PlayerOperationComponent::FastChageSlash() {


	// プレイヤーに一閃用の当たり判定用の子オブジェクトを装備させておくか
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();

	if (transform == nullptr || rigid == nullptr || m_WeaponObject == nullptr || m_ChargeSlashObject == nullptr)
	{
		return;
	}

	TestSwordActionComponent* swordAction = m_WeaponObject->GetComponent<TestSwordActionComponent>();
	if (swordAction == nullptr) {
		return;
	}

	AttackOneTimeComponent* attackComp = m_ChargeSlashObject->GetComponent<AttackOneTimeComponent>();
	if (attackComp == nullptr) {
		return;
	}

	m_ChargeSlashRecordTime += DeltaTime;

	// 一閃の待ち処理終了
	if (m_ChargeSlashRecordTime > ChargeSlashEndTime) {
		ChangeState(PlayerState::NONE);
		rigid->SetGravityFlag(true);
		m_ChargeSlashRecordTime = 0.0f;
		m_ChargeSlashObject->SetActiveState(ActiveState::ALL_STOP);
		attackComp->ClearAttackObjs();
	}
	// 一閃時間終了
	else if (m_ChargeSlashRecordTime > ChargeSlashStopTime) {
		swordAction->SetIsAction(false);
	}
	else {
		rigid->SetGravityFlag(false);
		rigid->ClearVelocity();

		if (m_CurrentRightLeft == RightLeft::LEFT) {

			transform->AddPosition(DashLeftSlashSpeed);
		}
		else if (m_CurrentRightLeft == RightLeft::RIGHT) {
			transform->AddPosition(DashRightSlashSpeed);
		}

		m_ChargeSlashObject->SetActiveState(ActiveState::ACTIVE);
	}
}

void PlayerOperationComponent::OnDamageHit(const DamageEvent& event) {

	const uint32_t targetID = m_Object->GetInstanceID();

	if (event.targetID != targetID) {
		return; // 自分宛じゃないなら無視
	}

	GameObject* attacker = GameObjectManager::GameObjectFindInstanceIDAll(event.attackerID);

	if (attacker == nullptr) {
		return;
	}

	TransformComponent* atTrans = attacker->GetComponent<TransformComponent>();
	TransformComponent* taTrans = m_Object->GetComponent<TransformComponent>();

	if (atTrans == nullptr || taTrans == nullptr) {
		return;
	}

	XMFLOAT3 atPos = atTrans->GetPosition();
	XMFLOAT3 taPos = taTrans->GetPosition();

	if (atPos.x < taPos.x) {

		m_KnockBackRightLeft = RightLeft::RIGHT;
	}
	else {

		m_KnockBackRightLeft = RightLeft::LEFT;
	}

	ChangeState(PlayerState::DAMAGE);
}

void PlayerOperationComponent::CreateSlashEffect() {

	TransformComponent* playerTransform = m_Object->GetComponent<TransformComponent>();

	GameObject* effect = GameObjectManager::AddAbsFront("swordEffect", "Effect");
	TransformComponent* effectTrans = effect->AddComponent<TransformComponent>();
	effectTrans->SetPosition(playerTransform->GetPosition());
	effectTrans->SetScale({ 45.0f,45.0f,5.0f });

	// 引き延ばしたり縮めたりするエフェクト
	SlashEffectComponent* slash = effect->AddComponent<SlashEffectComponent>();
	slash->SetRimitTime(0.5f);
	slash->SetSizeChange({ 0.0f, -5.0f });

	// エフェクト用のレンダー
	RenderLuminescenceBillboardComponent* render = effect->AddComponent<RenderLuminescenceBillboardComponent>();
	render->CreateMesh<SquareMesh>();
	render->SetShader("unlitTextureVS.hlsl", "unlitLuminescencePS.hlsl");
	render->SetColor({ 0.3f,0.8f,1.0f,1.0f });
	render->SetGlowPower(0.4f);
	render->SetGlowRadius(0.2f);
	render->SetEllipseScale({ 1.0f,1.0f });
}