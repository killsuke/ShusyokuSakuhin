#include "TestSwordActionComponent.h"
#include "TestMoveComponent.h"
#include "GoAroundComponent.h"
#include "Transform.h"
#include "input.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "Effect2DComponent.h"
#include "RenderBillboard.h"
#include "AttackOneTimeComponent.h"
#include "SquareMesh.h"
#include "SceneManager.h"

TestSwordActionComponent::TestSwordActionComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("BONE"); // ソート番号を設定、のちに完成したらちゃんと変える
}

// 左右への位置を反転処理（入力があれば）
// 剣を振るアクション作成（入力があれば）

void TestSwordActionComponent::Update() {

	if (m_holder == nullptr) {
		return;
	}

	auto goAround = p_object->GetComponent<GoAroundComponent>();
	auto moveComp = m_holder->GetComponent<TestMoveComponent>();
	auto sound = SceneManager::GetSound();

	bool keyEnter = Input::GetKeyTrigger(VK_L);

	// 剣を振る
	if (keyEnter == true) {
		if( m_swordAction == false ) {
			sound.Play(SOUND_LABEL::SOUND_LABEL_SE000);
		}

		m_swordAction = true;
		goAround->SetActiveFlag(true);
		goAround->SetRollingActive(true);

		p_object->SetDrawContainerChangeFlag(DrawContainer::AbsFront, true);
	}
	// 振っていない
	else if(keyEnter == false && m_swordAction == false){
		p_object->SetDrawContainerChangeFlag(DrawContainer::Default, true);
		goAround->SetActiveFlag(false);
		auto holderTrans = m_holder->GetComponent<TransformComponent>();
		auto holderPos = holderTrans->GetPosition();
		auto objTrans = p_object->GetComponent<TransformComponent>();

		if (moveComp->GetRightLeft() == true) {	// 右
			objTrans->SetRotation({ 0.0f,0.0f,130.0f });
			objTrans->SetPosition({ holderPos.x - 3.0f,holderPos.y + 3.0f,objTrans->GetPosition().z });
		}
		else {									// 左
			objTrans->SetRotation({ 0.0f,0.0f,50.0f });
			objTrans->SetPosition({ holderPos.x + 3.0f,holderPos.y + 3.0f,objTrans->GetPosition().z });
		}
	}

	if (m_swordAction == true) {
		SwordAction();
	}
}

void TestSwordActionComponent::SwordAction() {
	auto sound = SceneManager::GetSound();
	auto goAround = p_object->GetComponent<GoAroundComponent>();
	auto collider = p_object->GetComponent<ColliderComponent>();
	auto atkComp = p_object->GetComponent<AttackOneTimeComponent>();

	// ここのプレイヤー取得はのちに別のものに変更
	auto moveComp = m_holder->GetComponent<TestMoveComponent>();

	if (goAround == nullptr || collider == nullptr) {
		return;
	}

	collider->SetActiveColliderFlag(true);

	// 左右の向き変わったら、現在角度に＋90度して左右反転、上記の止める処理もちょいと変える？タイム方式とかに
	bool direction = moveComp->GetRightLeft();

	// ここで止める処理
	if ((goAround->GetNowAngleDegree() < 0.0f && direction == true) || (goAround->GetNowAngleDegree() > 180.0f && direction == false)) {
		goAround->SetRollingActive(false);
		goAround->ResetNowAngle_Radian();
		goAround->ResetNowAngle_Degree();
		collider->SetActiveColliderFlag(false);
		m_swordAction = false;
		p_object->SetDrawContainerChangeFlag(DrawContainer::Default,true);
		auto holderTrans = m_holder->GetComponent<TransformComponent>();
		auto holderPos = holderTrans->GetPosition();
		auto objTrans = p_object->GetComponent<TransformComponent>();

		if (moveComp->GetRightLeft() == true) {
			objTrans->SetRotation({ 0.0f,0.0f,130.0f });
			objTrans->SetPosition({ holderPos.x - 3.0f,holderPos.y + 3.0f,objTrans->GetPosition().z });
		}
		else {
			objTrans->SetRotation({ 0.0f,0.0f,50.0f });
			objTrans->SetPosition({ holderPos.x + 3.0f,holderPos.y + 3.0f,objTrans->GetPosition().z });
		}
		goAround->SetActiveFlag(false);

		return;
	}

	if (direction == true && m_beforeDirection == false) { // 右向き
		goAround->SetClockwise(true);
		goAround->SetFlipRequested(true);
		m_rightLeft = true;
	}
	else if (direction == false && m_beforeDirection == true) { // 左向き
		goAround->SetClockwise(false);
		goAround->SetFlipRequested(true);
		m_rightLeft = false;
	}

	if (atkComp->GetAttackHitFlag() == true) {
		CreateSwordEffect();
		//sound.Stop(SOUND_LABEL::SOUND_LABEL_SE001);
		sound.Play(SOUND_LABEL::SOUND_LABEL_SE001);
	}

	m_beforeDirection = direction;
}

void TestSwordActionComponent::CreateSwordEffect() {
	auto pos = p_object->GetComponent<TransformComponent>()->GetPosition();

	auto effect = GameObjectManager::AddAbsFront("swordEffect", "Effect");
	auto effectTrans = effect->AddComponent<TransformComponent>();
	effectTrans->SetScale({ 15.0f,15.0f,5.0f });
	if( m_rightLeft == true ) {
		effectTrans->SetPosition({ pos.x + 7.0f, pos.y, pos.z });
	}
	else {
		effectTrans->SetPosition({ pos.x - 7.0f, pos.y, pos.z });
	}
	SquareMesh square;
	auto render = effect->AddComponent<RenderBillboardComponent>();
	render->SetMesh(square);
	render->SetShader("Animation2DVS.hlsl", "shader/unlitTexturePS.hlsl");
	render->SetTexture("assets/texture/swordEffect.png");
	render->SetInversionFlag(!m_rightLeft);
	auto tex = render->GetTexture();
	tex->SetInitialCut(5.0f, 1.0f);
	auto effectComp = effect->AddComponent<Effect2DComponent>();
	effectComp->SetMaxTimeAndCut_X(0.3f, 5.0f);
}