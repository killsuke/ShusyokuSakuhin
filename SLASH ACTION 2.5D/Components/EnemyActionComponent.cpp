#include "EnemyActionComponent.h"
#include "TransformComponent.h"
#include "ColliderAttackComponent.h"
#include "ColliderDamageComponent.h"
#include "Manager/GameObjectManager.h"
#include "Manager/EventBusManager.h"
#include "RenderBillboardComponent.h"
#include "RigidBodyComponent.h"
#include "Mesh/SquareMesh.h"
#include "Effect2DComponent.h"
#include "ProjectileMotionComponent.h"
#include "Render2DComponent.h"
#include <cmath>

using namespace DirectX;

namespace {
	constexpr XMFLOAT3 BLOODSPLATTER_SCALE(15.0f, 15.0f, 5.0f);
	constexpr XMFLOAT2 OFFSET_POSITION(10.0f, 5.0f);
	constexpr XMFLOAT2 DIVISION_NUM(6.0f, 1.0f);
	constexpr float RIMIT_TIME = 0.2f;
}

EnemyActionComponent::EnemyActionComponent(GameObject& obj) :Component(obj) {

	m_SortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ソート番号を設定
	m_ListenerID_HitEvent = EventBusManager::Subscribe<HitEvent>([&](const HitEvent& e) {
		CreateDamageEffect(e);
		});

	m_ListenerID_DeathEvent = EventBusManager::Subscribe<DeathEvent>([&](const DeathEvent& e) {
		ActionOff(e);
		});

	m_FearPower = FEAR_POWER;
}

EnemyActionComponent::~EnemyActionComponent() {

	EventBusManager::Unsubscribe(m_ListenerID_HitEvent);
	EventBusManager::Unsubscribe(m_ListenerID_DeathEvent);
}

void EnemyActionComponent::Update() {

}

void EnemyActionComponent::CreateDamageEffect(const HitEvent& event) {

	const uint32_t targetID = m_Object->GetInstanceID();

	if (event.targetID != targetID) {
		return;
	}

	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();
	if (myTrans == nullptr) {
		return;
	}

	const XMFLOAT3 pos = myTrans->GetPosition();

	GameObject* effect = GameObjectManager::AddAbsFront("swordEffect", "Effect");
	TransformComponent* effectTrans = effect->AddComponent<TransformComponent>();
	effectTrans->SetScale(BLOODSPLATTER_SCALE);
	RightLeft direction = RightLeft::RIGHT;
	if (m_IsRightLeft == RightLeft::LEFT) {
		effectTrans->SetPosition({ pos.x + OFFSET_POSITION.x, pos.y + OFFSET_POSITION.y, pos.z });
		// 後に切った方向に合わせようか
		direction = RightLeft::RIGHT;
	}
	else if (m_IsRightLeft == RightLeft::RIGHT) {
		effectTrans->SetPosition({ pos.x - OFFSET_POSITION.x, pos.y + OFFSET_POSITION.y, pos.z });
		direction = RightLeft::LEFT;
	}

	RenderBillboardComponent* render = effect->AddComponent<RenderBillboardComponent>();
	SquareMesh* mesh = render->CreateMesh<SquareMesh>();
	render->SetShader("Animation2DVS.hlsl", "unlitTexturePS.hlsl");
	render->ChangeTexture("Blood_Splatter.png");
	render->SetInversionFlag(direction);
	mesh->SetInitialCut(DIVISION_NUM);
	Effect2DComponent* effectComp = effect->AddComponent<Effect2DComponent>();
	effectComp->SetMaxTimeAndCut_X(RIMIT_TIME, DIVISION_NUM.x);
}

void EnemyActionComponent::ActionOff(const DeathEvent& event) {

	const uint32_t deadID = m_Object->GetInstanceID();

	if (event.deadID != deadID) {
		return; // 自分宛じゃないなら無視
	}

	ColliderComponent* coll = m_Object->GetComponent<ColliderComponent>();
	if (coll != nullptr) {
		coll->SetActiveColliderFlag(false);
	}

	ColliderAttackComponent* collAttack = m_Object->GetComponent<ColliderAttackComponent>();
	if (collAttack != nullptr) {
		collAttack->SetActiveColliderFlag(false);
	}

	ColliderDamageComponent* collDamage = m_Object->GetComponent<ColliderDamageComponent>();
	if (collDamage != nullptr) {
		collDamage->SetActiveColliderFlag(false);
	}

	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();
	if (rigid != nullptr) {
		rigid->ClearVelocity();
		rigid->SetActiveFlag(false);
	}

	DeadAnimation();

	m_IsActiveFlag = false;
}

void EnemyActionComponent::FearAction() {

	Render2DComponent* render = m_Object->GetComponent<Render2DComponent>();
	if (render != nullptr) {

		if (m_IsFear == true) {

			m_RecordFearTime += TimeManager::GetFixedDeltaTime();


			const float result = m_FearPower * std::sinf(FEAR_SPEED * m_RecordFearTime);
			render->SetRenderOffsetPosition(XMFLOAT3(result, 0.0f, 0.0f));
			m_FearPower *= FEAR_DECAY; // 徐々に揺れを小さくしていく
		}

		if (m_RecordFearTime >= FEAR_RIMIT_TIME) {
			m_RecordFearTime = 0.0f;
			m_IsFear = false;
			m_FearPower = FEAR_POWER;
			render->SetRenderOffsetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
			m_EnemyState = EEnemyState::WAIT;
		}
	}
}