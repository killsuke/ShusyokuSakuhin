#include "TestSwordActionComponent.h"
#include "PlayerOperationComponent.h"
#include "ArbitraryRotationComponent.h"
#include "Transform.h"
#include "Manager/GameObjectManager.h"
#include "Collider.h"
#include "Effect2DComponent.h"
#include "RenderBillboard.h"
#include "AttackOneTimeComponent.h"
#include "Mesh/SquareMesh.h"
#include "Manager/SceneManager.h"
#include "TrailRenderComponent.h"
#include <iostream>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace {
	// 剣の回転の軸
	constexpr XMFLOAT3 Slash1st = XMFLOAT3(0.0f, 1.0f, 1.0f);
	constexpr XMFLOAT3 Slash2nd = XMFLOAT3(0.0f, 1.0f, -1.0f);
	constexpr XMFLOAT3 Slash3rd = XMFLOAT3(0.0f, 0.0f, 1.0f);

	// 剣の角度
	constexpr XMFLOAT3 LockAngle1st = XMFLOAT3(-45.0f, 0.0f, 0.0f);
	constexpr XMFLOAT3 LockAngle2nd = XMFLOAT3(45.0f, 0.0f, 0.0f);
	constexpr XMFLOAT3 LockAngle3rd = XMFLOAT3(0.0f, 0.0f, 0.0f);
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
	auto sound = SceneManager::GetSound();

	if (goAround == nullptr) {
		return;
	}

	const bool isAroundActive = goAround->GetRollingActive();
	const bool isActiveFlag = goAround->GetActiveFlag();
	const bool isRightLeft = moveComp->GetRightLeft();
	const bool isFinished = goAround->GetIsFinished();

	m_RightLeft = isRightLeft;

	// 剣を振る
	if (m_IsAction == true) {

		if (isAroundActive == false || isFinished == true) {

			m_IsUseTrailFlag = true;
			if (isFinished == true) {
				m_IsUseTrailFlag = false;
			}

			sound.Play(SOUND_LABEL::SOUND_LABEL_SE000);

			goAround->SetActiveFlag(true);

			// 念のため一度コライダーをオフ
			auto collider = m_Object->GetComponent<ColliderComponent>();
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
			if (m_TestSlashCount > 2) {
				m_TestSlashCount = 0;
			}

			ChoiceSlashPattern(isRightLeft);

			goAround->SetArbitraryAxis(m_SwordActionPattern.arbitraryAxis);

			goAround->SetLockAngle(m_SwordActionPattern.lockAngle);

			//goAround->SetActiveFlag(true);
			goAround->ResetVariables();

			if (isRightLeft == true) {
				goAround->SetStartAndEndAngle(120.0f, -120.0f, true);
			}
			else {
				goAround->SetStartAndEndAngle(60.0f, -60.0f, false);
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

		auto collider = m_Object->GetComponent<ColliderComponent>();
		collider->SetActiveColliderFlag(false);

		m_Object->SetDrawContainerChangeFlag(DrawContainer::Default, true);
		goAround->ResetIsFinished();
		goAround->SetActiveFlag(false);
		auto holderTrans = m_Holder->GetComponent<TransformComponent>();
		auto holderPos = holderTrans->GetPosition();

		if (moveComp->GetRightLeft() == true) {	// 右
			objTrans->SetRotation({ 0.0f,0.0f,-50.0f });
			objTrans->SetPosition({ holderPos.x - 3.0f,holderPos.y + 3.0f,3.0f });
		}
		else {									// 左
			objTrans->SetRotation({ 0.0f,0.0f,-130.0f });
			objTrans->SetPosition({ holderPos.x + 3.0f,holderPos.y + 3.0f,3.0f });
		}
	}


	if (isAroundActive == true) {
		SwordAction();
	}

	/*Vector3 nowPos = objTrans->GetPosition();
	if (nowPos.z < 0.0f) {
		m_Object->SetDrawContainerChangeFlag(DrawContainer::Default, true);
	}*/

	// 向き反転用
	m_BeforeDirection = isRightLeft;
	m_IsAction = false;
}

void TestSwordActionComponent::SwordAction() {

	auto sound = SceneManager::GetSound();
	auto goAround = m_Object->GetComponent<ArbitraryRotationComponent>();
	auto collider = m_Object->GetComponent<ColliderComponent>();
	auto atkComp = m_Object->GetComponent<AttackOneTimeComponent>();

	// ここのプレイヤー取得はのちに別のものに変更
	auto moveComp = m_Holder->GetComponent<PlayerOperationComponent>();

	if (goAround == nullptr || collider == nullptr) {
		return;
	}

	collider->SetActiveColliderFlag(true);

	// 左右の向き変わったら、現在角度に＋90度して左右反転、上記の止める処理もちょいと変える？タイム方式とかに
	const bool direction = moveComp->GetRightLeft();
	//const bool isFinished = goAround->GetIsFinished();

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
	if (direction == true && m_BeforeDirection == false) { // 右向き
		goAround->SetClockwise(true);
		goAround->SetFlipRequested(true);

		if (trail != nullptr) {

			trail->SetActiveFlag(true);
			trailMake->SetActiveFlag(true);
			trail->RequestInversion();
		}

		//	m_RightLeft = true;
	}
	else if (direction == false && m_BeforeDirection == true) { // 左向き
		goAround->SetClockwise(false);
		goAround->SetFlipRequested(true);

		if (trail != nullptr) {
			trail->SetActiveFlag(true);
			trailMake->SetActiveFlag(true);
			trail->RequestInversion();
		}

		//	m_RightLeft = false;
	}

	if (atkComp->GetAttackHitFlag() == true) {
		CreateSwordEffect();
		//sound.Stop(SOUND_LABEL::SOUND_LABEL_SE001);
		sound.Play(SOUND_LABEL::SOUND_LABEL_SE001);
	}

}

void TestSwordActionComponent::CreateSwordEffect() {
	auto pos = m_Object->GetComponent<TransformComponent>()->GetPosition();

	auto effect = GameObjectManager::AddAbsFront("swordEffect", "Effect");
	auto effectTrans = effect->AddComponent<TransformComponent>();
	effectTrans->SetScale({ 15.0f,15.0f,5.0f });
	if (m_RightLeft == true) {
		effectTrans->SetPosition({ pos.x + 7.0f, pos.y, pos.z });
	}
	else {
		effectTrans->SetPosition({ pos.x - 7.0f, pos.y, pos.z });
	}

	auto render = effect->AddComponent<RenderBillboardComponent>();
	auto mesh = render->CreateMesh<SquareMesh>();
	render->SetShader("shader/Animation2DVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	render->ChangeTexture("assets/texture/Blood_Splatter.png");
	render->ChangeTexture("assets/texture/swordEffect.png");
	render->SetInversionFlag(!m_RightLeft);
	mesh->SetInitialCut(5.0f, 1.0f);
	auto effectComp = effect->AddComponent<Effect2DComponent>();
	effectComp->SetMaxTimeAndCut_X(0.3f, 5.0f);
	//	effectComp->SetMaxTimeAndCut_X(0.2f, 6.0f);
}

void TestSwordActionComponent::ChoiceSlashPattern(const bool horizontalAxis) {

	switch (m_SwordActionState)
	{
	case ESwordActionState::NONE:
		break;
	case ESwordActionState::SLASH_1ST:
		if (m_RightLeft == true) {
			m_SwordActionPattern.startAngle = 120.0f;
			m_SwordActionPattern.endAngle = -120.0f;
		}
		else {
			m_SwordActionPattern.startAngle = 60.0f;
			m_SwordActionPattern.endAngle = -60.0f;
		}
		m_SwordActionPattern.arbitraryAxis = Slash1st;
		m_SwordActionPattern.lockAngle = LockAngle1st;

		break;
	case ESwordActionState::SLASH_2ND:
		if (m_RightLeft == true) {
			m_SwordActionPattern.startAngle = 120.0f;
			m_SwordActionPattern.endAngle = -120.0f;
		}
		else {
			m_SwordActionPattern.startAngle = 60.0f;
			m_SwordActionPattern.endAngle = -60.0f;
		}
		m_SwordActionPattern.arbitraryAxis = Slash2nd;
		m_SwordActionPattern.lockAngle = LockAngle2nd;

		break;
	case ESwordActionState::SLASH_3RD:
		if (m_RightLeft == true) {
			m_SwordActionPattern.startAngle = 120.0f;
			m_SwordActionPattern.endAngle = -120.0f;
		}
		else {
			m_SwordActionPattern.startAngle = 60.0f;
			m_SwordActionPattern.endAngle = -60.0f;
		}
		m_SwordActionPattern.arbitraryAxis = Slash3rd;
		m_SwordActionPattern.lockAngle = LockAngle3rd;

		break;
	default:
		break;
	}

}
