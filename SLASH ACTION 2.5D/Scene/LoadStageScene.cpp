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

	GameObject* camera = GameObjectManager::AddObject("camera", "Camera");
	SoundComponent* sound = camera->AddComponent<SoundComponent>();
	sound->AddSoundLabel("game");
	sound->Play();
	sound->SetMaxVolume(0.8f);

	TransformComponent* cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(XMFLOAT3(10.0f, 30.0f, -70.0f));	// 0.45度 == 1.0f として計算して、視野角同士の引き算で計算するとか？
	//cameraTrans->SetPosition(XMFLOAT3(10.0f, 30.0f, -170.0f));
	camera->AddComponent<RigidBodyComponent>();
	CameraMoveComponent* cameraMove = camera->AddComponent<CameraMoveComponent>();
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
		skyRend->SetShader("unlitTextureVSSkyDome.hlsl", "unlitTexturePS.hlsl");
		skyRend->TextureLoadSkyDome(L"skyDome.dds");*/
	}

	FLOAT clearColor[4] = { 0.4f,0.75f, 1.0f, 1.0f };
	DirectXRender::SetClearColor(clearColor);

	BackGroundCreate();



	{
		GameObject* stageRoadCSV = GameObjectManager::AddObject("StageRoadCSV", "StageRoadCSV");
		TransformComponent* stageRoadCSVTrans = stageRoadCSV->AddComponent<TransformComponent>();
		StageLoadCSVComponent* str = stageRoadCSV->AddComponent<StageLoadCSVComponent>();
		TerrainCreateComponent* teM = stageRoadCSV->AddComponent<TerrainCreateComponent>();
		EnemyCreateComponent* enM = stageRoadCSV->AddComponent<EnemyCreateComponent>();

		TerrainJsonComponent* terrainJson = stageRoadCSV->AddComponent<TerrainJsonComponent>();
		terrainJson->LoadTerrainJsonFile("json/terrain.json");
		std::vector<TerrainStatus> terrainStatus = terrainJson->GetTerrainStatus();
		//	auto terrainKinds = terrainJson->GetKindNames();

		EnemyJsonComponent* enemyJson = stageRoadCSV->AddComponent<EnemyJsonComponent>();
		enemyJson->LoadEnemyJsonFile("json/enemy.json");
		std::vector<EnemyStatus> enemyStatus = enemyJson->GetEnemyStatus();

		//	enemyJson->MakeSampleStatus(); // サンプルの敵キャラ情報を作成

		str->LoadStageCSV("Stage1.csv", *stageRoadCSV); // ステージのCSVを読み込む
		teM->CreateTerrains(terrainStatus, 10.0f); // 読み込んだCSVからTerrainを生成
		enM->CreateEnemies(enemyStatus); // 読み込んだJSONからEnemyを生成
	}

	BamboosCreate();

	GameObject* playOBJ = nullptr;
	{
		GameObject* player = GameObjectManager::AddObject("Player", "Player");
		playOBJ = player;

		PlayerOperationComponent* playerOperation = player->AddComponent<PlayerOperationComponent>();

		TransformComponent* playerTrans = player->AddComponent<TransformComponent>();
		playerTrans->SetScale({ 6.0f, 10.0f, 5.0f });
		playerTrans->SetPosition({ 0.0f,-9.0f,0.0f });
		playerTrans->SetRotation({ 0.0f, 0.0f, 0.0f });

		JumpComponent* cubeJump = player->AddComponent<JumpComponent>();
		cubeJump->SetJumpPower(50.0f);

		RigidBodyComponent* cubeRigid = player->AddComponent<RigidBodyComponent>();
		cubeRigid->SetMass(2.0f);
		cubeRigid->SetGravityFlag(true);

		HitFlashComponent* hitFlash = player->AddComponent<HitFlashComponent>();
		hitFlash->SetHitFlashColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
		hitFlash->SetHitFlashPower(0.8f);

		player->AddComponent<TestExtrusionJudgeComponent>();

		ColliderComponent* cubeColl = player->AddComponent<ColliderComponent>();
		ColliderDamageComponent* collDamage = player->AddComponent<ColliderDamageComponent>();

		FighterComponent* fighterPlayer = player->AddComponent<FighterComponent>();
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

		RenderCharacterComponent* cubeRe = player->AddComponent<RenderCharacterComponent>();
		cubeRe->CreateMesh<SquareMesh>();
		cubeRe->SetShader("OverVertexMove3DVS.hlsl", "Fighter2DPS.hlsl");
		cubeRe->ChangeTexture("aka.png");

		// 剣のオブジェクト生成 =============================================================
		GameObject* rolling = GameObjectManager::AddObject("sword", "Sword");

		TransformComponent* rollingTrans = rolling->AddComponent<TransformComponent>();
		rollingTrans->SetScale({ 11.0f, 4.0f, 3.0f });
		rollingTrans->SetPosition({ 30.0f,-9.0f,3.0f });

		ColliderComponent* rollingColl = rolling->AddComponent<ColliderComponent>();
		rollingColl->SetOffsetSizeOBB({ -2.0f,0.0f,6.0f });
		rollingColl->SetOffsetCenterOBB({ 5.0f,0.0f,0.0f });
		ColliderAttackComponent* collAttack = rolling->AddComponent<ColliderAttackComponent>();
		collAttack->SetOffsetSizeOBB({ -2.0f,0.0f,6.0f });
		collAttack->SetOffsetCenterOBB({ 5.0f,0.0f,0.0f });

		TestSwordActionComponent* testAction = rolling->AddComponent<TestSwordActionComponent>();
		testAction->SetHolder(player);

		ArbitraryRotationComponent* rollingGoAround = rolling->AddComponent<ArbitraryRotationComponent>();
		rollingGoAround->SetCenterObject(player); // プレイヤーを中心に回るように設定
		rollingGoAround->MakeInitialOffset(playerTrans->GetPosition(), rollingTrans->GetPosition()); // 初期オフセットを設定
		rollingGoAround->SetRotationSpeed(20.0f); // 回転速度を設定
		rollingGoAround->SetRollingActive(false);
		rollingGoAround->SetClockwise(true);
		rollingGoAround->SetStopTime(0.2f);

		FighterComponent* rollingFT = rolling->AddComponent<FighterComponent>();
		rollingFT->SetAtk(5);
		rollingFT->SetHp(50);

		AttackOneTimeComponent* rollingAtk = rolling->AddComponent<AttackOneTimeComponent>();

		EnemyDamageComponent* rollingED = rolling->AddComponent<EnemyDamageComponent>();

		SoundComponent* rollingSound = rolling->AddComponent<SoundComponent>();
		rollingSound->AddSoundLabel("slash");
		rollingSound->AddSoundLabel("slashHit");

		TrailRenderComponent* effectRender = rolling->AddComponent<TrailRenderComponent>();
		effectRender->SetTipPoint(10.0f);
		effectRender->SetBasePoint(-1.5f);
		effectRender->ChangeTexture("baran.png");
				
		Render3DComponent* rollingRender = rolling->AddComponent<Render3DComponent>();
		rollingRender->CreateMesh<SquareMesh>();
		rollingRender->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		rollingRender->ChangeTexture("sword.png");


		Render3DColliderOBBComponent* rollingCollRend = rolling->AddComponent<Render3DColliderOBBComponent>();
		rollingCollRend->CreateMesh<CubeMesh>();
		rollingCollRend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		rollingCollRend->ChangeTexture("NoTexture.png");
		rollingCollRend->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));

		playerOperation->SetWeaponObject(rolling);

		
	}

	TargetAndScroolCreate();

	{
		GameObject* hp = GameObjectManager::AddUI("hpFrameUI", "HP_UI");
		TransformComponent* hpTrans = hp->AddComponent<TransformComponent>();
		hpTrans->SetPosition({ -570.0f, 150.0f, 0.0f });
		hpTrans->SetScale({ 45.0f, 150.0f, 1.0f });

		Render3DComponent* hpRender = hp->AddComponent<Render3DComponent>();
		hpRender->CreateMesh<SquareMesh>();
		hpRender->SetShader("unlitTextureVS2D.hlsl", "unlitTexturePS.hlsl");
		hpRender->ChangeTexture("hp_bar.png");
		hpRender->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	GameObject* hp = GameObjectManager::AddUI("hpUI", "HP_UI");
	TransformComponent* hpTrans = hp->AddComponent<TransformComponent>();
	hpTrans->SetPosition({ -570.0f, 120.0f, 0.0f });
	hpTrans->SetScale({ 30.0f, 1.0f, 1.0f });

	RenderHpComponent* hpBar = hp->AddComponent<RenderHpComponent>();

	hpBar->SetReferenceHPObj(*playOBJ);

	hpBar->CreateMesh<SquareMesh>();
	hpBar->SetShader("OverVertexMoveVS.hlsl", "unlitTexturePS.hlsl");
	hpBar->ChangeTexture("NoTexture.png");
	hpBar->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	std::vector<GameObject*> fade = GameObjectManager::GameObjectFindTagUI("FadeUI");
	if (!fade.empty()) {
		DoorFadeComponent* fadeUI = fade[0]->GetComponent<DoorFadeComponent>();
		fadeUI->SetBootDoor(true);
		//	fadeUI->SetNextSceneName();
	}
	else {
		GameObject* fadeUI = GameObjectManager::AddUI("fade", "FadeUI");
		fadeUI->SetCarryOverFlag(true);
		TransformComponent* fadeTrans = fadeUI->AddComponent<TransformComponent>();
		DoorFadeComponent* fadeAC = fadeUI->AddComponent<DoorFadeComponent>();
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
		GameObject* target1 = GameObjectManager::AddObject("target1", "Target");
		TransformComponent* targetTrans1 = target1->AddComponent<TransformComponent>();
		targetTrans1->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans1->SetPosition({ 10.0f, 30.0f, 0.0f });
		RigidBodyComponent* rigidTa1 = target1->AddComponent<RigidBodyComponent>();
		rigidTa1->SetActiveFlag(false); // 物理演算を無効にする
		CameraTargetComponent* cameratarget1 = target1->AddComponent<CameraTargetComponent>();
		cameratarget1->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget1->SetSpringK(30.0f); // ばね定数をセット
		Render3DComponent* targetRend1 = target1->AddComponent<Render3DComponent>();
#if _DEBUG

		targetRend1->CreateMesh<CircleMesh>();
		targetRend1->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend1->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		targetRend1->ChangeTexture("NoTexture.png");
#endif

		GameObject* target2 = GameObjectManager::AddObject("target2", "Target");
		TransformComponent* targetTrans2 = target2->AddComponent<TransformComponent>();
		targetTrans2->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans2->SetPosition({ 240.0f, 30.0f, 0.0f });
		RigidBodyComponent* rigidTa2 = target2->AddComponent<RigidBodyComponent>();
		rigidTa2->SetActiveFlag(false); // 物理演算を無効にする
		CameraTargetComponent* cameratarget2 = target2->AddComponent<CameraTargetComponent>();
		cameratarget2->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget2->SetSpringK(30.0f); // ばね定数をセット
		Render3DComponent* targetRend2 = target2->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend2->CreateMesh<CircleMesh>();
		targetRend2->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend2->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		targetRend2->ChangeTexture("NoTexture.png");
#endif

		GameObject* target3 = GameObjectManager::AddObject("target3", "Target");
		TransformComponent* targetTrans3 = target3->AddComponent<TransformComponent>();
		targetTrans3->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans3->SetPosition({ 240.0f, -120.0f, 0.0f });
		RigidBodyComponent* rigidTa3 = target3->AddComponent<RigidBodyComponent>();
		rigidTa3->SetActiveFlag(false); // 物理演算を無効にする
		CameraTargetComponent* cameratarget3 = target3->AddComponent<CameraTargetComponent>();
		cameratarget3->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget3->SetSpringK(30.0f); // ばね定数をセット
		Render3DComponent* targetRend3 = target3->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend3->CreateMesh<CircleMesh>();
		targetRend3->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend3->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		targetRend3->ChangeTexture("NoTexture.png");
#endif

		GameObject* target4 = GameObjectManager::AddObject("target4", "Target");
		TransformComponent* targetTrans4 = target4->AddComponent<TransformComponent>();
		targetTrans4->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans4->SetPosition({ 240.0f, -290.0f, 0.0f });
		RigidBodyComponent* rigidTa4 = target4->AddComponent<RigidBodyComponent>();
		rigidTa4->SetActiveFlag(false); // 物理演算を無効にする
		CameraTargetComponent* cameratarget4 = target4->AddComponent<CameraTargetComponent>();
		cameratarget4->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget4->SetSpringK(30.0f); // ばね定数をセット
		Render3DComponent* targetRend4 = target4->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend4->CreateMesh<CircleMesh>();
		targetRend4->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend4->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		targetRend4->ChangeTexture("NoTexture.png");
#endif

		GameObject* target5 = GameObjectManager::AddObject("target5", "Target");
		TransformComponent* targetTrans5 = target5->AddComponent<TransformComponent>();
		targetTrans5->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans5->SetPosition({ 450.0f, -290.0f, 0.0f });
		RigidBodyComponent* rigidTa5 = target5->AddComponent<RigidBodyComponent>();
		rigidTa5->SetActiveFlag(false); // 物理演算を無効にする
		CameraTargetComponent* cameratarget5 = target5->AddComponent<CameraTargetComponent>();
		cameratarget5->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget5->SetSpringK(30.0f); // ばね定数をセット
		Render3DComponent* targetRend5 = target5->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend5->CreateMesh<CircleMesh>();
		targetRend5->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend5->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		targetRend5->ChangeTexture("NoTexture.png");
#endif

		GameObject* target6 = GameObjectManager::AddObject("target6", "Target");
		TransformComponent* targetTrans6 = target6->AddComponent<TransformComponent>();
		targetTrans6->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans6->SetPosition({ 700.0f, -290.0f, 0.0f });
		RigidBodyComponent* rigidTa6 = target6->AddComponent<RigidBodyComponent>();
		rigidTa6->SetActiveFlag(false); // 物理演算を無効にする
		CameraTargetComponent* cameratarget6 = target6->AddComponent<CameraTargetComponent>();
		cameratarget6->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget6->SetSpringK(30.0f); // ばね定数をセット
		Render3DComponent* targetRend6 = target6->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend6->CreateMesh<CircleMesh>();
		targetRend6->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend6->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		targetRend6->ChangeTexture("NoTexture.png");
#endif

		GameObject* target7 = GameObjectManager::AddObject("target7", "Target");
		TransformComponent* targetTrans7 = target7->AddComponent<TransformComponent>();
		targetTrans7->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans7->SetPosition({ 950.0f, -290.0f, 0.0f });
		RigidBodyComponent* rigidTa7 = target7->AddComponent<RigidBodyComponent>();
		rigidTa7->SetActiveFlag(false); // 物理演算を無効にする
		CameraTargetComponent* cameratarget7 = target7->AddComponent<CameraTargetComponent>();
		cameratarget7->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget7->SetSpringK(30.0f); // ばね定数をセット
		Render3DComponent* targetRend7 = target7->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend7->CreateMesh<CircleMesh>();
		targetRend7->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend7->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		targetRend7->ChangeTexture("NoTexture.png");
#endif

		GameObject* target8 = GameObjectManager::AddObject("target8", "Target");
		TransformComponent* targetTrans8 = target8->AddComponent<TransformComponent>();
		targetTrans8->SetScale({ 10.0f, 10.0f, 10.0f });
		targetTrans8->SetPosition({ 1200.0f, -290.0f, 0.0f });
		RigidBodyComponent* rigidTa8 = target8->AddComponent<RigidBodyComponent>();
		rigidTa8->SetActiveFlag(false); // 物理演算を無効にする
		CameraTargetComponent* cameratarget8 = target8->AddComponent<CameraTargetComponent>();
		cameratarget8->SetCameraPattern(CameraPattern::SPRING_CHASE);
		cameratarget8->SetSpringK(30.0f); // ばね定数をセット
		Render3DComponent* targetRend8 = target8->AddComponent<Render3DComponent>();

#if _DEBUG

		targetRend8->CreateMesh<CircleMesh>();
		targetRend8->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		targetRend8->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		targetRend8->ChangeTexture("NoTexture.png");
#endif

		GameObject* point1 = GameObjectManager::AddObject("CameraPoint1", "CameraPoint");
		TransformComponent* pointTrans1 = point1->AddComponent<TransformComponent>();
		pointTrans1->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans1->AddPosition({ 120.0f, 50.0f, 0.0f });
		CameraPointComponent* pointCamera1 = point1->AddComponent<CameraPointComponent>();
		ColliderComponent* pointColl1 = point1->AddComponent<ColliderComponent>();

		Render3DColliderAABBComponent* pointRend1 = point1->AddComponent<Render3DColliderAABBComponent>();
		pointRend1->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera1->SetBeforeAndNextTargetObj(*target1, *target2);
		pointCamera1->SetScrollDirection(SCROLL_IN_LEFT);

		GameObject* point2 = GameObjectManager::AddObject("CameraPoint2", "CameraPoint");
		TransformComponent* pointTrans2 = point2->AddComponent<TransformComponent>();
		pointTrans2->SetScale({ 40.0f, 5.0f, 10.0f });
		pointTrans2->AddPosition({ 305.0f, -30.0f, 0.0f });
		CameraPointComponent* pointCamera2 = point2->AddComponent<CameraPointComponent>();
		ColliderComponent* pointColl2 = point2->AddComponent<ColliderComponent>();

		Render3DColliderAABBComponent* pointRend2 = point2->AddComponent<Render3DColliderAABBComponent>();
		pointRend2->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera2->SetBeforeAndNextTargetObj(*target2, *target3);
		pointCamera2->SetScrollDirection(SCROLL_IN_UP);

		GameObject* point3 = GameObjectManager::AddObject("CameraPoint3", "CameraPoint");
		TransformComponent* pointTrans3 = point3->AddComponent<TransformComponent>();
		pointTrans3->SetScale({ 30.0f, 6.0f, 10.0f });
		pointTrans3->AddPosition({ 160.0f, -180.0f, 0.0f });
		CameraPointComponent* pointCamera3 = point3->AddComponent<CameraPointComponent>();
		ColliderComponent* pointColl3 = point3->AddComponent<ColliderComponent>();

		Render3DColliderAABBComponent* pointRend3 = point3->AddComponent<Render3DColliderAABBComponent>();
		pointRend3->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera3->SetBeforeAndNextTargetObj(*target3, *target4);
		pointCamera3->SetScrollDirection(SCROLL_IN_UP);

		GameObject* point4 = GameObjectManager::AddObject("CameraPoint4", "CameraPoint");
		TransformComponent* pointTrans4 = point4->AddComponent<TransformComponent>();
		pointTrans4->SetScale({ 10.0f, 90.0f, 10.0f });
		pointTrans4->AddPosition({ 330.0f, -270.0f, 0.0f });
		CameraPointComponent* pointCamera4 = point4->AddComponent<CameraPointComponent>();
		ColliderComponent* pointColl4 = point4->AddComponent<ColliderComponent>();

		Render3DColliderAABBComponent* pointRend4 = point4->AddComponent<Render3DColliderAABBComponent>();
		pointRend4->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera4->SetBeforeAndNextTargetObj(*target4, *target5);
		pointCamera4->SetScrollDirection(SCROLL_IN_LEFT);

		GameObject* point5 = GameObjectManager::AddObject("CameraPoint5", "CameraPoint");
		TransformComponent* pointTrans5 = point5->AddComponent<TransformComponent>();
		pointTrans5->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans5->AddPosition({ 580.0f, -280.0f, 0.0f });
		CameraPointComponent* pointCamera5 = point5->AddComponent<CameraPointComponent>();
		ColliderComponent* pointColl5 = point5->AddComponent<ColliderComponent>();

		Render3DColliderAABBComponent* pointRend5 = point5->AddComponent<Render3DColliderAABBComponent>();
		pointRend5->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera5->SetBeforeAndNextTargetObj(*target5, *target6);
		pointCamera5->SetScrollDirection(SCROLL_IN_LEFT);

		GameObject* point6 = GameObjectManager::AddObject("CameraPoint6", "CameraPoint");
		TransformComponent* pointTrans6 = point6->AddComponent<TransformComponent>();
		pointTrans6->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans6->AddPosition({ 830.0f, -280.0f, 0.0f });
		CameraPointComponent* pointCamera6 = point6->AddComponent<CameraPointComponent>();
		ColliderComponent* pointColl6 = point6->AddComponent<ColliderComponent>();

		Render3DColliderAABBComponent* pointRend6 = point6->AddComponent<Render3DColliderAABBComponent>();
		pointRend6->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f));
		pointCamera6->SetBeforeAndNextTargetObj(*target6, *target7);
		pointCamera6->SetScrollDirection(SCROLL_IN_LEFT);

		GameObject* point7 = GameObjectManager::AddObject("CameraPoint7", "CameraPoint");
		TransformComponent* pointTrans7 = point7->AddComponent<TransformComponent>();
		pointTrans7->SetScale({ 8.0f, 80.0f, 10.0f });
		pointTrans7->AddPosition({ 1080.0f, -280.0f, 0.0f });
		CameraPointComponent* pointCamera7 = point7->AddComponent<CameraPointComponent>();
		ColliderComponent* pointColl7 = point7->AddComponent<ColliderComponent>();
		BossEventComponent* bossEvent = point7->AddComponent<BossEventComponent>();
		SoundComponent* bossSound = point7->AddComponent<SoundComponent>();
		bossSound->AddSoundLabel("battle");

		Render3DColliderAABBComponent* pointRend7 = point7->AddComponent<Render3DColliderAABBComponent>();
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

		rend->SetShader("litTextureVS.hlsl", "litTexturePS.hlsl");*/
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

		//rend->SetShader("litTextureVS.hlsl", "litTexturePS.hlsl");
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
		rend->ChangeTexture("bambooForest.png");
		rend->SetShader("unlitTextureVS.hlsl", "blurPS.hlsl");
		rend->SetBlurTextureSize(XMFLOAT2(700.0f, 700.0f));
	}

	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 60.0f,50.0f,150.0f });
		trans->SetScale({ 384.0f,216.0f,1.0f });
		RenderBlurComponent* rend = backGround->AddComponent<RenderBlurComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->ChangeTexture("bambooForest.png");
		rend->SetShader("unlitTextureVS.hlsl", "blurPS.hlsl");
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
		rend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
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
		rend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	}


	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 700.0f,-350.0f,400.0f });
		trans->SetScale({ 384.0f,216.0f,1.0f });
		RenderBlurComponent* rend = backGround->AddComponent<RenderBlurComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->ChangeTexture("bambooForest.png");
		rend->SetShader("unlitTextureVS.hlsl", "blurPS.hlsl");
		rend->SetBlurTextureSize(XMFLOAT2(500.0f, 500.0f));
	}

	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 800.0f,-350.0f,300.0f });
		trans->SetScale({ 384.0f,216.0f,1.0f });
		RenderBlurComponent* rend = backGround->AddComponent<RenderBlurComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->ChangeTexture("bambooForest.png");
		rend->SetShader("unlitTextureVS.hlsl", "blurPS.hlsl");
		rend->SetBlurTextureSize(XMFLOAT2(700.0f, 700.0f));
	}

	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 400.0f,-300.0f,200.0f });
		trans->SetScale({ 384.0f,216.0f,1.0f });
		RenderBlurComponent* rend = backGround->AddComponent<RenderBlurComponent>();
		rend->CreateMesh<SquareMesh>();
		rend->ChangeTexture("bambooForest.png");
		rend->SetShader("unlitTextureVS.hlsl", "blurPS.hlsl");
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
		rend->ChangeTexture("testTerrain.png");
		rend->SetShader("litTextureVS.hlsl", "litTexturePS.hlsl");
	}

	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 250.0f,-250.0f,30.0f });
		trans->SetScale({ 110.0f,90.0f,5.0f });
		Render3DComponent* rend = backGround->AddComponent<Render3DComponent>();
		rend->CreateMesh<CubeMesh>();
		rend->ChangeTexture("testTerrain.png");
		rend->SetShader("litTextureVS.hlsl", "litTexturePS.hlsl");
	}

	{
		GameObject* backGround = GameObjectManager::AddObject("backGround", "BackGround");
		TransformComponent* trans = backGround->AddComponent<TransformComponent>();
		trans->SetPosition({ 250.0f,-250.0f,30.0f });
		trans->SetScale({ 110.0f,90.0f,5.0f });
		Render3DComponent* rend = backGround->AddComponent<Render3DComponent>();
		rend->CreateMesh<CubeMesh>();
		rend->ChangeTexture("testTerrain.png");
		rend->SetShader("litTextureVS.hlsl", "litTexturePS.hlsl");
	}
}
