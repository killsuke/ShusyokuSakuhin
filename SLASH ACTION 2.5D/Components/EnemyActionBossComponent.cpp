#include "EnemyActionBossComponent.h"
#include "Manager/GameObjectManager.h"
#include "Manager/EventBusManager.h"
#include "TransformComponent.h"
#include "Render2DComponent.h"
#include "Render3DComponent.h"
#include "RenderRingLuminescenceBillboardComponent.h"
#include "RenderTextureLuminescenceComponent.h"
#include "RenderLuminescenceBillboardComponent.h"
#include "FighterComponent.h"
#include "RigidBodyComponent.h"
#include "BulletComponent.h"
#include "AttackOneTimeComponent.h"
#include "AttackTimingComponent.h"
#include "PlayerDamageComponent.h"
#include "ColliderAttackComponent.h"
#include "ColliderDamageComponent.h"
#include "RenderHpComponent.h"
#include "JumpComponent.h"
#include "SoundComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include "TimeLineComponent.h"
#include "Mesh/SquareMesh.h"

using namespace DirectX;

namespace {

	constexpr float BULLET_TIMING = 0.5f;
	constexpr float BULLET_TIMING_EX = 0.35f;	// 強化バージョン
	constexpr float JUMP_MOVE_X = 2.0f;
	constexpr float TARGET_LENGTH = 20.0f;
	constexpr float MOVE_SPEED = 0.2f;
	constexpr float SCALE_SPEED = 0.05f;
	constexpr float ATTACK_TIME = 2.0f;
	constexpr float DAMAGE_TIME = 0.3f;
	constexpr int BARRIER_DURABILITY = 25;

	constexpr XMFLOAT2 DEFAULT_POSE{ 1.0f,1.0f };
	constexpr XMFLOAT2 MAKE_BARRIER_POSE{ 2.0f,1.0f };
	constexpr XMFLOAT2 JUMP_ATTACK_POSE{ 3.0f,1.0f };
	constexpr XMFLOAT2 ON_GROUND_ATTACK_POSE{ 4.0f,1.0f };
	constexpr XMFLOAT2 DAMAGE_POSE{ 5.0f,1.0f };
	constexpr XMFLOAT2 ANIM_CUT{ 5.0f,1.0f };
	constexpr XMFLOAT3 SHURIKEN_SCALE{ 6.5f,6.5f,1.0f };
}

EnemyActionBossComponent::EnemyActionBossComponent(GameObject& obj) :EnemyActionComponent(obj) {

	m_SortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ソート番号を設定

	// 防御用ボスのバリアの本体
	m_BossBarrier = GameObjectManager::AddAbsFront("boss_barrier_main", "Enemy");
	m_BossBarrier->SetChildAbsFrontFlag(true);
	TransformComponent* trans = m_BossBarrier->AddComponent<TransformComponent>();
	trans->SetLockScale(true);
	trans->SetLocalScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

	ColliderDamageComponent* collDamage = m_BossBarrier->AddComponent<ColliderDamageComponent>();
	collDamage->SetActiveColliderFlag(false); // 最初は当たり判定無効

	FighterComponent* fight = m_BossBarrier->AddComponent<FighterComponent>();
	fight->SetHp(BARRIER_DURABILITY);
	fight->SetAtk(1);

	Render2DComponent* rend2D = m_BossBarrier->AddComponent<Render2DComponent>();
	rend2D->CreateMesh<SquareMesh>();
	rend2D->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	rend2D->ChangeTexture("ring.png");

	// これはシェーダー側の調整が完了すれば実装する予定 ============
	//RenderTextureLuminescenceComponent* rendTex = m_BossBarrier->AddComponent<RenderTextureLuminescenceComponent>();
	//rendTex->CreateMesh<SquareMesh>();
	//rendTex->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	//rendTex->ChangeTexture("ring.png");
	//rendTex->SetGlowColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	// ========================================================

	RenderRingLuminescenceBillboardComponent* rend = m_BossBarrier->AddComponent<RenderRingLuminescenceBillboardComponent>();
	rend->CreateMesh<SquareMesh>();
	rend->SetShader("unlitLuminescenceVS.hlsl", "unlitRingLuminescencePS.hlsl");
	rend->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	rend->SetGlowPower(0.5f);
	rend->SetGlowRadius(0.3f);
	rend->SetEllipseScale({ 1.0f,1.0f });
	rend->SetRingRadius(0.5f);
	rend->SetRingWidth(0.2f);
	rend->SetExpand(1.2f);

	m_BossBarrier->SetActiveState(ActiveState::ALL_STOP);
	m_Object->SetChild(m_BossBarrier);

	m_BarrierList.fill(nullptr);

	// 攻撃用バリアオブジェクト作成
	for (int i = 0; i < BarrierCount; ++i) {
		GameObject* barrier = GameObjectManager::AddAbsFront("boss_barrier", "Effect");
		barrier->SetChildAbsFrontFlag(true);

		TransformComponent* trans = barrier->AddComponent<TransformComponent>();
		trans->SetRotationPattern(RotationPattern::REVOLUTION);
		trans->SetLockScale(true);

		trans->SetLocalScale(XMFLOAT3(7.0f, 7.0f, 7.0f));

		FighterComponent* fight = barrier->AddComponent<FighterComponent>();
		fight->SetHp(1);
		fight->SetAtk(2);

		ColliderAttackComponent* collAttack = barrier->AddComponent<ColliderAttackComponent>();
		collAttack->SetActiveColliderFlag(false); // 最初は当たり判定無効

		AttackTimingComponent* attack = barrier->AddComponent<AttackTimingComponent>();
		attack->SetCoolDownTime(0.5f);

		PlayerDamageComponent* damage = barrier->AddComponent<PlayerDamageComponent>();

		Render2DComponent* rend2D = barrier->AddComponent<Render2DComponent>();
		rend2D->CreateMesh<SquareMesh>();
		rend2D->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		rend2D->ChangeTexture("Shuriken.png");

		RenderRingLuminescenceBillboardComponent* rend = barrier->AddComponent<RenderRingLuminescenceBillboardComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->SetShader("unlitLuminescenceVS.hlsl", "unlitRingLuminescencePS.hlsl");
		rend->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		rend->SetGlowPower(0.5f);
		rend->SetGlowRadius(0.3f);
		rend->SetEllipseScale({ 1.0f,1.0f });
		rend->SetRingRadius(0.5f);
		rend->SetRingWidth(0.2f);
		rend->SetExpand(1.2f);

		barrier->SetActiveState(ActiveState::ALL_STOP);
		m_Object->SetChild(barrier);
		m_BarrierList[i] = barrier;
	}

	for (int i = 0; i < ShurikenCount; ++i) {

		GameObject* bullet = GameObjectManager::AddAbsFront("shuriken", "Effect");

		TransformComponent* trans = bullet->AddComponent<TransformComponent>();
		trans->SetPosition(XMFLOAT3());
		trans->SetScale(SHURIKEN_SCALE);
		RigidBodyComponent* rigid = bullet->AddComponent<RigidBodyComponent>();

		BulletComponent* bull = bullet->AddComponent<BulletComponent>();
		bull->SetDestroyOrStopFlag(false);	// 止まるようにする

		FighterComponent* fight = bullet->AddComponent<FighterComponent>();
		fight->SetAtk(5);
		fight->SetHp(10);

		AttackOneTimeComponent* atk = bullet->AddComponent<AttackOneTimeComponent>();
		PlayerDamageComponent* dmg = bullet->AddComponent<PlayerDamageComponent>();

		ColliderComponent* coll = bullet->AddComponent<ColliderComponent>();
		ColliderAttackComponent* collAttack = bullet->AddComponent<ColliderAttackComponent>();

		RenderTextureLuminescenceComponent* rendTex = bullet->AddComponent<RenderTextureLuminescenceComponent>();
		rendTex->CreateMesh<SquareMesh>();
		rendTex->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		rendTex->ChangeTexture("Shuriken.png");
		rendTex->SetGlowColor(DirectX::XMFLOAT4(0.0f, 0.0f, 0.8f, 1.0f));

		bullet->SetActiveState(ActiveState::ALL_STOP);
		m_ShurikenList[i] = bullet;
	}

	{
		GameObject* hp = GameObjectManager::AddUI("hpFrameUI", "HP_UI");
		TransformComponent* hpTrans = hp->AddComponent<TransformComponent>();
		hpTrans->SetPosition({ 570.0f, 150.0f, 0.0f });
		hpTrans->SetScale({ 45.0f, 150.0f, 1.0f });

		Render3DComponent* hpRender = hp->AddComponent<Render3DComponent>();
		hpRender->CreateMesh<SquareMesh>();
		hpRender->SetShader("unlitTextureVS2D.hlsl", "unlitTexturePS.hlsl");
		hpRender->ChangeTexture("boss_bar.png");
	}

	{
		m_HpBar = GameObjectManager::AddUI("hpUI_Boss", "HP_UI");
		TransformComponent* hpTrans = m_HpBar->AddComponent<TransformComponent>();
		hpTrans->SetPosition({ 570.0f, 120.0f, 0.0f });
		hpTrans->SetScale({ 30.0f, 1.0f, 1.0f });

		RenderHpComponent* hpBar = m_HpBar->AddComponent<RenderHpComponent>();
		hpBar->SetReferenceHPObj(*m_Object);
		hpBar->CreateMesh<SquareMesh>();
		hpBar->SetShader("OverVertexMoveVS.hlsl", "unlitTexturePS.hlsl");
		hpBar->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	}

	m_ListenerID_DamageEvent = EventBusManager::Subscribe<HitEvent>([&](const HitEvent& e) {
		DamageEvent(e);
		});
}

void EnemyActionBossComponent::Init() {

	SoundComponent* sound = m_Object->GetComponent<SoundComponent>();
	if (sound != nullptr) {
		sound->AddSoundLabel("boss_bullet");
	}
}

void EnemyActionBossComponent::Update() {

	GameObject* player = GameObjectManager::GameObjectFindName("Player");
	if (player == nullptr) {
		return;
	}

	TransformComponent* playTrans = player->GetComponent<TransformComponent>();
	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();
	Render2DComponent* rend = m_Object->GetComponent<Render2DComponent>();
	TestExtrusionJudgeComponent* testExtrude = m_Object->GetComponent<TestExtrusionJudgeComponent>();
	FighterComponent* fight = m_Object->GetComponent<FighterComponent>();

	if (playTrans == nullptr || myTrans == nullptr || rend == nullptr || testExtrude == nullptr || fight == nullptr) {
		return;
	}

	const int maxHp = fight->GetMaxHp();
	const int hp = fight->GetHp();

	m_RecordTime += TimeManager::GetFixedDeltaTime();

	const XMFLOAT3 playPos = playTrans->GetPosition();
	const XMFLOAT3 myPos = myTrans->GetPosition();

	if (myPos.x > playPos.x) {
		m_IsRightLeft = RightLeft::LEFT;
	}
	else {
		m_IsRightLeft = RightLeft::RIGHT;
	}
	rend->SetInversionFlag(m_IsRightLeft);

	const bool isGround = testExtrude->GetIsGround();

	StateUpdate(myPos, playPos, isGround);

	// 回転はこっち
	if (m_IsBarrier == true) {

		for (int i = 0; i < BarrierCount; ++i) {

			TransformComponent* barrierTrans = m_BarrierList[i]->GetComponent<TransformComponent>();
			if (barrierTrans != nullptr) {
				barrierTrans->AddLocalRotation({ 0.0f,0.0f,5.0f });
			}
		}
	}

	// バリアの耐久力チェック
	BarrierDurabilityCheck();

	{
		// HPバー処理
		RenderHpComponent* hpBar = m_HpBar->GetComponent<RenderHpComponent>();

		if (hpBar != nullptr) {

			// HPのカラー変更
			if (hp <= 0) {
				hpBar->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)); // 透明
			}
			else if (hp < (maxHp * 0.3f)) {
				hpBar->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)); // 赤色
			}
			else if (hp < (maxHp * 0.5f)) {
				hpBar->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)); // 黄色
			}
			else {
				hpBar->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)); // 緑色
			}
		}
	}
}

// ジャンプしながらプレイヤーに向かって弾を撃つ処理
void EnemyActionBossComponent::JumpBullet(const DirectX::XMFLOAT3& playPos, const DirectX::XMFLOAT3& myPos) {

	const XMVECTOR diff = XMLoadFloat3(&playPos) - XMLoadFloat3(&myPos);
	const float lengthSq = XMVectorGetX(XMVector3LengthSq(diff));

	XMFLOAT3 dirFloat3;
	if (lengthSq > 0.000001f) {
		// 正常に正規化できる
		XMVECTOR direction = XMVector3Normalize(diff);
		XMStoreFloat3(&dirFloat3, direction);
	}
	else {
		// 同じ位置 or ほぼ同じ位置
		dirFloat3 = XMFLOAT3();
		return;
	}

	SoundComponent* sound = m_Object->GetComponent<SoundComponent>();
	if (sound != nullptr) {
		sound->Play("boss_bullet");
	}

	// 手裏剣の数が上限に達している場合はこれ以上撃たない
	if(m_ShurikenCount > ShurikenCount - 1){
		return;
	}

	// 弾作成処理

	if (m_ShurikenList[m_ShurikenCount] == nullptr) {
		return;
	}

	m_ShurikenList[m_ShurikenCount]->SetActiveState(ActiveState::ACTIVE);

	BulletComponent* bull = m_ShurikenList[m_ShurikenCount]->GetComponent<BulletComponent>();
	TransformComponent* trans = m_ShurikenList[m_ShurikenCount]->GetComponent<TransformComponent>();
	AttackOneTimeComponent* atk = m_ShurikenList[m_ShurikenCount]->GetComponent<AttackOneTimeComponent>();

	if (bull == nullptr || trans == nullptr || atk == nullptr) {
		return;
	}

	atk->ClearAttackObjs();	// 攻撃対象をリセット

	trans->SetPosition(myPos);

	// 向きや飛ばす強さの設定

	bull->SetFiringVector(dirFloat3);
	bull->SetFiringSpeed(150.0f);
	bull->SetRimitTime(3.0f);
	bull->SetRotateFlag(true);

	const float diffX = playPos.x - myPos.x;

	if (diffX < 0.0f) {

		bull->SetRotateValue({ 0.0f,0.0f,10.0f });
	}
	else {

		bull->SetRotateValue({ 0.0f,0.0f,-10.0f });
	}

	m_ShurikenCount++;
}

// 状態の変更処理
void EnemyActionBossComponent::ChangeState(const BossActionState& state) {

	JumpComponent* jump = m_Object->GetComponent<JumpComponent>();
	TransformComponent* trans = m_Object->GetComponent<TransformComponent>();

	if (jump == nullptr || trans == nullptr) {
		return;
	}

	// 同じ状態に変更しようとした場合は何もしない
	if (m_CurrentState == state) {
		return;
	}

	// 終了時の処理
	switch (m_CurrentState)
	{
	case BossActionState::DEFAULT:
		break;
	case BossActionState::JUMP_SHOOTING:

		m_ShurikenCount = 0;	// ジャンプ攻撃で撃つ手裏剣の数をリセット
		m_RecordTime = 0.0f;

		jump->SetJumpPress(false);
		break;
	case BossActionState::BARRIER:
		break;
	case BossActionState::DAMAGE:
		break;
	case BossActionState::MAX:
		break;
	default:
		break;
	}

	m_CurrentState = state;

	// 開始時の処理
	switch (m_CurrentState)
	{
	case BossActionState::DEFAULT:

		break;
	case BossActionState::JUMP_SHOOTING:

		jump->SetJumpPress(true);

		// 向いている方向に対してジャンプしながら弾を撃つ
		if (m_IsRightLeft == RightLeft::LEFT) {

			m_MoveDir = -1.0f;
			m_MoveDir *= JUMP_MOVE_X;
		}
		else if (m_IsRightLeft == RightLeft::RIGHT) {

			m_MoveDir = 1.0f;
			m_MoveDir *= JUMP_MOVE_X;
		}
		break;
	case BossActionState::BARRIER:

		m_BossBarrier->SetActiveState(ActiveState::ACTIVE);

		{
			ColliderDamageComponent* collDamage = m_BossBarrier->GetComponent<ColliderDamageComponent>();
			collDamage->SetActiveColliderFlag(true);

			RenderRingLuminescenceBillboardComponent* rend = m_BossBarrier->GetComponent<RenderRingLuminescenceBillboardComponent>();
			rend->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		}

		// バリア展開処理をここに書く
		for (int i = 0; i < BarrierCount; ++i) {
			if (m_BarrierList[i] != nullptr) {
				m_BarrierList[i]->SetActiveState(ActiveState::ACTIVE);
				{
					// 当たり判定を有効化
					ColliderAttackComponent* collAttack = m_BarrierList[i]->GetComponent<ColliderAttackComponent>();
					if (collAttack != nullptr) {
						collAttack->SetActiveColliderFlag(true);
					}
				}
			}
		}

		// ボス本体のダメージ用当たり判定を切る
		{
			ColliderDamageComponent* collDamage = m_Object->GetComponent<ColliderDamageComponent>();
			if (collDamage != nullptr) {
				collDamage->SetActiveColliderFlag(false);
			}
		}

		// ここでバリア用のオブジェクトを作成
		// ボスのダメージを受ける方の当たり判定を切る

		m_IsBarrier = true;
		break;

	case BossActionState::DAMAGE:

	{

	}

	break;
	case BossActionState::MAX:
		break;
	default:
		break;
	}
}

// バリアのリセット処理
void EnemyActionBossComponent::ResetBarriers(FighterComponent& fight) {

	// バリアの耐久力回復＆当たり判定無効化
	ColliderDamageComponent* collDamage = m_BossBarrier->GetComponent<ColliderDamageComponent>();
	if (collDamage != nullptr) {
		collDamage->SetActiveColliderFlag(false);
	}
	fight.SetHp(BARRIER_DURABILITY);
	m_BossBarrier->SetActiveState(ActiveState::ALL_STOP);

	TransformComponent* bossBarrierTrans = m_BossBarrier->GetComponent<TransformComponent>();
	if (bossBarrierTrans != nullptr) {
		bossBarrierTrans->SetLocalScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	}

	m_LengthCount = 0.0f;
	m_IsBarrier = false;

	// 攻撃用バリア
	for (int i = 0; i < BarrierCount; ++i) {
		if (m_BarrierList[i] != nullptr) {
			m_BarrierList[i]->SetActiveState(ActiveState::ALL_STOP);
			{
				// 回転するバリアの位置戻し
				TransformComponent* barrierTrans = m_BarrierList[i]->GetComponent<TransformComponent>();
				if (barrierTrans != nullptr) {
					barrierTrans->SetLocalPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
				}

				// 当たり判定を無効化
				ColliderAttackComponent* collAttack = m_BarrierList[i]->GetComponent<ColliderAttackComponent>();
				if (collAttack != nullptr) {
					collAttack->SetActiveColliderFlag(false);
				}
			}
		}
	}

	// ボス本体の当たり判定を有効化
	ColliderDamageComponent* myCollDamage = m_Object->GetComponent<ColliderDamageComponent>();
	if (myCollDamage != nullptr) {
		myCollDamage->SetActiveColliderFlag(true);
	}
}

// バリアの耐久力チェック処理
void EnemyActionBossComponent::BarrierDurabilityCheck() {

	if (m_BossBarrier == nullptr) {
		return;
	}
	// バリア耐久値チェック
	FighterComponent* fight = m_BossBarrier->GetComponent<FighterComponent>();
	if (fight == nullptr) {
		return;
	}

	const int hp = fight->GetHp();

	// 耐久力が0以下になったらバリアをリセット
	if (hp <= 0) {

		ResetBarriers(*fight);
	}
	// 耐久力が半分以下になったらバリアの色を赤色に変更
	else if (hp <= (BARRIER_DURABILITY / 2)) {

		RenderRingLuminescenceBillboardComponent* rend = m_BossBarrier->GetComponent<RenderRingLuminescenceBillboardComponent>();
		if (rend != nullptr) {
			rend->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
}

// 状態に応じた処理
void EnemyActionBossComponent::StateUpdate(const DirectX::XMFLOAT3& myPos, const DirectX::XMFLOAT3& playPos, const bool isGround) {

	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();
	Render2DComponent* rend = m_Object->GetComponent<Render2DComponent>();

	if (myTrans == nullptr || rend == nullptr) {
		return;
	}

	Mesh* mesh = rend->GetMesh();

	if (mesh == nullptr) {
		return;
	}

	switch (m_CurrentState)
	{
	case BossActionState::DEFAULT:

		mesh->SetInitialCut(ANIM_CUT);
		mesh->SetCutNum(DEFAULT_POSE);

		if (m_RecordTime > ATTACK_TIME) {
			ChangeState(BossActionState::JUMP_SHOOTING);
			m_RecordTime = 0.0f;
		}

		break;
	case BossActionState::JUMP_SHOOTING:

		// 地上にいるかどうかで攻撃モーションを切り替える
		if (isGround == true) {

			mesh->SetCutNum(ON_GROUND_ATTACK_POSE);
		}
		else {

			mesh->SetCutNum(JUMP_ATTACK_POSE);
		}

		m_RecordTime_Jump += TimeManager::GetFixedDeltaTime();

		// 弾発射判定
		if (m_RecordTime_Jump > BULLET_TIMING) {

			JumpBullet(playPos, myPos);
			m_RecordTime_Jump = 0.0f;
		}

		// ジャンプ移動終了判定
		if (isGround == true && m_RecordTime_Jump <= 0.001f) {

			ChangeState(BossActionState::BARRIER);
			break;
		}

		myTrans->AddPosition({ m_MoveDir,0.0f,0.0f });
		break;
	case BossActionState::BARRIER:
	{
		TransformComponent* bossBarrierTrans = m_BossBarrier->GetComponent<TransformComponent>();

		if (bossBarrierTrans == nullptr) {

			return;
		}

		// 移動はこっち
		for (int i = 0; i < BarrierCount; ++i) {

			TransformComponent* barrierTrans = m_BarrierList[i]->GetComponent<TransformComponent>();

			if (barrierTrans == nullptr) {

				return;
			}

			// バリアの展開処理
			// 一定の距離までは移動と拡大を行う
			if (m_LengthCount < TARGET_LENGTH) {

				bossBarrierTrans->AddLocalScale({ SCALE_SPEED,SCALE_SPEED,0.0f });

				if (i == 0) {
					barrierTrans->AddLocalPosition({ 0.0f,MOVE_SPEED,0.0f });
				}
				else if (i == 1) {
					barrierTrans->AddLocalPosition({ MOVE_SPEED,0.0f,0.0f });
				}
				else if (i == 2) {
					barrierTrans->AddLocalPosition({ 0.0f,-MOVE_SPEED,0.0f });
				}
				else if (i == 3) {
					barrierTrans->AddLocalPosition({ -MOVE_SPEED,0.0f,0.0f });
				}

				mesh->SetCutNum(MAKE_BARRIER_POSE);
			}
			else {

				// 最後に回転を開始
				ChangeState(BossActionState::DEFAULT);
				break;
			}
		}

		m_LengthCount += MOVE_SPEED;
	}
	break;
	case BossActionState::DAMAGE:

		mesh->SetCutNum(DAMAGE_POSE);

		m_RecordTime_Damage += TimeManager::GetFixedDeltaTime();

		// ダメージモーション終了判定
		if (m_RecordTime_Damage > DAMAGE_TIME) {
			ChangeState(BossActionState::DEFAULT);
			m_RecordTime_Damage = 0.0f;
		}

		break;
	case BossActionState::MAX:
		break;
	default:
		break;
	}
}

// 死亡時のアニメーション処理
void EnemyActionBossComponent::DeadAnimation() {

	Render2DComponent* rend = m_Object->GetComponent<Render2DComponent>();

	if (rend == nullptr) {
		return;
	}

	Mesh* mesh = rend->GetMesh();

	if (mesh == nullptr) {
		return;
	}

	mesh->SetCutNum(DAMAGE_POSE);

	// バリアオブジェクトの非表示

	if (m_BossBarrier == nullptr) {
		return;
	}

	m_BossBarrier->SetActiveState(ActiveState::ALL_STOP);

	// 攻撃用バリアの非表示

	for (GameObject* barrier : m_BarrierList) {

		if (barrier != nullptr) {

			barrier->SetActiveState(ActiveState::ALL_STOP);
		}
	}
}

// ダメージを受けたときの処理
void EnemyActionBossComponent::DamageEvent(const HitEvent& e) {

	const uint32_t targetID = m_Object->GetInstanceID();

	if (e.targetID != targetID) {
		return;
	}

	if (m_CurrentState == BossActionState::DEFAULT) {

		ChangeState(BossActionState::DAMAGE);
	}
}