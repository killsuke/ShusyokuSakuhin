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
#include "AttackTimingComponent.h"
#include "CameraMoveComponent.h"
#include "CameraPointComponent.h"
#include "CameraTargetComponent.h"
#include "GoAroundComponent.h"
#include "HPBarMoveComponent.h"
#include "Spring.h"
#include "StageLoadCSVComponent.h"
#include "TerrainManagerComponent.h"
#include "TerrainJsonComponent.h"
#include "EnemyJsonComponent.h"

LoadStageScene::LoadStageScene() {
	auto camera = GameObjectManager::AddObject("camera", "Camera");
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(DirectX::SimpleMath::Vector3(10.0f, 30.0f, -170.0f));
	camera->AddComponent<RigidBodyComponent>();
	auto cameraMove = camera->AddComponent<CameraMoveComponent>();
	auto cameraComp = camera->AddComponent<Camera>();
	cameraComp->SetTarget(DirectX::SimpleMath::Vector3(10.0f, 30.0f, 0.0f));

	camera->AddComponent<SpringComponent>();

	{
		auto stageRoadCSV = GameObjectManager::AddObject("StageRoadCSV", "StageRoadCSV");
		auto stageRoadCSVTrans = stageRoadCSV->AddComponent<TransformComponent>();	
		auto str = stageRoadCSV->AddComponent<StageLoadCSVComponent>();
		auto teM = stageRoadCSV->AddComponent<TerrainManagerComponent>();

		auto terrainJson = stageRoadCSV->AddComponent<TerrainJsonComponent>();
		terrainJson->LoadTerrainJsonFile("json/terrain.json");
		auto terrainStatus = terrainJson->GetTerrainStatus();
	//	auto terrainKinds = terrainJson->GetKindNames();

		auto enemyJson = stageRoadCSV->AddComponent<EnemyJsonComponent>();
		enemyJson->LoadEnemyJsonFile("json/enemy.json");
		auto enemyStatus = enemyJson->GetEnemyStatus();

	//	enemyJson->MakeSampleStatus(); // サンプルの敵キャラ情報を作成

		str->LoadStageCSV("Stage1.csv", *stageRoadCSV); // ステージのCSVを読み込む
		teM->CreateTerrain(terrainStatus); // 読み込んだCSVからTerrainを生成
	}

	{
		auto player = GameObjectManager::AddObject("Player", "Player");
		player->AddComponent<TestMoveComponent>();

		auto cubeTrans = player->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 4.0f, 10.0f, 5.0f });
		cubeTrans->SetPosition({ 0.0f,50.0f,0.0f });
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

	{
		auto target1 = GameObjectManager::AddObject("target1", "Target");
		auto targetTrans1 = target1->AddComponent<TransformComponent>();
		targetTrans1->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans1->SetPosition({ 10.0f, 30.0f, 0.0f });
		auto rigidTa1 = target1->AddComponent<RigidBodyComponent>();
		rigidTa1->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget1 = target1->AddComponent<CameraTargetComponent>();
		cameratarget1->SetCameraPattern(SPRING_CHASE);
		cameratarget1->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend1 = target1->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh1;
		targetRend1->SetMesh(sphereMesh1);
		targetRend1->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend1->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend1->SetTexture("assets/texture/NoTexture.png");


		auto target2 = GameObjectManager::AddObject("target2", "Target");
		auto targetTrans2 = target2->AddComponent<TransformComponent>();
		targetTrans2->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans2->SetPosition({ 240.0f, 30.0f, 0.0f });
		auto rigidTa2 = target2->AddComponent<RigidBodyComponent>();
		rigidTa2->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget2 = target2->AddComponent<CameraTargetComponent>();
		cameratarget2->SetCameraPattern(SPRING_CHASE);
		cameratarget2->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend2 = target2->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh2;
		targetRend2->SetMesh(sphereMesh2);
		targetRend2->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend2->SetTexture("assets/texture/NoTexture.png");

		auto target3 = GameObjectManager::AddObject("target3", "Target");
		auto targetTrans3 = target3->AddComponent<TransformComponent>();
		targetTrans3->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans3->SetPosition({ 240.0f, -120.0f, 0.0f });
		auto rigidTa3 = target3->AddComponent<RigidBodyComponent>();
		rigidTa3->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget3 = target3->AddComponent<CameraTargetComponent>();
		cameratarget3->SetCameraPattern(SPRING_CHASE);
		cameratarget3->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend3 = target3->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh3;
		targetRend3->SetMesh(sphereMesh3);
		targetRend3->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend3->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend3->SetTexture("assets/texture/NoTexture.png");

		//auto circle = GameObjectManager::AddObject("Scroll", "Scroll");
		//auto circleTrans = circle->AddComponent<TransformComponent>();
		//circleTrans->SetScale({ 30.0f, 30.0f, 10.0f });
		//circleTrans->SetPosition({ -80.0f, 60.0f, 0.0f });
		//auto rigidMe = circle->AddComponent<RigidBodyComponent>();
		//rigidMe->SetActiveFlag(true); // 物理演算を有効にする
		//auto circleRend = circle->AddComponent<Render3DComponent>();
		//CircleMesh circleMesh;
		//circleRend->SetMesh(circleMesh);
		//circleRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		//circleRend->SetTexture("assets/texture/NoTexture.png");

		//auto circleSpring = circle->AddComponent<SpringComponent>();
		//circleSpring->SetK(20.0f); // ばね定数をセット
		//circleSpring->MakeDamping(); // ダンピングを作成
		//circleSpring->SetSpringPartner(target);

		//cameraSpring->SetK(20.0f); // ばね定数をセット
		//cameraSpring->MakeDamping(); // ダンピングを作成
		//cameraSpring->SetSpringPartner(target);

		auto point1 = GameObjectManager::AddObject("CameraPoint1", "CameraPoint");
		auto pointTrans1 = point1->AddComponent<TransformComponent>();
		pointTrans1->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans1->AddPosition({ 120.0f, 60.0f, 0.0f });
		auto pointCamera1 = point1->AddComponent<CameraPointComponent>();
		auto pointColl1 = point1->AddComponent<ColliderComponent>();
		CubeMesh pointMesh1;
		auto pointRend1 = point1->AddComponent<Render3DColliderAABBComponent>();
		pointRend1->SetMesh(pointMesh1);
		pointRend1->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		pointRend1->SetTexture("assets/texture/NoTexture.png");
		pointRend1->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera1->SetBeforeAndNextTargetObj(*target1, *target2);
		pointCamera1->SetScrollDirection(SCROLL_IN_LEFT);

		auto point2 = GameObjectManager::AddObject("CameraPoint2", "CameraPoint");
		auto pointTrans2 = point2->AddComponent<TransformComponent>();
		pointTrans2->SetScale({ 30.0f, 8.0f, 10.0f });
		pointTrans2->AddPosition({ 305.0f, -30.0f, 0.0f });
		auto pointCamera2 = point2->AddComponent<CameraPointComponent>();
		auto pointColl2 = point2->AddComponent<ColliderComponent>();
		CubeMesh pointMesh2;
		auto pointRend2 = point2->AddComponent<Render3DColliderAABBComponent>();
		pointRend2->SetMesh(pointMesh2);
		pointRend2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		pointRend2->SetTexture("assets/texture/NoTexture.png");
		pointRend2->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera2->SetBeforeAndNextTargetObj(*target2, *target3);
		pointCamera2->SetScrollDirection(SCROLL_IN_UP);
	}


}

LoadStageScene::~LoadStageScene() {
	GameObjectManager::ListClear(); // ゲームオブジェクトのリストをクリア
}

void LoadStageScene::Update() {

}