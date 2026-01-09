#include "EnemyActionComponent.h"
#include "Transform.h"
#include "Collider.h"
#include "Manager/GameObjectManager.h"
#include "Manager/EventBusManager.h"
#include "RenderBillboard.h"
#include "RigidBodyComponent.h"
#include "Mesh/SquareMesh.h"
#include "Effect2DComponent.h"
#include "ProjectileMotionComponent.h"

using namespace DirectX;

namespace {
	constexpr XMFLOAT3 BloodSplatterScale(15.0f,15.0f,5.0f);
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

	auto pos = m_Object->GetComponent<TransformComponent>()->GetPosition();

	auto effect = GameObjectManager::AddAbsFront("swordEffect", "Effect");
	auto effectTrans = effect->AddComponent<TransformComponent>();
	effectTrans->SetScale(BloodSplatterScale);
	RightLeft direction = RightLeft::RIGHT;
	if (m_IsRightLeft == RightLeft::LEFT) {
		effectTrans->SetPosition({ pos.x - 10.0f, pos.y + 5.0f, pos.z });
		// å„Ç…êÿÇ¡ÇΩï˚å¸Ç…çáÇÌÇπÇÊÇ§Ç©
		direction = RightLeft::RIGHT;
	}
	else if(m_IsRightLeft == RightLeft::RIGHT){
		effectTrans->SetPosition({ pos.x + 10.0f, pos.y + 5.0f, pos.z });
		direction = RightLeft::LEFT;
	}

	auto render = effect->AddComponent<RenderBillboardComponent>();
	auto mesh = render->CreateMesh<SquareMesh>();
	render->SetShader("shader/Animation2DVS.hlsl", "shader/unlitTexturePS.hlsl");
	render->ChangeTexture("assets/texture/Blood_Splatter.png");
	render->SetInversionFlag(direction);
	mesh->SetInitialCut(6.0f, 1.0f);
	auto effectComp = effect->AddComponent<Effect2DComponent>();
	effectComp->SetMaxTimeAndCut_X(0.2f, 6.0f);
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