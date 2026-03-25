#include "ChargePerformanceComponent.h"
#include "TransformComponent.h"
#include "RenderLuminescenceBillboardComponent.h"
#include "RenderRingLuminescenceBillboardComponent.h"
#include "ChargeParticleMoveComponent.h"
#include "Manager/GameObjectManager.h"
#include "Manager/TimeManager.h"
#include "Mesh/SquareMesh.h"
#include <algorithm>
#include <random>

using namespace DirectX;

ChargePerformanceComponent::ChargePerformanceComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("EFFECT"); // ソート番号を設定
}

void ChargePerformanceComponent::Update() {

	const float deltaTime = TimeManager::GetFixedDeltaTime();

	// アクティブになると、秒数に応じてパーティクルたちを
	// アクティブにする

	if (m_OneParticleActiveTime < m_RecordTime) {
		for (GameObject* obj : m_ChargeParticles) {

			if (obj->GetActiveState() == ActiveState::ALL_STOP) {
				obj->SetActiveState(ActiveState::ACTIVE);
				break; // １つアクティブにしたら抜ける
			}
		}

		m_RecordTime = 0.0f; // リセット
	}


	m_RecordTime += deltaTime;
	m_RecordTime2 += deltaTime;

	if (m_IsCompleteCharge == true) {
		TransformComponent* trans = m_ChargeCompleteParticle->GetComponent<TransformComponent>();

		if (trans != nullptr) {

			m_ChargeCompleteParticle->SetActiveState(ActiveState::ACTIVE);
			float scale = m_ScaleChangePower * std::sin(m_RecordTime2 * m_ScaleChangeSpeed);
			trans->SetLocalScale({ m_InitScale.x + scale,m_InitScale.y + scale,1.0f });
		}
	}
}

void ChargePerformanceComponent::InitCreateParticles(const unsigned int num, const float radius, GameObject& parent, const float bigRadius, const float power, const float speed) {

	TransformComponent* myTrans = m_Object->GetComponent<TransformComponent>();

	if (myTrans == nullptr) {
		return;
	}

	m_ChargeParticles.clear();

	XMFLOAT3 myPos = myTrans->GetLocalPosition();

	// 指定された数だけパーティクルを生成
	// 円形に配置
	for (int i = 0; i < static_cast<int>(num); ++i) {

		float angle = XM_2PI * (static_cast<float>(i) / static_cast<float>(num)); // 角度を計算
		float x = myPos.x + radius * cosf(angle); // x座標
		float y = myPos.y + radius * sinf(angle); // y座標

		// パーティクル生成
		GameObject* particleObj = GameObjectManager::AddChild("Particle", "Particle");
		TransformComponent* trans = particleObj->AddComponent<TransformComponent>();
		trans->SetLocalPosition(XMFLOAT3(x, y, myPos.z)); // 生成位置を設定（例として原点に設定）
		trans->SetLocalScale({ 20.0f,20.0f,1.0f }); // スケールを設定
		trans->SetLockScale(true);

		ChargeParticleMoveComponent* moveComp = particleObj->AddComponent<ChargeParticleMoveComponent>();
		moveComp->SetTargetObject(*m_Object); // ターゲットを設定
		moveComp->SetMoveSpeed(30.0f); // 移動速度を設定
		moveComp->SetRimitDistance(1.0f); // 到達距離を設定
		moveComp->SetStartPosition(XMFLOAT3(x, y, myPos.z)); // 開始位置を設定
		moveComp->SetInitScale(XMFLOAT3(20.0f, 20.0f, 1.0f)); // 初期スケールを設定

		RenderLuminescenceBillboardComponent* childRender = particleObj->AddComponent<RenderLuminescenceBillboardComponent>();
		childRender->CreateMesh<SquareMesh>();
		childRender->SetShader("unlitTextureVS.hlsl", "unlitLuminescencePS.hlsl");
		childRender->SetColor({ 0.3f,0.8f,1.0f,0.8f });
		childRender->SetGlowPower(1.0f);
		childRender->SetGlowRadius(0.5f);
		childRender->SetEllipseScale({ 1.0f,1.0f });

		particleObj->SetActiveState(ActiveState::ALL_STOP); // 最初は非アクティブにしておく

		parent.SetChild(particleObj); // 親子関係を設定

		m_ChargeParticles.push_back(particleObj);
	}

	std::random_device rd;
	std::mt19937 gen(rd());

	// パーティクルの順番をランダムにシャッフルして出現位置をランダム化
	std::shuffle(m_ChargeParticles.begin(), m_ChargeParticles.end(), gen);

	// プレイヤーの中心に存在する大きなパーティクルも生成
	GameObject* particleObj = GameObjectManager::AddChild("Particle", "Particle");
	TransformComponent* trans = particleObj->AddComponent<TransformComponent>();
	trans->SetLocalPosition(XMFLOAT3(myPos.x, myPos.y, myPos.z)); // 生成位置を設定（例として原点に設定）
	trans->SetLocalScale({ bigRadius,bigRadius,1.0f }); // スケールを設定
	trans->SetLockScale(true);
	m_InitScale = XMFLOAT3(bigRadius, bigRadius, 1.0f);

	RenderLuminescenceBillboardComponent* childRender = particleObj->AddComponent<RenderLuminescenceBillboardComponent>();
	childRender->CreateMesh<SquareMesh>();
	childRender->SetShader("unlitTextureVS.hlsl", "unlitLuminescencePS.hlsl");
	childRender->SetColor({ 0.3f,0.8f,1.0f,0.8f });
	childRender->SetGlowPower(0.5f);
	childRender->SetGlowRadius(0.7f);
	childRender->SetEllipseScale({ 1.0f,1.0f });

	particleObj->SetActiveState(ActiveState::ALL_STOP); // 最初は非アクティブにしておく
	parent.SetChild(particleObj); // 親子関係を設定
	m_ChargeCompleteParticle = particleObj;

	// サイズ変化のパラメータ設定
	m_ScaleChangePower = power;
	m_ScaleChangeSpeed = speed;
}

void ChargePerformanceComponent::ResetAllParticles() {

	for (GameObject* obj : m_ChargeParticles) {
		ChargeParticleMoveComponent* moveComp = obj->GetComponent<ChargeParticleMoveComponent>();
		if (moveComp != nullptr) {
			moveComp->ResetProcess();
		}
	}

	// リセット
	m_RecordTime = 0.0f;
	m_RecordTime2 = 0.0f;

	// サイズもリセット
	m_ChargeCompleteParticle->SetActiveState(ActiveState::ALL_STOP);
	TransformComponent* trans = m_ChargeCompleteParticle->GetComponent<TransformComponent>();
	if (trans != nullptr) {
		trans->SetLocalScale(m_InitScale);
	}
}