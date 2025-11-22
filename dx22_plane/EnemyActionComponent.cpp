#include "EnemyActionComponent.h"
#include "Transform.h"
#include "GameObjectManager.h"
#include "RenderBillboard.h"
#include "Mesh/SquareMesh.h"
#include "Effect2DComponent.h"

using namespace DirectX;

namespace {
	constexpr XMFLOAT3 BloodSplatterScale(15.0f,15.0f,5.0f);
}

EnemyActionComponent::EnemyActionComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("ENEMY_ACTION"); // ƒ\[ƒg”Ô†‚ðÝ’è
}

void EnemyActionComponent::Update() {

}

void EnemyActionComponent::CreateDamageEffect() {

	auto pos = m_Object->GetComponent<TransformComponent>()->GetPosition();

	auto effect = GameObjectManager::AddAbsFront("swordEffect", "Effect");
	auto effectTrans = effect->AddComponent<TransformComponent>();
	effectTrans->SetScale(BloodSplatterScale);
	if (m_IsRightLeft == true) {
		effectTrans->SetPosition({ pos.x - 10.0f, pos.y + 5.0f, pos.z });
	}
	else {
		effectTrans->SetPosition({ pos.x + 10.0f, pos.y + 5.0f, pos.z });
	}

	auto render = effect->AddComponent<RenderBillboardComponent>();
	auto mesh = render->CreateMesh<SquareMesh>();
	render->SetShader("shader/Animation2DVS.hlsl", "shader/unlitTexturePS.hlsl");
	render->ChangeTexture("assets/texture/Blood_Splatter.png");
	render->SetInversionFlag(m_IsRightLeft);
	mesh->SetInitialCut(6.0f, 1.0f);
	auto effectComp = effect->AddComponent<Effect2DComponent>();
	effectComp->SetMaxTimeAndCut_X(0.2f, 6.0f);
}