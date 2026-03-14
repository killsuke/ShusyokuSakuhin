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

using namespace DirectX;

namespace {
	constexpr XMFLOAT3 BLOODSPLATTER_SCALE(15.0f,15.0f,5.0f);
	constexpr XMFLOAT2 OFFSET_POSITION(10.0f,5.0f);
	constexpr XMFLOAT2 DIVISION_NUM(6.0f, 1.0f);
	constexpr float RIMIT_TIME = 0.2f;
}

EnemyActionComponent::EnemyActionComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // É\Å[Égî‘çÜÇê›íË
	m_listenerID_HitEvent = EventBusManager::Subscribe<HitEvent>([&](const HitEvent& e) {
		CreateDamageEffect(e);
		});

	m_listenerID_DeathEvent = EventBusManager::Subscribe<DeathEvent>([&](const DeathEvent& e) {
		ActionOff(e);
		});
}

EnemyActionComponent::~EnemyActionComponent() {

	EventBusManager::Unsubscribe(m_listenerID_HitEvent);
	EventBusManager::Unsubscribe(m_listenerID_DeathEvent);
}

void EnemyActionComponent::Update() {

}

void EnemyActionComponent::CreateDamageEffect(const HitEvent& event) {

	const uint32_t targetID = m_Object->GetInstanceID();

	if(event.targetID != targetID) {
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
		// å„Ç…êÿÇ¡ÇΩï˚å¸Ç…çáÇÌÇπÇÊÇ§Ç©
		direction = RightLeft::RIGHT;
	}
	else if(m_IsRightLeft == RightLeft::RIGHT){
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
		return; // é©ï™à∂Ç∂Ç·Ç»Ç¢Ç»ÇÁñ≥éã
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

	/*ProjectileMotionComponent* proj = m_Object->GetComponent<ProjectileMotionComponent>();
	if (proj != nullptr) {
		proj->InitProjectile(XMFLOAT3(0.5f,1.0f,-1.0f), XMFLOAT3(0.0f,0.0f,10.0f), 25.0f);
	}*/


	m_IsActiveFlag = false;

}