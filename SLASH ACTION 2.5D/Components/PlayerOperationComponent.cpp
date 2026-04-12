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
#include "TimeLineComponent.h"
#include "StretchingComponent.h"
#include "ColliderDamageComponent.h"
#include "RenderTextureLuminescenceComponent.h"
#include "DeadRingComponent.h"
#include "Mesh/SquareMesh.h"
#include "Manager/GameObjectManager.h"
#include "Manager/EventBusManager.h"
#include "Manager/TimeManager.h"
#include <iostream>

using namespace DirectX;

// タイムラインある？あとプレイヤーの死亡処理作成

namespace {
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

	m_listenerID_FallDamageEvent = EventBusManager::Subscribe<FallDamageEvent>([&](const FallDamageEvent& e) {
		OnDamageFallHit(e);
		});
}

PlayerOperationComponent::~PlayerOperationComponent() {
	EventBusManager::Unsubscribe(m_listenerID_HitEvent);
	EventBusManager::Unsubscribe(m_listenerID_FallDamageEvent);
}

// 更新処理
void PlayerOperationComponent::Update() {

	const bool keyLeft = Input::GetKeyPress(VK_A) || Input::GetKeyPress(VK_LEFT) || Input::GetButtonPress(XINPUT_LEFT);
	const bool keyRight = Input::GetKeyPress(VK_D) || Input::GetKeyPress(VK_RIGHT) || Input::GetButtonPress(XINPUT_RIGHT);
	const float deltaTime = TimeManager::GetFixedDeltaTime();

	FighterComponent* fighter = m_Object->GetComponent<FighterComponent>();
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	RenderCharacterComponent* rend = m_Object->GetComponent<RenderCharacterComponent>();
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (fighter == nullptr || rigid == nullptr)
	{
		return;
	}

	// 死亡フラグが立っていたら状態をDEADにする
	if (fighter->GetDeadFlag() == true) {

		ChangeState(PlayerState::DEAD);
	}

	rend->SetInversionFlag(m_CurrentRightLeft); // 向きに合わせて反転

	if (fighter->GetInvincibleFlag() == true) {
		// 無敵状態の時間を計測
		m_BlinkingRecordTime += deltaTime;

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

	if (state == m_CurrentState) {
		return; // 同じ状態に変更しようとしたら何もしない
	}

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

	{
		TimeLineComponent* timeLine = m_Object->GetComponent<TimeLineComponent>();
		if (timeLine != nullptr) {

			timeLine->AddPointDelayEvent(0.0f, this, [this]() { DeadCameraShake(); });
			timeLine->AddContinuousDelayEvent(0.2f, this, [this]() {DeadProcess(); });
			timeLine->AddPointDelayEvent(0.2f, this, [this]() { CreateDeadRing(); });
			timeLine->AddPointDelayEvent(0.6f, this, [this]() { CreateDeadRing(); });
			timeLine->AddPointDelayEvent(1.0f, this, [this]() { CreateDeadRing(); });
			timeLine->AddPointDelayEvent(2.5f, this, [this]() { ChangeResult(); });
		}

		RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
		TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
		ColliderComponent* collider = m_Object->GetComponent<ColliderComponent>();
		ColliderDamageComponent* collDamage = m_Object->GetComponent<ColliderDamageComponent>();

		if (rigid == nullptr || transform == nullptr || collider == nullptr || collDamage == nullptr) {
			return;
		}

		rigid->SetVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f)); // 死んだら動かないようにする
		rigid->SetGravityFlag(false); // 重力も無効にする
		rigid->SetActiveFlag(false); // 物理判定も無効にする
		transform->SetActiveFlag(false); // トランスフォームも無効にする
		collider->SetActiveColliderFlag(false); // 当たり判定も無効にする
		collDamage->SetActiveColliderFlag(false); // ダメージの当たり判定も無効にする
	}
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
	const float deltaTime = TimeManager::GetFixedDeltaTime();

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

		m_DamageRecordTime += deltaTime;
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
	//Charge(keyCharge, keyCAttack);

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
	if (!keyLeft && !keyRight && !keyUp && !keyBack && !keyAttack && !keyCharge && !keyCAttack && m_CurrentState != PlayerState::DAMAGE && m_CurrentState != PlayerState::DEAD) {
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
	const float deltaTime = TimeManager::GetFixedDeltaTime();

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
			m_ChargeTime += deltaTime;
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
	const float deltaTime = TimeManager::GetFixedDeltaTime();

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

	m_ChargeSlashRecordTime += deltaTime;

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

	ChangeState(PlayerState::DAMAGE);

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
}

void PlayerOperationComponent::OnDamageFallHit(const FallDamageEvent& event) {
	const uint32_t targetID = m_Object->GetInstanceID();

	if (event.targetID != targetID) {
		return; // 自分宛じゃないなら無視
	}

	ChangeState(PlayerState::DAMAGE);
	m_KnockBackRightLeft = RightLeft::NONE; // 落下ダメージはノックバックなし
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

void PlayerOperationComponent::DeadCameraShake() {

	GameObject* camera = GameObjectManager::GameObjectFindName("camera");
	GameObject* sword = GameObjectManager::GameObjectFindName("sword");

	if (camera == nullptr || sword == nullptr) {
		return;
	}

	CameraShakeComponent* camShake = camera->GetComponent<CameraShakeComponent>();
	if (camShake == nullptr) {
		return;
	}

	// 止めるゲームオブジェクトを選出しておく

	camShake->ShakingPreparation(15.0f, 4.5f, 0.2f);
	camShake->SetShakeType(ShakeType::RANDOM_2D);
	sword->SetActiveState(ActiveState::ALL_STOP);

	// 死亡演出に集中させるために一度止める
	std::vector<GameObject*> stopObjects = GameObjectManager::GameObjectFindTags("Enemy", "Effect", "SkyDome", "Bullets");
	for (GameObject* obj : stopObjects) {

		obj->SetActiveState(ActiveState::UPDATE_STOP);
	}
}

// 死亡処理。時間経過で徐々に透明にしていく
void PlayerOperationComponent::DeadProcess() {

	RenderCharacterComponent* rend = m_Object->GetComponent<RenderCharacterComponent>();

	if (rend == nullptr) {
		return;
	}

	rend->AddColor_A(m_ColorA);
	m_ColorA -= 0.04f;
}

// 死亡エフェクトのリングを生成する
void PlayerOperationComponent::CreateDeadRing() {

	TransformComponent* playerTransform = m_Object->GetComponent<TransformComponent>();

	if (playerTransform == nullptr) {
		return;
	}

	const XMFLOAT3 myPos = playerTransform->GetPosition();

	// 死亡エフェクトのリングを生成
	GameObject* ring = GameObjectManager::AddAbsFront("deadRing", "Effect_Dead");
	TransformComponent* ringTrans = ring->AddComponent<TransformComponent>();
	// プレイヤーの位置に生成
	ringTrans->SetPosition(XMFLOAT3(myPos.x, myPos.y, myPos.z - 1.0f));
	DeadRingComponent* ringComp = ring->AddComponent<DeadRingComponent>();
	ringComp->SetScaleUpSpeed(5.0f);
	ringComp->SetDeleteSpeed(0.02f);
	RenderTextureLuminescenceComponent* ringRender = ring->AddComponent<RenderTextureLuminescenceComponent>();
	ringRender->CreateMesh<SquareMesh>();
	ringRender->SetShader("unlitTextureVS.hlsl", "unlitLuminescencePS.hlsl");
	ringRender->ChangeTexture("dead_ring.png");
	ringRender->SetColor(XMFLOAT4(0.8f, 0.8f, 1.0f, 1.0f));
	ringRender->SetGlowColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
}

// 結果画面に移行する
void PlayerOperationComponent::ChangeResult() {

	GameObject* fadeObj = GameObjectManager::GameObjectFindNameUI("fade");

	if (fadeObj == nullptr) {
		return;
	}

	DoorFadeComponent* fade = fadeObj->GetComponent<DoorFadeComponent>();

	if (fade == nullptr) {
		return;
	}

	fade->SetWinLoseFlag(false);
	fade->SetNextSceneName("ResultScene");
	fade->SetBootDoor(true);
}