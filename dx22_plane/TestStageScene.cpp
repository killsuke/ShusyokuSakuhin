#include "TestStageScene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "CubeMesh.h"
#include "SquareMesh.h"
#include "CircleMesh.h"
#include "SphereMesh.h"
#include "Render3D.h"
#include "Render3DColliderAABBComponent.h"
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
#include "Spring.h"

TestStageScene::TestStageScene() {
	auto camera = GameObjectManager::AddObject("camera", "Camera");
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -200.0f));
	camera->AddComponent<RigidBodyComponent>();
	auto cameraMove = camera->AddComponent<CameraMoveComponent>();
	camera->AddComponent<Camera>();

	camera->AddComponent<SpringComponent>();

	{
		auto cube = GameObjectManager::AddObject("Player", "Player");
		cube->AddComponent<TestMoveComponent>();

		auto cubeTrans = cube->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 10.0f, 10.0f, 10.0f });
		cubeTrans->SetPosition({ 0.0f,30.0f,0.0f });

		auto cubeJump = cube->AddComponent<JumpComponent>();
		cubeJump->SetJumpPower(50.0f);

		auto cubeRigid = cube->AddComponent<RigidBodyComponent>();
		cubeRigid->SetMass(2.0f);

		auto cubeColl = cube->AddComponent<ColliderComponent>();
		cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		auto cubeCollEX = cube->AddComponent<TestExtrusionComponent>();

		CubeMesh cubeMesh;	// 四角形のメッシュ
		auto cubeRe = cube->AddComponent<Render3DComponent>();
		cubeRe->SetMesh(cubeMesh);
		cubeRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe->SetTexture("assets/texture/NoTexture.png");

		CubeMesh cubeMesh2;
		auto cubeRe2 = cube->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetMesh(cubeMesh2);
		cubeRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe2->SetTexture("assets/texture/NoTexture.png");
		cubeRe2->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));

		/*	CubeMesh cubeMesh3;
			auto sword = GameObjectManager::AddObject("sword", "Cube");
			auto swordTrans = sword->AddComponent<TransformComponent>();
			swordTrans->SetScale({ 10.0f, 20.0f, 10.0f });
			swordTrans->SetPosition({ 28.0f, 10.0f, 10.0f });
			auto swordColl = sword->AddComponent<ColliderComponent>();
			swordColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

			sword->AddComponent<AttackComponent>();

			auto fighter = sword->AddComponent<FighterComponent>();
			fighter->SetHp(100);
			fighter->SetAtk(10);

			sword->AddComponent<EnemyDamageComponent>();

			auto swordRe2 = sword->AddComponent<Render3DColliderAABBComponent>();
			swordRe2->SetMesh(cubeMesh3);
			swordRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
			swordRe2->SetTexture("assets/texture/NoTexture.png");
			swordRe2->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f));*/

	}

	{
		auto cube = GameObjectManager::AddObject("cube2", "Cube");

		auto cubeTrans = cube->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 100.0f, 20.0f, 10.0f });
		cubeTrans->SetPosition({ 0.0f, -20.0f, 0.0f });

		auto cubeColl = cube->AddComponent<ColliderComponent>();
		cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		CubeMesh cubeMesh;
		auto cubeRe = cube->AddComponent<Render3DComponent>();
		cubeRe->SetMesh(cubeMesh);
		cubeRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe->SetTexture("assets/texture/NoTexture.png");

		CubeMesh cubeMesh2;
		auto cubeRe2 = cube->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetMesh(cubeMesh2);
		cubeRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe2->SetTexture("assets/texture/NoTexture.png");
		cubeRe2->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f));
	}

	{
		auto cube = GameObjectManager::AddObject("cube3", "Cube");

		auto cubeTrans = cube->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 20.0f, 30.0f, 10.0f });
		cubeTrans->SetPosition({ -130.0f, -30.0f, 0.0f });

		auto cubeColl = cube->AddComponent<ColliderComponent>();
		cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		CubeMesh cubeMesh;
		auto cubeRe = cube->AddComponent<Render3DComponent>();
		cubeRe->SetMesh(cubeMesh);
		cubeRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe->SetTexture("assets/texture/NoTexture.png");

		CubeMesh cubeMesh2;
		auto cubeRe2 = cube->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetMesh(cubeMesh2);
		cubeRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe2->SetTexture("assets/texture/NoTexture.png");
		cubeRe2->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.5f));
	}

	/*{
		auto cube = GameObjectManager::AddObject("Enemy", "Enemy");

		auto cubeTrans = cube->AddComponent<TransformComponent>();
		cubeTrans->SetScale({ 20.0f, 30.0f, 10.0f });
		cubeTrans->SetPosition({ 100.0f, 40.0f, 0.0f });

		auto cubeColl = cube->AddComponent<ColliderComponent>();
		cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

		auto cubeFight = cube->AddComponent<FighterComponent>();
		cubeFight->SetHp(100);
		cubeFight->SetAtk(10);

		CubeMesh cubeMesh;
		auto cubeRe = cube->AddComponent<Render3DComponent>();
		cubeRe->SetMesh(cubeMesh);
		cubeRe->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe->SetTexture("assets/texture/NoTexture.png");

		CubeMesh cubeMesh2;
		auto cubeRe2 = cube->AddComponent<Render3DColliderAABBComponent>();
		cubeRe2->SetMesh(cubeMesh2);
		cubeRe2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		cubeRe2->SetTexture("assets/texture/NoTexture.png");
		cubeRe2->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 0.5f));
	}*/

	{
		auto target1 = GameObjectManager::AddObject("target1", "Target");
		auto targetTrans = target1->AddComponent<TransformComponent>();
		targetTrans->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans->SetPosition({ 100.0f, 60.0f, 0.0f });
		auto rigidTa = target1->AddComponent<RigidBodyComponent>();
		rigidTa->SetActiveFlag(false); // 物理演算を無効にする
		auto targetRend = target1->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh;
		targetRend->SetMesh(sphereMesh);
		targetRend->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend->SetTexture("assets/texture/NoTexture.png");


		auto target2 = GameObjectManager::AddObject("target2", "Target");
		auto targetTrans2 = target2->AddComponent<TransformComponent>();
		targetTrans2->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans2->SetPosition({ 0.0f, 60.0f, 0.0f });
		auto rigidTa2 = target2->AddComponent<RigidBodyComponent>();
		rigidTa2->SetActiveFlag(false); // 物理演算を無効にする
		auto targetRend2 = target2->AddComponent<Render3DComponent>();
		CircleMesh sphereMesh2;
		targetRend2->SetMesh(sphereMesh2);
		targetRend2->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		targetRend2->SetTexture("assets/texture/NoTexture.png");


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

		auto point = GameObjectManager::AddObject("CameraPoint", "CameraPoint");
		auto pointTrans = point->AddComponent<TransformComponent>();
		pointTrans->SetScale({ 10.0f, 80.0f, 10.0f });
		pointTrans->AddPosition({ 50.0f, 60.0f, 0.0f });
		auto pointCamera = point->AddComponent<CameraPointComponent>();
		auto pointColl = point->AddComponent<ColliderComponent>();
		pointColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));
		CubeMesh pointMesh;
		auto pointRend = point->AddComponent<Render3DColliderAABBComponent>();
		pointRend->SetMesh(pointMesh);
		pointRend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		pointRend->SetTexture("assets/texture/NoTexture.png");
		pointRend->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));

		pointCamera->SetCameraPattern(SPRING_CHASE);
		pointCamera->SetNextTargetObj(*target1);
		pointCamera->SetBeforeTargetObj(*target2);
		pointCamera->SetScrollDirection(SCROLL_IN_LEFT);

	}


}

TestStageScene::~TestStageScene() {
	GameObjectManager::ListClear(); // ゲームオブジェクトのリストをクリア
}

void TestStageScene::Update() {

}