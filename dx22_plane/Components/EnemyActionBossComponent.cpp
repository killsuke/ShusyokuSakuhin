#include "EnemyActionBossComponent.h"
#include "Manager/GameObjectManager.h"
#include "Transform.h"
#include "Render2D.h"
#include "RenderRingLuminescenceBillboardComponent.h"
#include "FighterComponent.h"
#include "RigidBodyComponent.h"
#include "BulletComponent.h"
#include "AttackOneTimeComponent.h"
#include "PlayerDamageComponent.h"
#include "Collider.h"
#include "JumpComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include "Mesh/SquareMesh.h"

using namespace DirectX;

namespace {

	constexpr float BulletTiming = 0.25f;
	constexpr float JumpMoveX = 2.0f;
	constexpr float TargetLength = 5.0f;
}

EnemyActionBossComponent::EnemyActionBossComponent(GameObject& obj) :EnemyActionComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ソート番号を設定

	m_BarrierList.fill(nullptr);

	// バリアオブジェクト作成
	for (int i = 0; i < BarrierCount; ++i) {
		GameObject* barrier = GameObjectManager::AddAbsFront("boss_barrier", "Enemy");
		barrier->SetChildAbsFrontFlag(true);

		TransformComponent* trans = barrier->AddComponent<TransformComponent>();
		trans->SetRotationPattern(RotationPattern::REVOLUTION);
		trans->SetLockScale(true);

		if (i == 0) {
			trans->SetLocalPosition(XMFLOAT3(0.0f, 12.0f, 0.0f));
		}
		else if (i == 1) {
			trans->SetLocalPosition(XMFLOAT3(12.0f, 0.0f, 0.0f));
		}
		else if (i == 2) {
			trans->SetLocalPosition(XMFLOAT3(0.0f, -12.0f, 0.0f));
		}
		else if (i == 3) {
			trans->SetLocalPosition(XMFLOAT3(-12.0f, 0.0f, 0.0f));
		}
		trans->SetLocalScale(XMFLOAT3(7.0f, 7.0f, 7.0f));

		Render2DComponent* rend2D = barrier->AddComponent<Render2DComponent>();
		rend2D->CreateMesh<SquareMesh>();
		rend2D->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		rend2D->ChangeTexture("assets/texture/ring.png");

		RenderRingLuminescenceBillboardComponent* rend = barrier->AddComponent<RenderRingLuminescenceBillboardComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->SetShader("shader/unlitLuminescenceVS.hlsl", "shader/unlitRingLuminescencePS.hlsl");
		rend->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		rend->SetGlowPower(0.5f);
		rend->SetGlowRadius(0.3f);
		rend->SetEllipseScale({ 1.0f,1.0f });
		rend->SetRingRadius(0.5f);
		rend->SetRingWidth(0.2f);
		rend->SetExpand(1.2f);

//		barrier->SetActiveState(ActiveState::ALL_STOP);
		m_Object->SetChild(barrier);
		m_BarrierList[i] = barrier;
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

		m_RecordTime1 += m_deltaTime;

		// バリアを広げるて、展開完了したら通知する
		// なおバリアの処理はUpdate内で行う
		// フラグでスイッチをする
		for (int i = 0; i < BarrierCount; ++i) {


		}

		// 最後に回転を開始

		if (m_RecordTime1 > 1.0f) {	// バリア展開
			ChangeState(BossActionState::DEFAULT);
			m_RecordTime1 = 0.0f;
		}

		break;
	case BossActionState::MAX:
		break;
	default:
		break;
	}

	for (int i = 0; i < BarrierCount; ++i) {

		TransformComponent* barrierTrans = m_BarrierList[i]->GetComponent<TransformComponent>();
		if (barrierTrans != nullptr) {
			barrierTrans->AddLocalRotation({ 0.0f,0.0f,5.0f });
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
	GameObject* bullet = GameObjectManager::AddAbsFront("bullet", "Enemy");

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

	Render2DComponent* rend2D = bullet->AddComponent<Render2DComponent>();
	rend2D->CreateMesh<SquareMesh>();
	rend2D->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	rend2D->ChangeTexture("assets/texture/ring.png");

	RenderRingLuminescenceBillboardComponent* rend = bullet->AddComponent<RenderRingLuminescenceBillboardComponent>();

	rend->CreateMesh<SquareMesh>();
	rend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitRingLuminescencePS.hlsl");
	rend->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
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
		// バリア展開処理をここに書く
		/*for(int i = 0; i < BarrierCount; ++i){
			if (m_BarrierList[i] != nullptr) {
				m_BarrierList[i]->SetActiveState(ActiveState::ACTIVE);
			}
		}*/
		break;
	case BossActionState::MAX:
		break;
	default:
		break;
	}
}