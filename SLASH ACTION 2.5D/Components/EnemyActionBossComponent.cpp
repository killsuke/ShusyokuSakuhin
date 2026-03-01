#include "EnemyActionBossComponent.h"
#include "Manager/GameObjectManager.h"
#include "TransformComponent.h"
#include "Render2DComponent.h"
#include "Render3DComponent.h"
#include "RenderRingLuminescenceBillboardComponent.h"
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
#include "TestExtrusionJudgeComponent.h"
#include "Mesh/SquareMesh.h"

using namespace DirectX;

namespace {

	constexpr float BulletTiming = 0.25f;
	constexpr float JumpMoveX = 2.0f;
	constexpr float TargetLength = 20.0f;
	constexpr float MoveSpeed = 0.2f;
	constexpr float ScaleSpeed = 0.05f;
	constexpr int BarrierDurability = 35;
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
	fight->SetHp(BarrierDurability);
	fight->SetAtk(1);

	Render2DComponent* rend2D = m_BossBarrier->AddComponent<Render2DComponent>();
	rend2D->CreateMesh<SquareMesh>();
	rend2D->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	rend2D->ChangeTexture("assets/texture/ring.png");

	RenderRingLuminescenceBillboardComponent* rend = m_BossBarrier->AddComponent<RenderRingLuminescenceBillboardComponent>();
	rend->CreateMesh<SquareMesh>();
	rend->SetShader("ShaderResource/unlitLuminescenceVS.hlsl", "ShaderResource/unlitRingLuminescencePS.hlsl");
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

		//	ColliderComponent* coll = barrier->AddComponent<ColliderComponent>();
		ColliderAttackComponent* collAttack = barrier->AddComponent<ColliderAttackComponent>();
		collAttack->SetActiveColliderFlag(false); // 最初は当たり判定無効

		AttackTimingComponent* attack = barrier->AddComponent<AttackTimingComponent>();
		attack->SetCoolDownTime(0.5f);

		PlayerDamageComponent* damage = barrier->AddComponent<PlayerDamageComponent>();

		Render2DComponent* rend2D = barrier->AddComponent<Render2DComponent>();
		rend2D->CreateMesh<SquareMesh>();
		rend2D->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		rend2D->ChangeTexture("assets/texture/ring.png");

		RenderRingLuminescenceBillboardComponent* rend = barrier->AddComponent<RenderRingLuminescenceBillboardComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->SetShader("ShaderResource/unlitLuminescenceVS.hlsl", "ShaderResource/unlitRingLuminescencePS.hlsl");
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

	{
		GameObject* hp = GameObjectManager::AddUI("hpFrameUI", "HP_UI");
		TransformComponent* hpTrans = hp->AddComponent<TransformComponent>();
		hpTrans->SetPosition({ 570.0f, 150.0f, 0.0f });
		hpTrans->SetScale({ 45.0f, 150.0f, 1.0f });

		Render3DComponent* hpRender = hp->AddComponent<Render3DComponent>();
		hpRender->CreateMesh<SquareMesh>();
		hpRender->SetShader("ShaderResource/unlitTextureVS2D.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		hpRender->ChangeTexture("assets/texture/boss_bar.png");
	}

	{
		m_HpBar = GameObjectManager::AddUI("hpUI", "HP_UI");
		TransformComponent* hpTrans = m_HpBar->AddComponent<TransformComponent>();
		hpTrans->SetPosition({ 570.0f, 120.0f, 0.0f });
		hpTrans->SetScale({ 30.0f, 1.0f, 1.0f });

		RenderHpComponent* hpBar = m_HpBar->AddComponent<RenderHpComponent>();
		hpBar->SetReferenceHPObj(*m_Object);
		hpBar->CreateMesh<SquareMesh>();
		hpBar->SetShader("ShaderResource/OverVertexMoveVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		hpBar->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
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

	if (playTrans == nullptr || myTrans == nullptr || rend == nullptr || testExtrude == nullptr) {
		return;
	}

	XMFLOAT3 playPos = playTrans->GetPosition();
	XMFLOAT3 myPos = myTrans->GetPosition();

	if (myPos.x > playPos.x) {
		m_IsRightLeft = RightLeft::LEFT;
	}
	else {
		m_IsRightLeft = RightLeft::RIGHT;
	}
	rend->SetInversionFlag(m_IsRightLeft);

	const bool isGround = testExtrude->GetIsGround();

	switch (m_CurrentState)
	{
	case BossActionState::DEFAULT:

		if (m_recordTime > 2.0f) {
			ChangeState(BossActionState::JUMP_SHOOTING);
			m_recordTime = 0.0f;
		}

		break;
	case BossActionState::JUMP_SHOOTING:

		m_RecordTime1 += m_deltaTime;
		m_recordTime += m_deltaTime;

		// 弾発射判定
		if (m_RecordTime1 > BulletTiming) {

			JumpBullet(playPos, myPos);
			m_RecordTime1 = 0.0f;
		}

		// ジャンプ移動終了判定
		if (isGround == true && m_RecordTime1 == 0.0f) {

			ChangeState(BossActionState::BARRIER);
			break;
		}

		myTrans->AddPosition({ m_MoveDir,0.0f,0.0f });
		break;
	case BossActionState::BARRIER:
	{
		TransformComponent* bossBarrierTrans = m_BossBarrier->GetComponent<TransformComponent>();

		if (bossBarrierTrans != nullptr) {

			// 移動はこっち
			for (int i = 0; i < BarrierCount; ++i) {

				TransformComponent* barrierTrans = m_BarrierList[i]->GetComponent<TransformComponent>();
				if (barrierTrans != nullptr) {

					if (m_LengthCount < TargetLength) {

						bossBarrierTrans->AddLocalScale({ ScaleSpeed,ScaleSpeed,0.0f });

						if (i == 0) {
							barrierTrans->AddLocalPosition({ 0.0f,MoveSpeed,0.0f });
						}
						else if (i == 1) {
							barrierTrans->AddLocalPosition({ MoveSpeed,0.0f,0.0f });
						}
						else if (i == 2) {
							barrierTrans->AddLocalPosition({ 0.0f,-MoveSpeed,0.0f });
						}
						else if (i == 3) {
							barrierTrans->AddLocalPosition({ -MoveSpeed,0.0f,0.0f });
						}
					}
					else {

						//m_LengthCount = 0.0f;
						// 最後に回転を開始
						ChangeState(BossActionState::DEFAULT);
						break;
					}
				}
			}

			m_LengthCount += MoveSpeed;
		}
	}
	break;
	case BossActionState::MAX:
		break;
	default:
		break;
	}

	// 回転はこっち
	if (m_IsBarrier == true) {

		for (int i = 0; i < BarrierCount; ++i) {

			TransformComponent* barrierTrans = m_BarrierList[i]->GetComponent<TransformComponent>();
			if (barrierTrans != nullptr) {
				barrierTrans->AddLocalRotation({ 0.0f,0.0f,5.0f });
			}
		}
	}

	{
		// バリア耐久値チェック
		FighterComponent* fight = m_BossBarrier->GetComponent<FighterComponent>();
		if (m_BossBarrier == nullptr) {
			return;
		}
		if (fight == nullptr) {
			return;
		}

		const int hp = fight->GetHp();

		if (hp <= 0) {

			ResetBarriers(*fight);
		}
		else if (hp <= (BarrierDurability / 2)) {

			RenderRingLuminescenceBillboardComponent* rend = m_BossBarrier->GetComponent<RenderRingLuminescenceBillboardComponent>();
			rend->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	{
		// HPバー処理
		FighterComponent* fight = m_Object->GetComponent<FighterComponent>();
		RenderHpComponent* hpBar = m_HpBar->GetComponent<RenderHpComponent>();

		if (fight != nullptr && hpBar != nullptr) {

			const int maxHp = fight->GetMaxHp();
			const int hp = fight->GetHp();

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

	m_recordTime += m_deltaTime;
}

void EnemyActionBossComponent::JumpBullet(const DirectX::XMFLOAT3& playPos, const DirectX::XMFLOAT3& myPos) {

	XMVECTOR diff = XMLoadFloat3(&playPos) - XMLoadFloat3(&myPos);
	float lengthSq = XMVectorGetX(XMVector3LengthSq(diff));

	XMFLOAT3 dirFloat3;
	if (lengthSq > 0.000001f) {
		// 正常に正規化できる
		XMVECTOR direction = XMVector3Normalize(diff);
		XMStoreFloat3(&dirFloat3, direction);
	}
	else {
		// 同じ位置 or ほぼ同じ位置
		dirFloat3 = XMFLOAT3(0.0f, 0.0f, 0.0f);
		return;
	}


	// ここが弾作成処理
	GameObject* bullet = GameObjectManager::AddAbsFront("bullet", "Effect");

	TransformComponent* trans = bullet->AddComponent<TransformComponent>();
	trans->SetPosition({ myPos.x + 10.0f,myPos.y,myPos.z });
	trans->SetScale({ 5.0f,5.0f,1.0f });
	RigidBodyComponent* rigid = bullet->AddComponent<RigidBodyComponent>();
	BulletComponent* bull = bullet->AddComponent<BulletComponent>();
	bull->SetFiringVector(dirFloat3);
	bull->SetFiringSpeed(150.0f);
	bull->SetRimitTime(3.0f);
	FighterComponent* fight = bullet->AddComponent<FighterComponent>();
	fight->SetAtk(5);
	fight->SetHp(10);
	AttackOneTimeComponent* atk = bullet->AddComponent<AttackOneTimeComponent>();
	PlayerDamageComponent* dmg = bullet->AddComponent<PlayerDamageComponent>();

	ColliderComponent* coll = bullet->AddComponent<ColliderComponent>();
	ColliderAttackComponent* collAttack = bullet->AddComponent<ColliderAttackComponent>();

	Render2DComponent* rend2D = bullet->AddComponent<Render2DComponent>();
	rend2D->CreateMesh<SquareMesh>();
	rend2D->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	rend2D->ChangeTexture("assets/texture/ring.png");

	RenderRingLuminescenceBillboardComponent* rend = bullet->AddComponent<RenderRingLuminescenceBillboardComponent>();

	rend->CreateMesh<SquareMesh>();
	rend->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitRingLuminescencePS.hlsl");
	rend->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	rend->SetGlowPower(1.0f);
	rend->SetGlowRadius(0.5f);
	rend->SetEllipseScale({ 1.0f,1.0f });
	rend->SetRingRadius(0.8f);
	rend->SetRingWidth(0.2f);
}

void EnemyActionBossComponent::ChangeState(const BossActionState& state) {

	JumpComponent* jump = m_Object->GetComponent<JumpComponent>();
	TransformComponent* trans = m_Object->GetComponent<TransformComponent>();

	if (jump == nullptr || trans == nullptr) {
		return;
	}

	// 終了時の処理
	switch (m_CurrentState)
	{
	case BossActionState::DEFAULT:
		break;
	case BossActionState::JUMP_SHOOTING:

		jump->SetJumpPress(false);
		break;
	case BossActionState::BARRIER:
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

		m_recordTime = 0.0f;
		break;
	case BossActionState::JUMP_SHOOTING:

		jump->SetJumpPress(true);

		// 向いている方向に対してジャンプしながら弾を撃つ
		if (m_IsRightLeft == RightLeft::LEFT) {

			m_MoveDir = -1.0f;
			m_MoveDir *= JumpMoveX;
		}
		else if (m_IsRightLeft == RightLeft::RIGHT) {

			m_MoveDir = 1.0f;
			m_MoveDir *= JumpMoveX;
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
	case BossActionState::MAX:
		break;
	default:
		break;
	}
}

void EnemyActionBossComponent::ResetBarriers(FighterComponent& fight) {

	// バリアの耐久力回復＆当たり判定無効化
	ColliderDamageComponent* collDamage = m_BossBarrier->GetComponent<ColliderDamageComponent>();
	if (collDamage != nullptr) {
		collDamage->SetActiveColliderFlag(false);
	}
	fight.SetHp(BarrierDurability);
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
