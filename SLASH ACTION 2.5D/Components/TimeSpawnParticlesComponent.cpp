#include "TimeSpawnParticlesComponent.h"
#include "TransformComponent.h"
#include "Manager/GameObjectManager.h"
#include "RenderParticlesComponent.h"
#include "RenderLuminescenceBillboardComponent.h"
#include "Render3DComponent.h"
#include "FliesMoveComponent.h"
#include "Mesh/SquareMesh.h"

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
		trans->SetScale({ 100.0f,100.0f,5.0f }); // スケールを設定
		FliesMoveComponent* moveComp = particleObj->AddComponent<FliesMoveComponent>();
		moveComp->SetMovePower(5.0f); // 移動速度を設定

		RenderParticlesComponent* renderComp1 = particleObj->AddComponent<RenderParticlesComponent>();
		renderComp1->CreateMesh<SquareMesh>();
		renderComp1->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		renderComp1->ChangeTexture("shine.png");
		renderComp1->SetColor({1.0f,1.0f,1.0f,1.0f});
		renderComp1->SetVanishTime(3.0f);

		RenderLuminescenceBillboardComponent* childRender = particleObj->AddComponent<RenderLuminescenceBillboardComponent>();
		childRender->CreateMesh<SquareMesh>();
		childRender->SetShader("unlitTextureVS.hlsl", "unlitLuminescencePS.hlsl");
		childRender->SetColor({ 0.3f,0.8f,1.0f,0.5f });
		childRender->SetGlowPower(0.5f);
		childRender->SetGlowRadius(0.5f);
		childRender->SetEllipseScale({ 1.0f,1.0f });

		m_SpawnInterval = m_InitInterval; // インターバルをリセット（例として1秒に設定）
		return;
	}

	m_SpawnInterval -= DeltaTime;
}