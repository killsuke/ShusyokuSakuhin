#include "TimeSpawnParticlesComponent.h"
#include "Transform.h"
#include "Manager/GameObjectManager.h"
#include "RenderParticlesComponent.h"
#include "FliesMoveComponent.h"
#include "Mesh/SquareMesh.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace {
	constexpr float DeltaTime = 0.016f;
}

TimeSpawnParticlesComponent::TimeSpawnParticlesComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を仮置き
}

void TimeSpawnParticlesComponent::Update() {
	// 時間経過でパーティクルをスポーンさせる処理をここに実装

	if(m_SpawnInterval <= 0.0f) {

		TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();

		// パーティクル生成
		GameObject* particleObj = GameObjectManager::AddUI("Particle", "Particle");
		TransformComponent* trans = particleObj->AddComponent<TransformComponent>();
		trans->SetPosition(myTrans->GetPosition()); // 生成位置を設定（例として原点に設定）
		trans->SetScale({ 50.0f,50.0f,5.0f }); // スケールを設定
		FliesMoveComponent* moveComp = particleObj->AddComponent<FliesMoveComponent>();
		moveComp->SetMovePower(5.0f); // 移動速度を設定
		RenderParticlesComponent* renderComp = particleObj->AddComponent<RenderParticlesComponent>();
		renderComp->SetVanishTime(3.0f);
		renderComp->CreateMesh<SquareMesh>();
		renderComp->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
		renderComp->ChangeTexture("assets/texture/shine.png");
		renderComp->SetColor({0.3f,0.8f,1.0f,1.0f});

		m_SpawnInterval = m_InitInterval; // インターバルをリセット（例として1秒に設定）
		return;
	}

	m_SpawnInterval -= DeltaTime;
}