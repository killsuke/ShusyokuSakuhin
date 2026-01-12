#include "ChargePerformanceComponent.h"
#include "Transform.h"
#include "RenderLuminescenceBillboardComponent.h"
#include "ChargeParticleMoveComponent.h"
#include "Manager/GameObjectManager.h"
#include "Mesh/SquareMesh.h"

namespace {
	constexpr float DeltaTime = 0.016f;
}

using namespace DirectX;

ChargePerformanceComponent::ChargePerformanceComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("EFFECT"); // ソート番号を設定
}

void ChargePerformanceComponent::Update() {

	// アクティブになると、秒数に応じてパーティクルたちを
	// アクティブにする

	//if (m_OneParticleActiveTime > m_RecordTime) {
	//	for (GameObject* obj : m_ChargeParticles) {

	//		if (obj->GetActiveState() == ActiveState::ALL_STOP) {
	//			obj->SetActiveState(ActiveState::ACTIVE);
	//			break; // １つアクティブにしたら抜ける
	//		}
	//	}

	//	m_OneParticleActiveTime = 0.0f; // リセット
	//}

	//m_RecordTime += DeltaTime;
}

void ChargePerformanceComponent::InitCreateParticles(const unsigned int num, const float radius, GameObject& parent) {

	// radius決めて、円周上に均等に配置するように変更予定

	for (int i = 0; i < static_cast<int>(num); ++i) {

		TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();
		XMFLOAT3 myPos = myTrans->GetLocalPosition();

		float angle = XM_2PI * (static_cast<float>(i) / static_cast<float>(num)); // 角度を計算
		float x = myPos.x + radius * cosf(angle); // x座標
		float y = myPos.y + radius * sinf(angle); // y座標

		// パーティクル生成
		GameObject* particleObj = GameObjectManager::AddChild("Particle", "Particle");
		TransformComponent* trans = particleObj->AddComponent<TransformComponent>();
		trans->SetLocalPosition(XMFLOAT3(x, y, myPos.z)); // 生成位置を設定（例として原点に設定）
		trans->SetLocalScale({ 20.0f,20.0f,1.0f }); // スケールを設定

		//ChargeParticleMoveComponent* moveComp = particleObj->AddComponent<ChargeParticleMoveComponent>();
		//moveComp->SetTargetObject(*m_Object); // ターゲットを設定

		RenderLuminescenceBillboardComponent* childRender = particleObj->AddComponent<RenderLuminescenceBillboardComponent>();
		childRender->CreateMesh<SquareMesh>();
		childRender->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitLuminescencePS.hlsl");
		childRender->SetColor({ 0.3f,0.8f,1.0f,0.5f });
		childRender->SetGlowPower(0.5f);
		childRender->SetGlowRadius(0.2f);
		childRender->SetEllipseScale({ 1.0f,1.0f });

		//	particleObj->SetActiveState(ActiveState::ALL_STOP); // 最初は非アクティブにしておく

		parent.SetChild(particleObj); // 親子関係を設定

		m_ChargeParticles.push_back(particleObj);
	}
}