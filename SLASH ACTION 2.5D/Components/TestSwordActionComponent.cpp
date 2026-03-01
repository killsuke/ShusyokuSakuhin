#include "TestSwordActionComponent.h"
#include "PlayerOperationComponent.h"
#include "ArbitraryRotationComponent.h"
#include "TransformComponent.h"
#include "Manager/GameObjectManager.h"
#include "ColliderAttackComponent.h"
#include "Effect2DComponent.h"
#include "RenderBillboardComponent.h"
#include "AttackOneTimeComponent.h"
#include "Mesh/SquareMesh.h"
#include "Manager/SceneManager.h"
#include "TrailRenderComponent.h"
#include "RenderLuminescenceBillboardComponent.h"
#include "SlashEffectComponent.h"
#include "SoundComponent.h"
#include <iostream>
#include <DirectXMath.h>

using namespace DirectX;

namespace {
	// 剣の回転の軸
	constexpr XMFLOAT3 Slash1st = XMFLOAT3(0.0f, 1.0f, 1.0f);
	constexpr XMFLOAT3 Slash2nd = XMFLOAT3(0.0f, 1.0f, -1.0f);
	constexpr XMFLOAT3 Slash3rd = XMFLOAT3(0.0f, 0.0f, 1.0f);
	constexpr XMFLOAT3 SlashFast = XMFLOAT3(0.0f, 1.0f, -0.5f);

	// 剣の角度
	constexpr XMFLOAT3 LockAngle1st = XMFLOAT3(-45.0f, 0.0f, 0.0f);
	constexpr XMFLOAT3 LockAngle2nd = XMFLOAT3(45.0f, 0.0f, 0.0f);
	constexpr XMFLOAT3 LockAngle3rd = XMFLOAT3(0.0f, 0.0f, 0.0f);
	constexpr XMFLOAT3 LockAngleFast = XMFLOAT3(30.0f, 0.0f, 0.0f);

	// 剣のエフェクトの角度
	constexpr XMFLOAT3 EffectAngle1st = XMFLOAT3(45.0f, 0.0f, 0.0f);
	constexpr XMFLOAT3 EffectAngle2nd = XMFLOAT3(315.0f, 0.0f, 0.0f);
	constexpr XMFLOAT3 EffectAngle3rd = XMFLOAT3(90.0f, 0.0f, 0.0f);
	constexpr XMFLOAT3 EffectAngleFast = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

TestSwordActionComponent::TestSwordActionComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("BONE"); // ソート番号を設定、のちに完成したらちゃんと変える
}

// 左右への位置を反転処理（入力があれば）
// 剣を振るアクション作成（入力があれば）

void TestSwordActionComponent::Update() {

	if (m_Holder == nullptr) {
		return;
	}

	auto goAround = m_Object->GetComponent<ArbitraryRotationComponent>();
	auto moveComp = m_Holder->GetComponent<PlayerOperationComponent>();
	auto objTrans = m_Object->GetComponent<TransformComponent>();
//	auto sound = SceneManager::GetSound();

	if (goAround == nullptr) {
		return;
	}

	const bool isAroundActive = goAround->GetRollingActive();
	const bool isActiveFlag = goAround->GetActiveFlag();
	const RightLeft isRightLeft = moveComp->GetRightLeft();
	const bool isFinished = goAround->GetIsFinished();

	m_RightLeft = isRightLeft;

	// 剣を振る
	if (m_IsAction == true) {

		// 先ほどまでは、剣の動き終了後にもう一度振れるようにしていた
		// それをプレイヤー側の制御に移す
		if (isAroundActive == false) {

			m_IsUseTrailFlag = true;
			if (isFinished == true) {
				m_IsUseTrailFlag = false;
			}
			SoundComponent* soundComp = m_Object->GetComponent<SoundComponent>();
			soundComp->Play("slash");

			goAround->SetActiveFlag(true);

			// 念のため一度コライダーをオフ
			auto collider = m_Object->GetComponent<ColliderAttackComponent>();
			collider->SetActiveColliderFlag(false);

			// ここで攻撃対称をクリア
			AttackOneTimeComponent* atkComp = m_Object->GetComponent<AttackOneTimeComponent>();
			atkComp->ClearAttackObjs();


			// 簡易実装
			if (m_TestSlashCount == 0) {
				m_SwordActionState = ESwordActionState::SLASH_1ST;
			}
			else if (m_TestSlashCount == 1) {
				m_SwordActionState = ESwordActionState::SLASH_2ND;
			}
			else if (m_TestSlashCount == 2) {
				m_SwordActionState = ESwordActionState::SLASH_3RD;
			}

			// 次の斬り方へ
			m_TestSlashCount++;
			if (m_TestSlashCount == 3) {
				m_TestSlashCount = 0;
			}

			ChoiceSlashPattern(isRightLeft);

			goAround->SetArbitraryAxis(m_SwordActionPattern.arbitraryAxis);

			goAround->SetLockAngle(m_SwordActionPattern.lockAngle);

			//goAround->SetActiveFlag(true);
			goAround->ResetVariables();

			goAround->SetSampleDivisions(14);

			TrailRenderComponent* trail = m_Object->GetComponent<TrailRenderComponent>();
			if (trail != nullptr) {
				trail->SetTrailDivisionsCount(100);
			}

			if (isRightLeft == RightLeft::RIGHT) {

				goAround->SetStartAndEndAngle(m_SwordActionPattern.startAngle, m_SwordActionPattern.endAngle, true);
			}
			else if(isRightLeft == RightLeft::LEFT){

				goAround->SetStartAndEndAngle(m_SwordActionPattern.startAngle, m_SwordActionPattern.endAngle, false);
			}

			// 回転の動きのシミュレーション（軌跡用）
			goAround->SimulationMove();
		}

		m_Object->SetDrawContainerChangeFlag(DrawContainer::AbsFront, true);
	}
	// 振っていない
	else if (m_IsAction == false && isAroundActive == false) {

		// 初期状態に戻す
		m_SwordActionState = ESwordActionState::SLASH_1ST;
		m_TestSlashCount = 0;

		TrailRenderComponent* trail = m_Object->GetComponent<TrailRenderComponent>();
		TrailMakeComponent* trailMake = m_Object->GetComponent<TrailMakeComponent>();
		if (trail != nullptr && trailMake != nullptr) {
			//	trail->ClearTrail();
			trail->SetActiveFlag(false);
			trailMake->SetActiveFlag(false);
		}

		auto collider = m_Object->GetComponent<ColliderAttackComponent>();
		collider->SetActiveColliderFlag(false);

		m_Object->SetDrawContainerChangeFlag(DrawContainer::Default, true);
		goAround->ResetIsFinished();
		goAround->SetActiveFlag(false);
		auto holderTrans = m_Holder->GetComponent<TransformComponent>();
		auto holderPos = holderTrans->GetPosition();

		if (isRightLeft == RightLeft::RIGHT) {	// 右
			objTrans->SetRotation({ 0.0f,0.0f,-50.0f });
			objTrans->SetPosition({ holderPos.x - 3.0f,holderPos.y + 3.0f,3.0f });
		}
		else if(isRightLeft == RightLeft::LEFT){									// 左
			objTrans->SetRotation({ 0.0f,0.0f,-130.0f });
			objTrans->SetPosition({ holderPos.x + 3.0f,holderPos.y + 3.0f,3.0f });
		}
	}

	if (isAroundActive == true) {
		SwordAction();
		m_IsSwordAction = true;
	}
	else {
		m_IsSwordAction = false;
	}


	// 向き反転用
	m_BeforeDirection = isRightLeft;
}

void TestSwordActionComponent::SwordAction() {

	auto goAround = m_Object->GetComponent<ArbitraryRotationComponent>();
	auto collider = m_Object->GetComponent<ColliderAttackComponent>();
	auto atkComp = m_Object->GetComponent<AttackOneTimeComponent>();

	// ここのプレイヤー取得はのちに別のものに変更
	auto moveComp = m_Holder->GetComponent<PlayerOperationComponent>();

	if (goAround == nullptr || collider == nullptr) {
		return;
	}

	collider->SetActiveColliderFlag(true);

	// 左右の向き変わったら、現在角度に＋90度して左右反転、上記の止める処理もちょいと変える？タイム方式とかに
	const RightLeft direction = moveComp->GetRightLeft();

	TrailRenderComponent* trail = m_Object->GetComponent<TrailRenderComponent>();
	TrailMakeComponent* trailMake = m_Object->GetComponent<TrailMakeComponent>();

	if (trail != nullptr && trailMake != nullptr) {
		if (m_IsUseTrailFlag == true) {
			trail->SetActiveFlag(true);
			trailMake->SetActiveFlag(true);
		}
		else {	// 連続攻撃の時の軌跡のズレを直すためにここで一度オフにする
			trail->SetActiveFlag(false);
			trailMake->SetActiveFlag(false);
			goAround->ResetIsFinished();
			m_IsUseTrailFlag = true;
		}
	}

	// 方向が変わったら反転処理
	if (direction == RightLeft::RIGHT && m_BeforeDirection == RightLeft::LEFT) { // 右向き
		goAround->SetClockwise(true);
		goAround->SetFlipRequested(true);

		if (trail != nullptr) {

			trail->SetActiveFlag(true);
			trailMake->SetActiveFlag(true);
			trail->RequestInversion();
			trail->InversionEvent();
		}
	}
	else if (direction == RightLeft::LEFT && m_BeforeDirection == RightLeft::RIGHT) { // 左向き
		goAround->SetClockwise(false);
		goAround->SetFlipRequested(true);

		if (trail != nullptr) {
			trail->SetActiveFlag(true);
			trailMake->SetActiveFlag(true);
			trail->RequestInversion();
			trail->InversionEvent();
		}
	}

	if (atkComp->GetAttackHitFlag() == true) {
		CreateSwordEffect();
		SoundComponent* soundComp = m_Object->GetComponent<SoundComponent>();
		soundComp->Play("slashHit");
	}

}

void TestSwordActionComponent::CreateSwordEffect() {
	auto pos = m_Object->GetComponent<TransformComponent>()->GetPosition();

	float zAngle = 0.0f;
	switch (m_SwordActionState)
	{
	case ESwordActionState::NONE:
		break;
	case ESwordActionState::SLASH_1ST:
		if(m_RightLeft == RightLeft::RIGHT){
			zAngle = EffectAngle1st.x;
		}
		else if(m_RightLeft == RightLeft::LEFT){
			zAngle = EffectAngle2nd.x;
		}
		break;
	case ESwordActionState::SLASH_2ND:
		if (m_RightLeft == RightLeft::LEFT) {
			zAngle = EffectAngle1st.x;
		}
		else if (m_RightLeft == RightLeft::RIGHT) {
			zAngle = EffectAngle2nd.x;
		}
		break;
	case ESwordActionState::SLASH_3RD:
		zAngle = EffectAngle3rd.x;
		break;
	case ESwordActionState::SLASH_FAST:
		zAngle = EffectAngleFast.x;
		break;
	default:
		break;
	}

	GameObject* effect = GameObjectManager::AddAbsFront("swordEffect", "Effect");
	TransformComponent* effectTrans = effect->AddComponent<TransformComponent>();
	effectTrans->SetScale({ 45.0f,135.0f,5.0f });
	if (m_RightLeft == RightLeft::RIGHT) {
		effectTrans->SetPosition({ pos.x + 7.0f, pos.y, pos.z });
	}
	else if (m_RightLeft == RightLeft::LEFT) {
		effectTrans->SetPosition({ pos.x - 7.0f, pos.y, pos.z });
	}
	effectTrans->SetRotation({ 0.0f,0.0f,zAngle });

	// 引き延ばしたり縮めたりするエフェクト
	SlashEffectComponent* slash = effect->AddComponent<SlashEffectComponent>();
	slash->SetRimitTime(0.5f);
	slash->SetSizeChange({ 2.0f, 20.0f });

	// エフェクト用のレンダー
	RenderLuminescenceBillboardComponent* render = effect->AddComponent<RenderLuminescenceBillboardComponent>();
	render->CreateMesh<SquareMesh>();
	render->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitLuminescencePS.hlsl");
	render->SetColor({ 0.3f,0.8f,1.0f,1.0f });
	render->SetGlowPower(0.5f);
	render->SetGlowRadius(0.6f);
	render->SetEllipseScale({ 1.0f,1.0f });
}

// 振り方パターンを選択
void TestSwordActionComponent::ChoiceSlashPattern(const RightLeft& horizontalAxis) {

	switch (m_SwordActionState)
	{
	case ESwordActionState::NONE:
		break;
	case ESwordActionState::SLASH_1ST:
		if (m_RightLeft == RightLeft::RIGHT) {
			m_SwordActionPattern.startAngle = 120.0f;
			m_SwordActionPattern.endAngle = -120.0f;
		}
		else if (m_RightLeft == RightLeft::LEFT) {
			m_SwordActionPattern.startAngle = 60.0f;
			m_SwordActionPattern.endAngle = -60.0f;
		}

		m_SwordActionPattern.arbitraryAxis = Slash1st;
		m_SwordActionPattern.lockAngle = LockAngle1st;

		break;
	case ESwordActionState::SLASH_2ND:
		if (m_RightLeft == RightLeft::RIGHT) {
			m_SwordActionPattern.startAngle = 120.0f;
			m_SwordActionPattern.endAngle = -120.0f;
		}
		else if (m_RightLeft == RightLeft::LEFT) {
			m_SwordActionPattern.startAngle = 60.0f;
			m_SwordActionPattern.endAngle = -60.0f;
		}
		m_SwordActionPattern.arbitraryAxis = Slash2nd;
		m_SwordActionPattern.lockAngle = LockAngle2nd;

		break;
	case ESwordActionState::SLASH_3RD:
		if (m_RightLeft == RightLeft::RIGHT) {
			m_SwordActionPattern.startAngle = 120.0f;
			m_SwordActionPattern.endAngle = -120.0f;
		}
		else if (m_RightLeft == RightLeft::LEFT) {
			m_SwordActionPattern.startAngle = 60.0f;
			m_SwordActionPattern.endAngle = -60.0f;
		}
		m_SwordActionPattern.arbitraryAxis = Slash3rd;
		m_SwordActionPattern.lockAngle = LockAngle3rd;

		break;
	case ESwordActionState::SLASH_FAST:

		if (m_RightLeft == RightLeft::RIGHT) {
			m_SwordActionPattern.startAngle = 180.0f;
			m_SwordActionPattern.endAngle = 0.0f;
		}
		else if (m_RightLeft == RightLeft::LEFT) {
			m_SwordActionPattern.startAngle = 0.0f;
			m_SwordActionPattern.endAngle = 180.0f;
		}

		m_SwordActionPattern.arbitraryAxis = SlashFast;
		m_SwordActionPattern.lockAngle = LockAngleFast;

		break;
	default:
		break;
	}

}
