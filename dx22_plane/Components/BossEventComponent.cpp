#include "BossEventComponent.h"
#include "CameraPointComponent.h"
#include "Transform.h"
#include "Collider.h"
#include "Render2D.h"
#include "Render3D.h"
#include "GameObjectManager.h"
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
#include "GameObjectManager.h"

BossEventComponent::BossEventComponent(GameObject& obj) : Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE");	// 仮にテストムーブを
}

void BossEventComponent::Update() {
	auto point = m_Object->GetComponent<CameraPointComponent>();
	if (point != nullptr)
	{
		if (point->GetScrollDir() == 1) {
			if(createCompletionFlag == false) {
				CreateBossWalls();
				return;
			}

			auto obj = GameObjectManager::GameObjectFindNameUI("fade");
			auto fade = obj->GetComponent<DoorFadeComponent>();

			if(GameObjectManager::GameObjectFindName("Boss") == nullptr) {
				fade->SetWinLoseFlag(true);
				fade->SetNextSceneName("ResultScene");
				fade->SetBootDoor(true);
				return;
			}
			fade->SetWinLoseFlag(false);

		}



	}
}

void BossEventComponent::CreateBossWalls() {

	auto terrainObj = GameObjectManager::AddObject("bossTerrain", "Terrain");
	auto transform = terrainObj->AddComponent<TransformComponent>();
	transform->SetPosition({ 1080.0f, -280.0f, 0.0f });
	transform->SetScale({8.0f,80.0f,10.0f});

	auto collider = terrainObj->AddComponent<ColliderComponent>();

	auto render = terrainObj->AddComponent<Render3DComponent>();
	render->CreateMesh<CubeMesh>();
	render->SetShader("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");
	render->ChangeTexture("assets/texture/testTerrain.png");


	auto boss = GameObjectManager::AddObject("Boss", "Enemy");

	auto playerTrans = boss->AddComponent<TransformComponent>();
	playerTrans->SetScale({ 40.0f, 30.0f, 5.0f });
	playerTrans->SetPosition({ 1230.0f,-100.0f,0.0f });

	auto cubeJump = boss->AddComponent<JumpComponent>();
	cubeJump->SetJumpPower(50.0f);

	auto cubeRigid = boss->AddComponent<RigidBodyComponent>();
	cubeRigid->SetMass(2.0f);
	cubeRigid->SetGravityFlag(true);

	boss->AddComponent<TestExtrusionJudgeComponent>();

	auto cubeColl = boss->AddComponent<ColliderComponent>();

	auto fighterPlayer = boss->AddComponent<FighterComponent>();
	fighterPlayer->SetHp(50);
	fighterPlayer->SetMaxHp(50);
	fighterPlayer->SetAtk(10);
	//fighterPlayer->SetUseInvincible(true);	// 一旦封印（のちにボスを無敵時間入れつつ倒せるようにする）

	auto enemyAction = boss->AddComponent<EnemyActionBossComponent>();

	auto cubeRe = boss->AddComponent<Render2DComponent>();
	cubeRe->CreateMesh<SquareMesh>();
	cubeRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	cubeRe->ChangeTexture("assets/texture/bossHo-dai.png");

	//CubeMesh cubeMesh2;
	//auto cubeRe2 = boss->AddComponent<Render3DColliderAABBComponent>();
	//cubeRe2->SetMesh(cubeMesh2);
	//cubeRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//cubeRe2->SetTexture("assets/texture/NoTexture.png");
	//cubeRe2->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));

	m_boss = boss;

	createCompletionFlag = true;
}