#include "BossEventComponent.h"
#include "CameraPointComponent.h"
#include "Transform.h"
#include "Collider.h"
#include "Render3D.h"
#include "GameObjectManager.h"
#include "CubeMesh.h"
#include "TerrainJsonComponent.h"
#include "MoveTerrainComponent.h"
#include "RigidBodyComponent.h"
#include "JumpComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include "FighterComponent.h"
#include "SquareMesh.h"

BossEventComponent::BossEventComponent(GameObject& obj) : Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE");	// 仮にテストムーブを
}

void BossEventComponent::Update() {
	auto point = p_object->GetComponent<CameraPointComponent>();
	if (point != nullptr)
	{
		if (point->GetScrollDir() == 1) {
			if(createCompletionFlag == false) {
				CreateBossWalls();
			}



		}



	}
}

void BossEventComponent::CreateBossWalls() {

	auto terrainObj = GameObjectManager::AddObject("bossTerrain", "Terrain");
	auto transform = terrainObj->AddComponent<TransformComponent>();
	transform->SetPosition({ 1080.0f, -280.0f, 0.0f });
	transform->SetScale({8.0f,80.0f,10.0f});

	auto collider = terrainObj->AddComponent<ColliderComponent>();

	CubeMesh cubeMesh;
	auto render = terrainObj->AddComponent<Render3DComponent>();
	render->SetMesh(cubeMesh);
	render->SetShader("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");
	render->SetTexture("assets/texture/testTerrain.png");


	auto boss = GameObjectManager::AddObject("Boss", "Enemy");

	auto playerTrans = boss->AddComponent<TransformComponent>();
	playerTrans->SetScale({ 4.0f, 10.0f, 5.0f });
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
	fighterPlayer->SetUseInvincible(true);

	SquareMesh cubeMesh2;	// 四角形のメッシュ
	auto cubeRe = boss->AddComponent<Render3DComponent>();
	cubeRe->SetMesh(cubeMesh2);
	cubeRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	cubeRe->SetTexture("assets/texture/NoTexture.png");

	//CubeMesh cubeMesh2;
	//auto cubeRe2 = boss->AddComponent<Render3DColliderAABBComponent>();
	//cubeRe2->SetMesh(cubeMesh2);
	//cubeRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//cubeRe2->SetTexture("assets/texture/NoTexture.png");
	//cubeRe2->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));

	createCompletionFlag = true;
}