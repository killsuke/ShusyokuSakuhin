#include "LoadStageScene.h"
#include "Components/CameraComponent.h"
#include "Manager/GameObjectManager.h"
#include "Components/TransformComponent.h"
#include "Mesh/CubeMesh.h"
#include "Mesh/SquareMesh.h"
#include "Mesh/CircleMesh.h"
#include "Mesh/SphereMesh.h"
#include "Mesh/PlaneMesh.h"
#include "Components/Render3DComponent.h"
#include "Components/Render3DColliderAABBComponent.h"
#include "Components/Render3DColliderOBBComponent.h"
#include "Components/Render2DComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/PlayerOperationComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/JumpComponent.h"
#include "Components/EnemyDamageComponent.h"
#include "Components/StretchingComponent.h"
#include "Components/FighterComponent.h"
#include "Components/AttackTimingComponent.h"
#include "Components/AttackOneTimeComponent.h"
#include "Components/CameraMoveComponent.h"
#include "Components/CameraPointComponent.h"
#include "Components/CameraTargetComponent.h"
#include "Components/CameraShakeComponent.h"
#include "Components/ArbitraryRotationComponent.h"
#include "Components/RenderHpComponent.h"
#include "Components/SpringComponent.h"
#include "Components/StageLoadCSVComponent.h"
#include "Components/TerrainCreateComponent.h"
#include "Components/EnemyCreateComponent.h"
#include "Components/TerrainJsonComponent.h"
#include "Components/EnemyJsonComponent.h"
#include "Components/TestExtrusionJudgeComponent.h"
#include "Mesh/SkyDomeMesh.h"
#include "Components/SkyDomeRenderComponent.h"
#include "Components/TestSwordActionComponent.h"
#include "Components/DoorFadeComponent.h"
#include "Components/BossEventComponent.h"
#include "Components/RenderBlurComponent.h"
#include "Manager/ModelManager.h"
#include "Components/HitFlashComponent.h"
#include "Components/TrailRenderComponent.h"
#include "Components/RenderMotionBlurComponent.h"
#include "Components/ChargePerformanceComponent.h"
#include "Components/ColliderAttackComponent.h"
#include "Components/ColliderDamageComponent.h"
#include "Components/SoundComponent.h"
#include "Components/RenderCharacterComponent.h"

using namespace DirectX;

LoadStageScene::LoadStageScene() {
	auto camera = GameObjectManager::AddObject("camera", "Camera");
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(XMFLOAT3(10.0f, 30.0f, -70.0f));	// 0.45度 == 1.0f として計算して、視野角同士の引き算で計算するとか？
	//cameraTrans->SetPosition(XMFLOAT3(10.0f, 30.0f, -170.0f));
	camera->AddComponent<RigidBodyComponent>();
	auto cameraMove = camera->AddComponent<CameraMoveComponent>();
	CameraComponent* cameraComp = camera->AddComponent<CameraComponent>();
	cameraComp->SetTarget(XMFLOAT3(10.0f, 30.0f, 100.0f));
	//cameraComp->SetTarget(XMFLOAT3(10.0f, 30.0f, 0.0f));
	cameraComp->SetFieldOfView(FieldOfView::WIDE);
	//cameraComp->SetFieldOfView(FieldOfView::DEFAULT);
	CameraShakeComponent* cameraShake = camera->AddComponent<CameraShakeComponent>();

	camera->AddComponent<SpringComponent>();

	{
		/*auto skydome = GameObjectManager::AddObject("skyDome", "SkyDome");
		auto skyTrans = skydome->AddComponent<TransformComponent>();
		skyTrans->SetScale({ 1000.0f,500.0f,1000.0f });
		auto skyRend = skydome->AddComponent<SkyDomeRenderComponent>();
		skyRend->CreateMesh<SkyDomeMesh>();
		skyRend->SetShader("ShaderResource/unlitTextureVSSkyDome.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		skyRend->TextureLoadSkyDome(L"assets/texture/skyDome.dds");*/
	}

	FLOAT clearColor[4] = { 0.4f,0.75f, 1.0f, 1.0f };
	DirectXRender::SetClearColor(clearColor);

	BackGroundCreate();



	{
		auto stageRoadCSV = GameObjectManager::AddObject("StageRoadCSV", "StageRoadCSV");
		auto stageRoadCSVTrans = stageRoadCSV->AddComponent<TransformComponent>();
		auto str = stageRoadCSV->AddComponent<StageLoadCSVComponent>();
		auto teM = stageRoadCSV->AddComponent<TerrainCreateComponent>();
		auto enM = stageRoadCSV->AddComponent<EnemyCreateComponent>();

		auto terrainJson = stageRoadCSV->AddComponent<TerrainJsonComponent>();
		terrainJson->LoadTerrainJsonFile("json/terrain.json");
		auto terrainStatus = terrainJson->GetTerrainStatus();
		//	auto terrainKinds = terrainJson->GetKindNames();

		auto enemyJson = stageRoadCSV->AddComponent<EnemyJsonComponent>();
		enemyJson->LoadEnemyJsonFile("json/enemy.json");
		auto enemyStatus = enemyJson->GetEnemyStatus();

		//	enemyJson->MakeSampleStatus(); // サンプルの敵キャラ情報を作成

		str->LoadStageCSV("Stage1.csv", *stageRoadCSV); // ステージのCSVを読み込む
		teM->CreateTerrains(terrainStatus, 10.0f); // 読み込んだCSVからTerrainを生成
		enM->CreateEnemies(enemyStatus); // 読み込んだJSONからEnemyを生成
	}

	BamboosCreate();

	GameObject* playOBJ = nullptr;
	{
		/*GameObject* childSlash = GameObjectManager::AddChild("dashSlash", "Child");
		TransformComponent* childTrans = childSlash->AddComponent<TransformComponent>();
		childTrans->SetLocalScale({5.0f,1.0f,1.0f});
		ColliderComponent* childColl = childSlash->AddComponent<ColliderComponent>();
		AttackOneTimeComponent* childAtk = childSlash->AddComponent<AttackOneTimeComponent>();
		EnemyDamageComponent* childED = childSlash->AddComponent<EnemyDamageComponent>();
		FighterComponent* childFT = childSlash->AddComponent<FighterComponent>();
		childFT->SetAtk(10);
		childFT->SetHp(5);
		Render3DColliderAABBComponent* childRend = childSlash->AddComponent<Render3DColliderAABBComponent>();
		childSlash->SetActiveState(ActiveState::ALL_STOP);*/

		auto player = GameObjectManager::AddObject("Player", "Player");
		playOBJ = player;

		PlayerOperationComponent* playerOperation = player->AddComponent<PlayerOperationComponent>();
	//	playerOperation->SetChargeSlashObject(childSlash);


		auto playerTrans = player->AddComponent<TransformComponent>();
		playerTrans->SetScale({ 6.0f, 10.0f, 5.0f });
		playerTrans->SetPosition({ 0.0f,-9.0f,0.0f });
		playerTrans->SetRotation({ 0.0f, 0.0f, 0.0f });

		auto cubeJump = player->AddComponent<JumpComponent>();
		cubeJump->SetJumpPower(50.0f);

		auto cubeRigid = player->AddComponent<RigidBodyComponent>();
		cubeRigid->SetMass(2.0f);
		cubeRigid->SetGravityFlag(true);

		HitFlashComponent* hitFlash = player->AddComponent<HitFlashComponent>();
		hitFlash->SetHitFlashColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
		hitFlash->SetHitFlashPower(0.8f);

		player->AddComponent<TestExtrusionJudgeComponent>();

		auto cubeColl = player->AddComponent<ColliderComponent>();
		//	cubeColl->SetOffsetSizeAABB(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
			//		cubeColl->SetOffsetSizeOBB(DirectX::XMFLOAT3(20.0f, 5.0f, 0.0f));
			//		cubeColl->SetOffsetRotationOBB(DirectX::XMFLOAT3(0.0f, 0.0f, 45.0f));
		ColliderDamageComponent* collDamage = player->AddComponent<ColliderDamageComponent>();

		auto fighterPlayer = player->AddComponent<FighterComponent>();
		fighterPlayer->SetHp(50);
		fighterPlayer->SetMaxHp(50);
		fighterPlayer->SetAtk(10);
		fighterPlayer->SetUseInvincible(true);
		fighterPlayer->SetUseDeadFlag(false);

		StretchingComponent* stretching = player->AddComponent<StretchingComponent>();
		stretching->SetStretchPower(0.1f);
		stretching->SetStretchSpeed(1.0f);

		ChargePerformanceComponent* chargePerf = player->AddComponent<ChargePerformanceComponent>();
		chargePerf->InitCreateParticles(10, 20.0f,*player,50.0f,10.0f,20.0f);
		chargePerf->SetOneParticleActiveTime(0.05f);
		chargePerf->SetActiveFlag(false);

		auto cubeRe = player->AddComponent<RenderCharacterComponent>();
		cubeRe->CreateMesh<SquareMesh>();
		cubeRe->SetShader("ShaderResource/OverVertexMove3DVS.hlsl", "ShaderResource/Fighter2DPS.hlsl");
		cubeRe->ChangeTexture("assets/texture/aka.png");

		//player->SetChild(childSlash);

		/*	auto cubeRe2 = player->AddComponent<Render3DColliderAABBComponent>();
			cubeRe2->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));*/
	
			//	auto rolling = GameObjectManager::AddObject("rolling", "Sword");

		// 剣のオブジェクト生成 =============================================================
		auto rolling = GameObjectManager::AddObject("sword", "Sword");

		auto rollingTrans = rolling->AddComponent<TransformComponent>();
		rollingTrans->SetScale({ 11.0f, 4.0f, 3.0f });
		rollingTrans->SetPosition({ 30.0f,-9.0f,3.0f });
		//	rollingTrans->SetRotation({ 90.0f,0.0f,0.0f });

		auto rollingColl = rolling->AddComponent<ColliderComponent>();
		rollingColl->SetOffsetSizeOBB({ -2.0f,0.0f,6.0f });
		rollingColl->SetOffsetCenterOBB({ 5.0f,0.0f,0.0f });
		ColliderAttackComponent* collAttack = rolling->AddComponent<ColliderAttackComponent>();
		collAttack->SetOffsetSizeOBB({ -2.0f,0.0f,6.0f });
		collAttack->SetOffsetCenterOBB({ 5.0f,0.0f,0.0f });

		auto testAction = rolling->AddComponent<TestSwordActionComponent>();
		testAction->SetHolder(player);

		auto rollingGoAround = rolling->AddComponent<ArbitraryRotationComponent>();
		rollingGoAround->SetCenterObject(player); // プレイヤーを中心に回るように設定
		rollingGoAround->MakeInitialOffset(playerTrans->GetPosition(), rollingTrans->GetPosition()); // 初期オフセットを設定
		rollingGoAround->SetRotationSpeed(20.0f); // 回転速度を設定
		rollingGoAround->SetRollingActive(false);
		rollingGoAround->SetClockwise(true);
		rollingGoAround->SetStopTime(0.2f);

		auto rollingFT = rolling->AddComponent<FighterComponent>();
		rollingFT->SetAtk(5);
		rollingFT->SetHp(50);

		auto rollingAtk = rolling->AddComponent<AttackOneTimeComponent>();

		auto rollingED = rolling->AddComponent<EnemyDamageComponent>();

		SoundComponent* rollingSound = rolling->AddComponent<SoundComponent>();
		rollingSound->AddSoundLabel("slash");
		rollingSound->AddSoundLabel("slashHit");

		auto effectRender = rolling->AddComponent<TrailRenderComponent>();
		effectRender->SetTipPoint(10.0f);
		effectRender->SetBasePoint(-1.5f);
		effectRender->ChangeTexture("assets/texture/baran.png");
		//	effectRender->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		//	auto rollingRender = rolling->AddComponent<Render3DComponent>();
				
		Render3DComponent* rollingRender = rolling->AddComponent<Render3DComponent>();
		rollingRender->CreateMesh<SquareMesh>();
		rollingRender->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		rollingRender->ChangeTexture("assets/texture/sword.png");

		//	rollingRender->SetBlurVelocity({ 0.0f,0.0f });

		auto rollingCollRend = rolling->AddComponent<Render3DColliderOBBComponent>();
		rollingCollRend->CreateMesh<CubeMesh>();
		rollingCollRend->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		rollingCollRend->ChangeTexture("assets/texture/NoTexture.png");
		rollingCollRend->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));

		playerOperation->SetWeaponObject(rolling);

		
	}

	TargetAndScroolCreate();

	{
		auto hp = GameObjectManager::AddUI("hpFrameUI", "HP_UI");
		auto hpTrans = hp->AddComponent<TransformComponent>();
		hpTrans->SetPosition({ -570.0f, 150.0f, 0.0f });
		hpTrans->SetScale({ 45.0f, 150.0f, 1.0f });

		auto hpRender = hp->AddComponent<Render3DComponent>();
		hpRender->CreateMesh<SquareMesh>();
		hpRender->SetShader("ShaderResource/unlitTextureVS2D.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		hpRender->ChangeTexture("assets/texture/hp_bar.png");
		hpRender->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	auto hp = GameObjectManager::AddUI("hpUI", "HP_UI");
	auto hpTrans = hp->AddComponent<TransformComponent>();
	hpTrans->SetPosition({ -570.0f, 120.0f, 0.0f });
	hpTrans->SetScale({ 30.0f, 1.0f, 1.0f });

	RenderHpComponent* hpBar = hp->AddComponent<RenderHpComponent>();

	hpBar->SetReferenceHPObj(*playOBJ);

	hpBar->CreateMesh<SquareMesh>();
	hpBar->SetShader("ShaderResource/OverVertexMoveVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	hpBar->ChangeTexture("assets/texture/NoTexture.png");
	hpBar->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	auto fade = GameObjectManager::GameObjectFindTagUI("FadeUI");
	if (!fade.empty()) {
		auto fadeUI = fade[0]->GetComponent<DoorFadeComponent>();
		fadeUI->SetBootDoor(true);
		//	fadeUI->SetNextSceneName();
	}
	else {
		auto fadeUI = GameObjectManager::AddUI("fade", "FadeUI");
		fadeUI->SetCarryOverFlag(true);
		auto fadeTrans = fadeUI->AddComponent<TransformComponent>();
		auto fadeAC = fadeUI->AddComponent<DoorFadeComponent>();
		fadeAC->SetNextSceneName("LoadStageScene");
	}
}

LoadStageScene::~LoadStageScene() {
	GameObjectManager::OtherThanClear(); // 指定したタグ以外のゲームオブジェクトのリストをクリア
}

void LoadStageScene::Update() {

}

void LoadStageScene::TargetAndScroolCreate() {

	{
		auto target1 = GameObjectManager::AddObject("target1", "Target");
		auto targetTrans1 = target1->AddComponent<TransformComponent>();
		targetTrans1->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans1->SetPosition({ 10.0f, 30.0f, 0.0f });
		auto rigidTa1 = target1->AddComponent<RigidBodyComponent>();
		rigidTa1->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget1 = target1->AddComponent<CameraTargetComponent>();
		cameratarget1->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget1->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend1 = target1->AddComponent<Render3DComponent>();
#if _DEBUG

		targetRend1->CreateMesh<CircleMesh>();
		targetRend1->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend1->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		targetRend1->ChangeTexture("assets/texture/NoTexture.png");
#endif

		auto target2 = GameObjectManager::AddObject("target2", "Target");
		auto targetTrans2 = target2->AddComponent<TransformComponent>();
		targetTrans2->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans2->SetPosition({ 240.0f, 30.0f, 0.0f });
		auto rigidTa2 = target2->AddComponent<RigidBodyComponent>();
		rigidTa2->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget2 = target2->AddComponent<CameraTargetComponent>();
		cameratarget2->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget2->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend2 = target2->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend2->CreateMesh<CircleMesh>();
		targetRend2->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend2->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		targetRend2->ChangeTexture("assets/texture/NoTexture.png");
#endif

		auto target3 = GameObjectManager::AddObject("target3", "Target");
		auto targetTrans3 = target3->AddComponent<TransformComponent>();
		targetTrans3->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans3->SetPosition({ 240.0f, -120.0f, 0.0f });
		auto rigidTa3 = target3->AddComponent<RigidBodyComponent>();
		rigidTa3->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget3 = target3->AddComponent<CameraTargetComponent>();
		cameratarget3->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget3->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend3 = target3->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend3->CreateMesh<CircleMesh>();
		targetRend3->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend3->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		targetRend3->ChangeTexture("assets/texture/NoTexture.png");
#endif

		auto target4 = GameObjectManager::AddObject("target4", "Target");
		auto targetTrans4 = target4->AddComponent<TransformComponent>();
		targetTrans4->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans4->SetPosition({ 240.0f, -290.0f, 0.0f });
		auto rigidTa4 = target4->AddComponent<RigidBodyComponent>();
		rigidTa4->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget4 = target4->AddComponent<CameraTargetComponent>();
		cameratarget4->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget4->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend4 = target4->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend4->CreateMesh<CircleMesh>();
		targetRend4->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend4->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		targetRend4->ChangeTexture("assets/texture/NoTexture.png");
#endif

		auto target5 = GameObjectManager::AddObject("target5", "Target");
		auto targetTrans5 = target5->AddComponent<TransformComponent>();
		targetTrans5->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans5->SetPosition({ 450.0f, -290.0f, 0.0f });
		auto rigidTa5 = target5->AddComponent<RigidBodyComponent>();
		rigidTa5->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget5 = target5->AddComponent<CameraTargetComponent>();
		cameratarget5->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget5->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend5 = target5->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend5->CreateMesh<CircleMesh>();
		targetRend5->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend5->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		targetRend5->ChangeTexture("assets/texture/NoTexture.png");
#endif

		auto target6 = GameObjectManager::AddObject("target6", "Target");
		auto targetTrans6 = target6->AddComponent<TransformComponent>();
		targetTrans6->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans6->SetPosition({ 700.0f, -290.0f, 0.0f });
		auto rigidTa6 = target6->AddComponent<RigidBodyComponent>();
		rigidTa6->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget6 = target6->AddComponent<CameraTargetComponent>();
		cameratarget6->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget6->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend6 = target6->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend6->CreateMesh<CircleMesh>();
		targetRend6->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend6->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		targetRend6->ChangeTexture("assets/texture/NoTexture.png");
#endif

		auto target7 = GameObjectManager::AddObject("target7", "Target");
		auto targetTrans7 = target7->AddComponent<TransformComponent>();
		targetTrans7->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans7->SetPosition({ 950.0f, -290.0f, 0.0f });
		auto rigidTa7 = target7->AddComponent<RigidBodyComponent>();
		rigidTa7->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget7 = target7->AddComponent<CameraTargetComponent>();
		cameratarget7->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget7->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend7 = target7->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend7->CreateMesh<CircleMesh>();
		targetRend7->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend7->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		targetRend7->ChangeTexture("assets/texture/NoTexture.png");
#endif

		auto target8 = GameObjectManager::AddObject("target8", "Target");
		auto targetTrans8 = target8->AddComponent<TransformComponent>();
		targetTrans8->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans8->SetPosition({ 1200.0f, -290.0f, 0.0f });
		auto rigidTa8 = target8->AddComponent<RigidBodyComponent>();
		rigidTa8->SetActiveFlag(false); // 物理演算を無効にする
		auto cameratarget8 = target8->AddComponent<CameraTargetComponent>();
		cameratarget8->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget8->SetSpringK(30.0f); // ばね定数をセット
		auto targetRend8 = target8->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend8->CreateMesh<CircleMesh>();
		targetRend8->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend8->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		targetRend8->ChangeTexture("assets/texture/NoTexture.png");
#endif

		auto point1 = GameObjectManager::AddObject("CameraPoint1", "CameraPoint");
		auto pointTrans1 = point1->AddComponent<TransformComponent>();
		pointTrans1->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans1->AddPosition({ 120.0f, 50.0f, 0.0f });
		auto pointCamera1 = point1->AddComponent<CameraPointComponent>();
		auto pointColl1 = point1->AddComponent<ColliderComponent>();

		auto pointRend1 = point1->AddComponent<Render3DColliderAABBComponent>();
		pointRend1->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera1->SetBeforeAndNextTargetObj(*target1, *target2);
		pointCamera1->SetScrollDirection(SCROLL_IN_LEFT);

		auto point2 = GameObjectManager::AddObject("CameraPoint2", "CameraPoint");
		auto pointTrans2 = point2->AddComponent<TransformComponent>();
		pointTrans2->SetScale({ 40.0f, 5.0f, 10.0f });
		pointTrans2->AddPosition({ 305.0f, -30.0f, 0.0f });
		auto pointCamera2 = point2->AddComponent<CameraPointComponent>();
		auto pointColl2 = point2->AddComponent<ColliderComponent>();

		auto pointRend2 = point2->AddComponent<Render3DColliderAABBComponent>();
		pointRend2->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera2->SetBeforeAndNextTargetObj(*target2, *target3);
		pointCamera2->SetScrollDirection(SCROLL_IN_UP);

		auto point3 = GameObjectManager::AddObject("CameraPoint3", "CameraPoint");
		auto pointTrans3 = point3->AddComponent<TransformComponent>();
		pointTrans3->SetScale({ 30.0f, 6.0f, 10.0f });
		pointTrans3->AddPosition({ 160.0f, -180.0f, 0.0f });
		auto pointCamera3 = point3->AddComponent<CameraPointComponent>();
		auto pointColl3 = point3->AddComponent<ColliderComponent>();

		auto pointRend3 = point3->AddComponent<Render3DColliderAABBComponent>();
		pointRend3->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera3->SetBeforeAndNextTargetObj(*target3, *target4);
		pointCamera3->SetScrollDirection(SCROLL_IN_UP);

		auto point4 = GameObjectManager::AddObject("CameraPoint4", "CameraPoint");
		auto pointTrans4 = point4->AddComponent<TransformComponent>();
		pointTrans4->SetScale({ 10.0f, 90.0f, 10.0f });
		pointTrans4->AddPosition({ 330.0f, -270.0f, 0.0f });
		auto pointCamera4 = point4->AddComponent<CameraPointComponent>();
		auto pointColl4 = point4->AddComponent<ColliderComponent>();

		auto pointRend4 = point4->AddComponent<Render3DColliderAABBComponent>();
		pointRend4->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera4->SetBeforeAndNextTargetObj(*target4, *target5);
		pointCamera4->SetScrollDirection(SCROLL_IN_LEFT);

		auto point5 = GameObjectManager::AddObject("CameraPoint5", "CameraPoint");
		auto pointTrans5 = point5->AddComponent<TransformComponent>();
		pointTrans5->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans5->AddPosition({ 580.0f, -280.0f, 0.0f });
		auto pointCamera5 = point5->AddComponent<CameraPointComponent>();
		auto pointColl5 = point5->AddComponent<ColliderComponent>();

		auto pointRend5 = point5->AddComponent<Render3DColliderAABBComponent>();
		pointRend5->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera5->SetBeforeAndNextTargetObj(*target5, *target6);
		pointCamera5->SetScrollDirection(SCROLL_IN_LEFT);

		auto point6 = GameObjectManager::AddObject("CameraPoint6", "CameraPoint");
		auto pointTrans6 = point6->AddComponent<TransformComponent>();
		pointTrans6->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans6->AddPosition({ 830.0f, -280.0f, 0.0f });
		auto pointCamera6 = point6->AddComponent<CameraPointComponent>();
		auto pointColl6 = point6->AddComponent<ColliderComponent>();

		auto pointRend6 = point6->AddComponent<Render3DColliderAABBComponent>();
		pointRend6->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera6->SetBeforeAndNextTargetObj(*target6, *target7);
		pointCamera6->SetScrollDirection(SCROLL_IN_LEFT);

		auto point7 = GameObjectManager::AddObject("CameraPoint7", "CameraPoint");
		auto pointTrans7 = point7->AddComponent<TransformComponent>();
		pointTrans7->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans7->AddPosition({ 1080.0f, -280.0f, 0.0f });
		auto pointCamera7 = point7->AddComponent<CameraPointComponent>();
		auto pointColl7 = point7->AddComponent<ColliderComponent>();
		auto bossEvent = point7->AddComponent<BossEventComponent>();

		auto pointRend7 = point7->AddComponent<Render3DColliderAABBComponent>();
		pointRend7->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera7->SetBeforeAndNextTargetObj(*target7, *target8);
		pointCamera7->SetScrollDirection(SCROLL_IN_LEFT);
	}
}

void LoadStageScene::BamboosCreate() {

	{
		/*auto testObj = GameObjectManager::AddObject("testObj", "Terrain");
		auto testTrans = testObj->AddComponent<TransformComponent>();
		testTrans->SetPosition({ -90.0f, 10.0f, 10.0f });
		testTrans->SetScale({ 3.0f,3.0f,80.0f });
		testTrans->SetRotation({ 90.0f,0.0f,0.0f });
		auto collider = testObj->AddComponent<ColliderComponent>();
		collider->SetOffsetSizeAABB({ 5.0f,80.0f,-70.0f });
		collider->SetOffsetCenterAABB({ 0.0f,30.0f,0.0f });

		auto collRend = testObj->AddComponent<Render3DColliderAABBComponent>();
		auto rend = testObj->AddComponent<Render3DComponent>();
		rend->LoadModelMesh("assets/model/bamboos/bambooRod1.fbx",
			"assets/model/bamboos");

		rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");*/
	}

	{
		//auto testObj = GameObjectManager::AddObject("testObj", "Test");
		//auto testTrans = testObj->AddComponent<TransformComponent>();
		//testTrans->SetPosition({ 30.0f, -20.0f, 20.0f });
		//testTrans->SetScale({ 60.0f,60.0f,60.0f });
		//testTrans->SetRotation({ 0.0f,45.0f,0.0f });
		////	auto collider = testObj->AddComponent<ColliderComponent>();
		////	collider->SetOffsetSizeAABB({ 10.0f,10.0f,10.0f });
		//auto collRend = testObj->AddComponent<Render3DColliderAABBComponent>();
		//auto rend = testObj->AddComponent<Render3DComponent>();
		//rend->LoadModelMesh("assets/model/bamboos/bambooGrass1.fbx",
		//	"assets/model/bamboos");

		//rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	}


}

void LoadStageScene::BackGroundCreate() {


	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ -40.0f,80.0f,200.0f });
		trans->SetScale({ 384.0f,216.0f,1.0f });
		RenderBlurComponent* rend = backGround->AddComponent<RenderBlurComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->ChangeTexture("assets/texture/bambooForest.png");
		rend->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/blurPS.hlsl");
		rend->SetBlurTextureSize(XMFLOAT2(700.0f, 700.0f));
	}

	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 60.0f,50.0f,150.0f });
		trans->SetScale({ 384.0f,216.0f,1.0f });
		RenderBlurComponent* rend = backGround->AddComponent<RenderBlurComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->ChangeTexture("assets/texture/bambooForest.png");
		rend->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/blurPS.hlsl");
		rend->SetBlurTextureSize(XMFLOAT2(900.0f, 900.0f));
	}

	{
		GameObject* rock1 = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = rock1->AddComponent<TransformComponent>();
		trans->SetPosition({ 700.0f,-500.0f,150.0f });
		trans->SetScale({ 100.0f,200.0f,500.0f });
		trans->SetRotation({0.0f,90.0f,0.0f});
		Render3DComponent* rend = rock1->AddComponent<Render3DComponent>();
		rend->CreateMesh<StaticMesh>();
		rend->LoadModelMesh("assets/model/rock9/rock9_HIGH_RES.fbx",
			"assets/model/rock9");
		rend->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	}

	{
		GameObject* rock1 = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = rock1->AddComponent<TransformComponent>();
		trans->SetPosition({ 670.0f,-500.0f,150.0f });
		trans->SetScale({ 100.0f,200.0f,500.0f });
		trans->SetRotation({ 0.0f,-90.0f,0.0f });
		Render3DComponent* rend = rock1->AddComponent<Render3DComponent>();
		rend->CreateMesh<StaticMesh>();
		rend->LoadModelMesh("assets/model/rock9/rock9_HIGH_RES.fbx",
			"assets/model/rock9");
		rend->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	}


	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 700.0f,-350.0f,400.0f });
		trans->SetScale({ 384.0f,216.0f,1.0f });
		RenderBlurComponent* rend = backGround->AddComponent<RenderBlurComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->ChangeTexture("assets/texture/bambooForest.png");
		rend->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/blurPS.hlsl");
		rend->SetBlurTextureSize(XMFLOAT2(500.0f, 500.0f));
	}

	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 800.0f,-350.0f,300.0f });
		trans->SetScale({ 384.0f,216.0f,1.0f });
		RenderBlurComponent* rend = backGround->AddComponent<RenderBlurComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->ChangeTexture("assets/texture/bambooForest.png");
		rend->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/blurPS.hlsl");
		rend->SetBlurTextureSize(XMFLOAT2(700.0f, 700.0f));
	}

	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 400.0f,-300.0f,200.0f });
		trans->SetScale({ 384.0f,216.0f,1.0f });
		RenderBlurComponent* rend = backGround->AddComponent<RenderBlurComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->ChangeTexture("assets/texture/bambooForest.png");
		rend->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/blurPS.hlsl");
		rend->SetBlurTextureSize(XMFLOAT2(900.0f, 900.0f));
	}

	// 設置物をさらに増やす
	// 三角形と五角形メッシュを作成

	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 230.0f,-110.0f,30.0f });
		trans->SetScale({ 110.0f,90.0f,5.0f });
		Render3DComponent* rend = backGround->AddComponent<Render3DComponent>();
		rend->CreateMesh<CubeMesh>();
		rend->ChangeTexture("assets/texture/testTerrain.png");
		rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	}

	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 250.0f,-250.0f,30.0f });
		trans->SetScale({ 110.0f,90.0f,5.0f });
		Render3DComponent* rend = backGround->AddComponent<Render3DComponent>();
		rend->CreateMesh<CubeMesh>();
		rend->ChangeTexture("assets/texture/testTerrain.png");
		rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	}

	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 250.0f,-250.0f,30.0f });
		trans->SetScale({ 110.0f,90.0f,5.0f });
		Render3DComponent* rend = backGround->AddComponent<Render3DComponent>();
		rend->CreateMesh<CubeMesh>();
		rend->ChangeTexture("assets/texture/testTerrain.png");
		rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	}
}
