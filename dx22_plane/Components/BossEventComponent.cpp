#include "BossEventComponent.h"
#include "CameraPointComponent.h"
#include "Transform.h"
#include "Render2D.h"
#include "Render3D.h"
#include "Manager/GameObjectManager.h"
#include "Mesh/CubeMesh.h"
#include "TerrainJsonComponent.h"
#include "MoveTerrainComponent.h"
#include "RigidBodyComponent.h"
#include "JumpComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include "FighterComponent.h"
#include "Mesh/SquareMesh.h"
#include "EnemyActionBossComponent.h"
#include "DoorFadeComponent.h"
#include "MeshCut2DComponent.h"
#include "EnemyDeathEventComponent.h"
#include "ColliderAttackComponent.h"
#include "ColliderDamageComponent.h"

namespace {
	constexpr float DeltaTime = 0.016f;
}

BossEventComponent::BossEventComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE");	// 仮にテストムーブを
}

void BossEventComponent::Update() {

	GameObject* obj = GameObjectManager::GameObjectFindNameUI("fade");

	if(obj == nullptr) {
		return;
	}
	DoorFadeComponent* fade = obj->GetComponent<DoorFadeComponent>();

	if(fade == nullptr) {
		return;
	}

	if (m_IsBossDied == true) {

		if (m_RecordTime > 3.0f) {
			fade->SetWinLoseFlag(true);
			fade->SetNextSceneName("ResultScene");
			fade->SetBootDoor(true);
		}
		m_RecordTime += DeltaTime;
		return;
	}

	CameraPointComponent* point = m_Object->GetComponent<CameraPointComponent>();
	if (point != nullptr)
	{
		if (point->GetScrollDir() == 1) {
			if (createCompletionFlag == false) {
				CreateBossWalls();
				return;
			}

			// ボスの体力をチェックして、0以下なら勝利処理を行う

			GameObject* boss = GameObjectManager::GameObjectFindName("Boss");
			if (boss != nullptr) {
				FighterComponent* bossFighter = boss->GetComponent<FighterComponent>();
				if (bossFighter->GetDeadFlag() == true) {
					EnemyActionBossComponent* bossAction = boss->GetComponent<EnemyActionBossComponent>();
					if (bossAction != nullptr) {
						bossAction->SetActiveFlag(false);
					}
					m_IsBossDied = true;
					return;
				}
			}
			fade->SetWinLoseFlag(false);
		}
	}
}

void BossEventComponent::CreateBossWalls() {

	// ボス戦用の壁を作成
	GameObject* terrainObj = GameObjectManager::AddObject("bossTerrain", "Terrain");
	TransformComponent* transform = terrainObj->AddComponent<TransformComponent>();
	transform->SetPosition({ 1080.0f, -280.0f, 0.0f });
	transform->SetScale({ 8.0f,80.0f,10.0f });

	ColliderComponent* collider = terrainObj->AddComponent<ColliderComponent>();

	Render3DComponent* render = terrainObj->AddComponent<Render3DComponent>();
	render->CreateMesh<CubeMesh>();
	render->SetShader("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");
	render->ChangeTexture("assets/texture/testTerrain.png");


	// ボス本体を作成 =======================================================
	GameObject* boss = GameObjectManager::AddObject("Boss", "Enemy");

	TransformComponent* playerTrans = boss->AddComponent<TransformComponent>();
	playerTrans->SetScale({ 8.0f, 10.0f, 5.0f });
	playerTrans->SetPosition({ 1230.0f,-100.0f,0.0f });

	JumpComponent* cubeJump = boss->AddComponent<JumpComponent>();
	cubeJump->SetJumpPower(70.0f);

	RigidBodyComponent* cubeRigid = boss->AddComponent<RigidBodyComponent>();
	cubeRigid->SetMass(2.0f);
	cubeRigid->SetGravityFlag(true);

	boss->AddComponent<TestExtrusionJudgeComponent>();

	ColliderComponent* cubeColl = boss->AddComponent<ColliderComponent>();
	ColliderAttackComponent* collAttack = boss->AddComponent<ColliderAttackComponent>();
	ColliderDamageComponent* collDamage = boss->AddComponent<ColliderDamageComponent>();

	FighterComponent* fighterPlayer = boss->AddComponent<FighterComponent>();
	fighterPlayer->SetHp(50);
	fighterPlayer->SetMaxHp(50);
	fighterPlayer->SetAtk(10);

	MeshCut2DComponent* meshCut = boss->AddComponent<MeshCut2DComponent>();

	EnemyDeathEventComponent* deathEvent = boss->AddComponent<EnemyDeathEventComponent>();

	EnemyActionBossComponent* enemyAction = boss->AddComponent<EnemyActionBossComponent>();

	Render2DComponent* cubeRe = boss->AddComponent<Render2DComponent>();
	cubeRe->CreateMesh<SquareMesh>();
	cubeRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
//	cubeRe->ChangeTexture("assets/texture/bossHo-dai.png");
	cubeRe->SetColor(DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));

	m_boss = boss;

	createCompletionFlag = true;
}