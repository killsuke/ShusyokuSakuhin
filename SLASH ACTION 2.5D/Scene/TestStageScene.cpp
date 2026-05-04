#include "TestStageScene.h"
#include "Components/CameraComponent.h"
#include "Manager/GameObjectManager.h"
#include "Components/TransformComponent.h"
#include "Mesh/CubeMesh.h"
#include "Mesh/SquareMesh.h"
#include "Mesh/CircleMesh.h"
#include "Mesh/SphereMesh.h"
#include "Components/Render3DComponent.h"
#include "Components/Render3DColliderAABBComponent.h"
#include "Components/Render3DColliderOBBComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/PlayerOperationComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/JumpComponent.h"
#include "Components/EnemyDamageComponent.h"
#include "Components/FighterComponent.h"
#include "Components/AttackTimingComponent.h"
#include "Components/AttackOneTimeComponent.h"
#include "Components/CameraMoveComponent.h"
#include "Components/CameraPointComponent.h"
#include "Components/CameraTargetComponent.h"
#include "Components/HPBarMoveComponent.h"
#include "Components/TestExtrusionJudgeComponent.h"
#include "Components/SpringComponent.h"

TestStageScene::TestStageScene() {
	auto camera = GameObjectManager::AddObject("camera", "Camera");
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -200.0f));
	camera->AddComponent<RigidBodyComponent>();
	auto cameraMove = camera->AddComponent<CameraMoveComponent>();
	camera->AddComponent<CameraComponent>();

	camera->AddComponent<SpringComponent>();

	{
		auto player = GameObjectManager::AddObject("Player", "Player");
		player->AddComponent<PlayerOperationComponent>();

		auto cubeTrans = player->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 10.0f, 10.0f, 10.0f });
		cubeTrans->SetPosition({ 0.0f,100.0f,0.0f });
		cubeTrans->SetRotation({ 0.0f, 0.0f, 0.0f });

		auto cubeJump = player->AddComponent<JumpComponent>();
		cubeJump->SetJumpPower(50.0f);

		auto cubeRigid = player->AddComponent<RigidBodyComponent>();
		cubeRigid->SetMass(2.0f);
		cubeRigid->SetGravityFlag(true);

		player->AddComponent<TestExtrusionJudgeComponent>();

		auto cubeColl = player->AddComponent<ColliderComponent>();
		cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));
		//		cubeColl->SetOffsetSizeOBB(DirectX::XMFLOAT3(20.0f, 5.0f, 0.0f));
		//		cubeColl->SetOffsetRotationOBB(DirectX::XMFLOAT3(0.0f, 0.0f, 45.0f));

		auto fighterPlayer = player->AddComponent<FighterComponent>();
		fighterPlayer->SetHp(50);
		fighterPlayer->SetMaxHp(50);
		fighterPlayer->SetAtk(10);

		auto cubeRe = player->AddComponent<Render3DComponent>();
		
		auto cubeRe2 = player->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));

		/*	CubeMesh cubeMesh3;
			auto cubeRe3 = player->AddComponent<Render3DColliderOBBComponent>();
			cubeRe3->SetMesh(cubeMesh3);
			cubeRe3->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
			cubeRe3->SetTexture("NoTexture.png");
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
			childRender->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
			childRender->SetTexture("NoTexture.png");*/

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

		auto swordAttack = sword->AddComponent<AttackTimingComponent>();
		swordAttack->SetCoolDownTime(1.0f);

		auto fighter = sword->AddComponent<FighterComponent>();
		fighter->SetHp(100);
		fighter->SetAtk(10);

		sword->AddComponent<EnemyDamageComponent>();

		auto swordRe = sword->AddComponent<Render3DComponent>();
		swordRe->CreateMesh<CubeMesh>();
		swordRe->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		swordRe->ChangeTexture("NoTexture.png");

		auto swordRe2 = sword->AddComponent<Render3DColliderOBBComponent>();
		swordRe2->CreateMesh<CubeMesh>();
		swordRe2->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		swordRe2->ChangeTexture("NoTexture.png");
		swordRe2->SetColor(DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 0.5f));
	}

	{
		auto cube = GameObjectManager::AddObject("cube2", "Terrain");

		auto cubeTrans = cube->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 100.0f, 20.0f, 10.0f });
		cubeTrans->SetPosition({ 0.0f, -20.0f, 0.0f });

		auto cubeColl = cube->AddComponent<ColliderComponent>();
		cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		auto cubeRe = cube->AddComponent<Render3DComponent>();
		cubeRe->CreateMesh<CubeMesh>();
		cubeRe->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		cubeRe->ChangeTexture("NoTexture.png");

		auto cubeRe2 = cube->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f));
	}

	{
		auto cube = GameObjectManager::AddObject("cube3", "Terrain");

		auto cubeTrans = cube->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 20.0f, 10.0f, 10.0f });
		cubeTrans->SetPosition({ -60.0f, 60.0f, 0.0f });

		auto cubeColl = cube->AddComponent<ColliderComponent>();
		cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));
		cubeColl->SetOffsetSizeOBB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		auto cubeRe = cube->AddComponent<Render3DComponent>();
		cubeRe->CreateMesh<CubeMesh>();
		cubeRe->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		cubeRe->ChangeTexture("NoTexture.png");

		auto cubeRe2 = cube->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f));
	}

	{
		auto cube = GameObjectManager::AddObject("cube4", "Terrain");

		auto cubeTrans = cube->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 20.0f, 10.0f, 10.0f });
		cubeTrans->SetPosition({ -60.0f, 20.0f, 0.0f });

		auto cubeColl = cube->AddComponent<ColliderComponent>();
		cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));
		cubeColl->SetOffsetSizeOBB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		auto cubeRe = cube->AddComponent<Render3DComponent>();
		cubeRe->CreateMesh<CubeMesh>();
		cubeRe->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		cubeRe->ChangeTexture("NoTexture.png");

		auto cubeRe2 = cube->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f));
	}

	{
		auto cube = GameObjectManager::AddObject("Enemy", "Enemy");

		auto cubeTrans = cube->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 20.0f, 30.0f, 10.0f });
		cubeTrans->SetPosition({ 100.0f, 40.0f, 0.0f });

		auto cubeColl = cube->AddComponent<ColliderComponent>();
		cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		auto cubeFight = cube->AddComponent<FighterComponent>();
		cubeFight->SetHp(100);
		cubeFight->SetAtk(10);

		auto cubeRe = cube->AddComponent<Render3DComponent>();
		cubeRe->CreateMesh<CubeMesh>();
		cubeRe->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		cubeRe->ChangeTexture("NoTexture.png");

		auto cubeRe2 = cube->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 0.5f));
	}

	{
		auto target1 = GameObjectManager::AddObject("target1", "Target");
		auto targetTrans = target1->AddComponent<TransformComponent>();
		targetTrans->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans->SetPosition({ 100.0f, 60.0f, 0.0f });
		auto rigidTa = target1->AddComponent<RigidBodyComponent>();
		rigidTa->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget1 = target1->AddComponent<CameraTargetComponent>();
		cameratarget1->SetCameraPattern(CameraPattern::SPRING_CHASE);
		auto targetRend = target1->AddComponent<Render3DComponent>();
		targetRend->CreateMesh<CircleMesh>();
		targetRend->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		targetRend->ChangeTexture("NoTexture.png");


		auto target2 = GameObjectManager::AddObject("target2", "Target");
		auto targetTrans2 = target2->AddComponent<TransformComponent>();
		targetTrans2->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans2->SetPosition({ 0.0f, 60.0f, 0.0f });
		auto rigidTa2 = target2->AddComponent<RigidBodyComponent>();
		rigidTa2->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget2 = target2->AddComponent<CameraTargetComponent>();
		cameratarget2->SetCameraPattern(CameraPattern::SPRING_CHASE);
		auto targetRend2 = target2->AddComponent<Render3DComponent>();
		targetRend2->CreateMesh<CircleMesh>();
		targetRend2->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend2->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		targetRend2->ChangeTexture("NoTexture.png");


		//auto circle = GameObjectManager::AddObject("Scroll", "Scroll");
		//auto circleTrans = circle->AddComponent<TransformComponent>();
		//circleTrans->SetScale({ 30.0f, 30.0f, 10.0f });
		//circleTrans->SetPosition({ -80.0f, 60.0f, 0.0f });
		//auto rigidMe = circle->AddComponent<RigidBodyComponent>();
		//rigidMe->SetActiveFlag(true); // 物理演算を有効にする
		//auto circleRend = circle->AddComponent<Render3DComponent>();
		//CircleMesh circleMesh;
		//circleRend->SetMesh(circleMesh);
		//circleRend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		//circleRend->SetTexture("NoTexture.png");

		//auto circleSpring = circle->AddComponent<SpringComponent>();
		//circleSpring->SetK(20.0f); // ばね定数をセット
		//circleSpring->MakeDamping(); // ダンピングを作成
		//circleSpring->SetSpringPartner(target);

		//cameraSpring->SetK(20.0f); // ばね定数をセット
		//cameraSpring->MakeDamping(); // ダンピングを作成
		//cameraSpring->SetSpringPartner(target);

		auto point = GameObjectManager::AddObject("CameraPoint", "CameraPoint");
		auto pointTrans = point->AddComponent<TransformComponent>();
		pointTrans->SetScale({ 10.0f, 80.0f, 10.0f });
		pointTrans->AddPosition({ 50.0f, 60.0f, 0.0f });
		auto pointCamera = point->AddComponent<CameraPointComponent>();
		auto pointColl = point->AddComponent<ColliderComponent>();
		pointColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));
		auto pointRend = point->AddComponent<Render3DColliderAABBComponent>();
		pointRend->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));

		pointCamera->SetNextTargetObj(*target1);
		pointCamera->SetBeforeTargetObj(*target2);
		pointCamera->SetScrollDirection(SCROLL_IN_LEFT);

	}

	// UIの設定
	{
		auto hpUnderlay = GameObjectManager::AddUI("hpUI_Back", "HP_UI");
		auto hpUnderlayTrans = hpUnderlay->AddComponent<TransformComponent>();
		hpUnderlayTrans->SetPosition({ -570.0f, 195.0f, 10.0f });
		hpUnderlayTrans->SetScale({ 40.0f, 145.0f, 1.0f });

		auto hpUnderlayRender = hpUnderlay->AddComponent<Render3DComponent>();
		hpUnderlayRender->CreateMesh<SquareMesh>();
		hpUnderlayRender->SetShader("unlitTextureVS2D.hlsl", "unlitTexturePS.hlsl");
		hpUnderlayRender->ChangeTexture("NoTexture.png");
		hpUnderlayRender->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

		auto hp = GameObjectManager::AddUI("hpUI", "HP_UI");
		auto hpTrans = hp->AddComponent<TransformComponent>();
		hpTrans->SetPosition({ -570.0f, 70.0f, 0.0f });
		hpTrans->SetScale({ 30.0f, 1.0f, 1.0f });

		auto hpBar = hp->AddComponent<HPBarMoveComponent>();
		auto player = GameObjectManager::GameObjectFindName("Player");
		hpBar->SetReferenceHPObj(*player);

		auto hpRender = hp->AddComponent<Render3DComponent>();
		hpRender->CreateMesh<SquareMesh>();
		hpRender->SetShader("OverVertexMoveVS.hlsl", "unlitTexturePS.hlsl");
		hpRender->ChangeTexture("NoTexture.png");
		hpRender->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	}

}

TestStageScene::~TestStageScene() {
	GameObjectManager::ListClear(); // ゲームオブジェクトのリストをクリア
}

void TestStageScene::Update() {

}