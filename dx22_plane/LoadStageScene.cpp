#include "LoadStageScene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "CubeMesh.h"
#include "SquareMesh.h"
#include "CircleMesh.h"
#include "SphereMesh.h"
#include "Render3D.h"
#include "Render3DColliderAABBComponent.h"
#include "Render3DColliderOBBComponent.h"
#include "Collider.h"
#include "TestMoveComponent.h"
#include "TestExtrusionComponent.h"
#include "RigidBodyComponent.h"
#include "JumpComponent.h"
#include "EnemyDamageComponent.h"
#include "FighterComponent.h"
#include "AttackComponent.h"
#include "CameraMoveComponent.h"
#include "CameraPointComponent.h"
#include "CameraTargetComponent.h"
#include "GoAroundComponent.h"
#include "HPBarMoveComponent.h"
#include "Spring.h"
#include "StageLoadCSVComponent.h"
#include "TerrainManagerComponent.h"
#include "TerrainJsonComponent.h"

LoadStageScene::LoadStageScene() {
	auto camera = GameObjectManager::AddObject("camera", "Camera");
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(DirectX::SimpleMath::Vector3(10.0f, 10.0f, -170.0f));
	camera->AddComponent<RigidBodyComponent>();
	//auto cameraMove = camera->AddComponent<CameraMoveComponent>();
	camera->AddComponent<Camera>();

//	camera->AddComponent<SpringComponent>();

	{
		auto stageRoadCSV = GameObjectManager::AddObject("StageRoadCSV", "StageRoadCSV");
		auto stageRoadCSVTrans = stageRoadCSV->AddComponent<TransformComponent>();	
		auto str = stageRoadCSV->AddComponent<StageLoadCSVComponent>();
		auto teM = stageRoadCSV->AddComponent<TerrainManagerComponent>();

		auto terrainJson = stageRoadCSV->AddComponent<TerrainJsonComponent>();
		auto jsonFile = terrainJson->LoadJsonFile("json/terrain.json");
		auto terrainStatus = terrainJson->GetTerrainStatus();
		auto terrainKinds = terrainJson->GetTerrainKinds();

		str->LoadStageCSV("Stage1.csv", *stageRoadCSV); // ステージのCSVを読み込む
		teM->CreateTerrain(terrainStatus); // 読み込んだCSVからTerrainを生成

		
	}

	{
		auto player = GameObjectManager::AddObject("Player", "Player");
		player->AddComponent<TestMoveComponent>();

		auto cubeTrans = player->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 4.0f, 10.0f, 5.0f });
		cubeTrans->SetPosition({ -50.0f,-30.0f,0.0f });
		cubeTrans->SetRotation({ 0.0f, 0.0f, 0.0f });

		auto cubeJump = player->AddComponent<JumpComponent>();
		cubeJump->SetJumpPower(50.0f);

		auto cubeRigid = player->AddComponent<RigidBodyComponent>();
		cubeRigid->SetMass(2.0f);
		cubeRigid->SetGravityFlag(true);

		auto cubeColl = player->AddComponent<ColliderComponent>();
	//	cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		//		cubeColl->SetOffsetSizeOBB(DirectX::XMFLOAT3(20.0f, 5.0f, 0.0f));
		//		cubeColl->SetOffsetRotationOBB(DirectX::XMFLOAT3(0.0f, 0.0f, 45.0f));

		auto cubeCollEX = player->AddComponent<TestExtrusionComponent>();

		auto fighterPlayer = player->AddComponent<FighterComponent>();
		fighterPlayer->SetHp(50);
		fighterPlayer->SetMaxHp(50);
		fighterPlayer->SetAtk(10);

		SquareMesh cubeMesh;	// 四角形のメッシュ
		auto cubeRe = player->AddComponent<Render3DComponent>();
		cubeRe->SetMesh(cubeMesh);
		cubeRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe->SetTexture("assets/texture/NoTexture.png");

		CubeMesh cubeMesh2;
		auto cubeRe2 = player->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetMesh(cubeMesh2);
		cubeRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe2->SetTexture("assets/texture/NoTexture.png");
		cubeRe2->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));

		/*	CubeMesh cubeMesh3;
			auto cubeRe3 = player->AddComponent<Render3DColliderOBBComponent>();
			cubeRe3->SetMesh(cubeMesh3);
			cubeRe3->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
			cubeRe3->SetTexture("assets/texture/NoTexture.png");
			cubeRe3->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));*/

			/*auto child = GameObjectManager::AddChild("child", "Child");
			child->SetChild(child);

			auto childTrans = child->AddComponent<TransformComponent>();
			childTrans->SetLocalPosition({5.0f,0.0f,0.0f});
			childTrans->SetLocalScale({ 1.0f, 1.0f, 1.0f });
			childTrans->SetLocalRotation({ 0.0f, 0.0f, 45.0f });
			childTrans->MakeChildWorld();

			CubeMesh childMesh;
			auto childRender = child->AddComponent<Render3DComponent>();
			childRender->SetMesh(childMesh);
			childRender->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
			childRender->SetTexture("assets/texture/NoTexture.png");*/


		/*CubeMesh cubeMeshSword;
		CubeMesh cubeMeshSword2;
		auto sword = GameObjectManager::AddChild("sword", "Cube");

		player->SetChild(sword);

		auto swordTrans = sword->AddComponent<TransformComponent>();
		swordTrans->SetLocalScale({ 1.5f, 0.5f, 1.0f });
		swordTrans->SetLocalPosition({ 3.0f, 2.0f, 0.0f });
		swordTrans->SetLocalRotation({ 0.0f, 0.0f, 30.0f });
		swordTrans->MakeChildWorld();

		auto swordColl = sword->AddComponent<ColliderComponent>();
		swordColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));
		swordColl->SetOffsetSizeOBB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		auto swordAttack = sword->AddComponent<AttackComponent>();
		swordAttack->SetCoolDownTime(1.0f);

		auto fighter = sword->AddComponent<FighterComponent>();
		fighter->SetHp(100);
		fighter->SetAtk(10);

		sword->AddComponent<EnemyDamageComponent>();

		auto swordRe = sword->AddComponent<Render3DComponent>();
		swordRe->SetMesh(cubeMeshSword);
		swordRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		swordRe->SetTexture("assets/texture/NoTexture.png");

		auto swordRe2 = sword->AddComponent<Render3DColliderOBBComponent>();
		swordRe2->SetMesh(cubeMeshSword2);
		swordRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		swordRe2->SetTexture("assets/texture/NoTexture.png");
		swordRe2->SetColor(DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 0.5f));*/
	}


}

LoadStageScene::~LoadStageScene() {
	GameObjectManager::ListClear(); // ゲームオブジェクトのリストをクリア
}

void LoadStageScene::Update() {

}