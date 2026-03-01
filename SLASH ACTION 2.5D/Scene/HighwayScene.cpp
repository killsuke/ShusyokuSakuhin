#include "HighwayScene.h"
#include "Manager/GameObjectManager.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/CameraComponent.h"
#include "Components/CameraMoveComponent.h"
#include "Components/Render2DComponent.h"
#include "Components/Render3DComponent.h"
#include "Components/Render3DColliderAABBComponent.h"
#include "Components/SpringComponent.h"
#include "Mesh/CubeMesh.h"
#include "Mesh/SquareMesh.h"
#include "Components/TestExtrusionJudgeComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/PlayerOperationComponent.h"
#include "Components/JumpComponent.h"
#include "Components/FighterComponent.h"
#include "Components/LeapToCenterCarComponent.h"
#include "Components/MoveCarComponent.h"

using namespace DirectX;

HighwayScene::HighwayScene() {


	// 次は敵を配置させてこちらにジャンプ

//	TerrainsInFront();
	TerrainsCenter();
	TerrainsBack();

//	Enemeis();

	GameObject* player = GameObjectManager::AddObject("Player", "Player");
	TransformComponent* transPlayer = player->AddComponent<TransformComponent>();
	transPlayer->SetScale(XMFLOAT3(6.0f,10.0f,5.0f));
	RigidBodyComponent* rigidPlayer = player->AddComponent<RigidBodyComponent>();
	rigidPlayer->SetMass(2.0f);
	rigidPlayer->SetGravityFlag(true);
	Render2DComponent* renderPlayer = player->AddComponent<Render2DComponent>();
	renderPlayer->CreateMesh<SquareMesh>();
	renderPlayer->ChangeTexture("aka.png");
	renderPlayer->SetShader("ShaderResource/Animation2DVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	auto jumpPlayer = player->AddComponent<JumpComponent>();
	jumpPlayer->SetJumpPower(50.0f);
	player->AddComponent<PlayerOperationComponent>();
	player->AddComponent<TestExtrusionJudgeComponent>();
	auto colliderPlayer = player->AddComponent<ColliderComponent>();
	colliderPlayer->SetColliderSize_AABB(XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(6.0f, 10.0f, 5.0f));
	auto fighterPlayer = player->AddComponent<FighterComponent>();
	fighterPlayer->SetHp(50);
	fighterPlayer->SetMaxHp(50);
	fighterPlayer->SetAtk(10);
	fighterPlayer->SetUseInvincible(true);
	fighterPlayer->SetUseDeadFlag(false);
	player->AddComponent<Render3DColliderAABBComponent>();

	GameObject* camera = GameObjectManager::AddObject("camera", "Camera");
	TransformComponent* cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(XMFLOAT3(0.0f, 9.0f, -140.0f));
	camera->AddComponent<RigidBodyComponent>();
	CameraMoveComponent* cameraMove = camera->AddComponent<CameraMoveComponent>();
	CameraComponent* cameraComp = camera->AddComponent<CameraComponent>();
	cameraComp->SetTarget(XMFLOAT3(0.0f, 0.0f, 0.0f));

	camera->AddComponent<SpringComponent>();
}

HighwayScene::~HighwayScene() {

	GameObjectManager::OtherThanClear(); // 指定したタグ以外のゲームオブジェクトのリストをクリア
}

void HighwayScene::Update() {

}

void HighwayScene::Enemeis() {

	GameObject* enemy1 = GameObjectManager::AddObject("enemy1", "Enemy");
	TransformComponent* enemy1Trans = enemy1->AddComponent<TransformComponent>();
	enemy1Trans->SetPosition(XMFLOAT3(-100.0f, 0.0f, 100.0f));
	enemy1Trans->SetScale(XMFLOAT3(6.0f, 10.0f, 5.0f));
	RigidBodyComponent* enemy1Rigid = enemy1->AddComponent<RigidBodyComponent>();
	enemy1Rigid->SetMass(2.0f);
	enemy1Rigid->SetGravityFlag(true);
	Render2DComponent* enemy1Render = enemy1->AddComponent<Render2DComponent>();
	enemy1Render->CreateMesh<SquareMesh>();
	enemy1Render->ChangeTexture("hopper.png");
	enemy1Render->SetShader("ShaderResource/Animation2DVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	enemy1Render->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	enemy1->AddComponent<TestExtrusionJudgeComponent>();
	ColliderComponent* enemy1Coll = enemy1->AddComponent<ColliderComponent>();
	enemy1Coll->SetColliderSize_AABB(*enemy1Trans);
	FighterComponent* enemy1Fighter = enemy1->AddComponent<FighterComponent>();
	enemy1Fighter->SetHp(30);
	enemy1Fighter->SetAtk(5);
	JumpComponent* enemy1Jump = enemy1->AddComponent<JumpComponent>();
	//enemy1Jump->SetJumpPower(50.0f);
	//enemy1Jump->SetAttenuationUp(0.5f);
//	LeapToCenterCarComponent* enemy1Leap = enemy1->AddComponent<LeapToCenterCarComponent>();

	GameObject* enemy2 = GameObjectManager::AddObject("enemy2", "Enemy");
	TransformComponent* enemy2Trans = enemy2->AddComponent<TransformComponent>();
	enemy2Trans->SetPosition(XMFLOAT3(-100.0f, 0.0f, 100.0f));
	enemy2Trans->SetScale(XMFLOAT3(6.0f, 10.0f, 5.0f));
	RigidBodyComponent* enemy2Rigid = enemy2->AddComponent<RigidBodyComponent>();
	enemy2Rigid->SetMass(2.0f);
	enemy2Rigid->SetGravityFlag(true);
	Render2DComponent* enemy2Render = enemy2->AddComponent<Render2DComponent>();
	enemy2Render->CreateMesh<SquareMesh>();
	enemy2Render->ChangeTexture("hopper.png");
	enemy2Render->SetShader("ShaderResource/Animation2DVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	enemy2Render->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	enemy2->AddComponent<TestExtrusionJudgeComponent>();
	ColliderComponent* enemy2Coll = enemy2->AddComponent<ColliderComponent>();
	enemy2Coll->SetColliderSize_AABB(*enemy2Trans);
	FighterComponent* enemy2Fighter = enemy2->AddComponent<FighterComponent>();
	enemy2Fighter->SetHp(30);
	enemy2Fighter->SetAtk(5);
}

void HighwayScene::TerrainsInFront() {

}

void HighwayScene::TerrainsCenter() {

	auto terrain1 = GameObjectManager::AddObject("terrain1", "Terrain");
	auto terrain1Trans = terrain1->AddComponent<TransformComponent>();
	terrain1Trans->SetPosition(XMFLOAT3(0.0f, -25.0f, 0.0f));
	terrain1Trans->SetScale(XMFLOAT3(50.0f, 30.0f, 50.0f));
	auto terrain1Coll = terrain1->AddComponent<ColliderComponent>();
	terrain1Coll->SetColliderSize_AABB(*terrain1Trans);
	auto terrain1Rend = terrain1->AddComponent<Render3DComponent>();
	terrain1Rend->CreateMesh<CubeMesh>();
	terrain1Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	auto rigid1 = terrain1->AddComponent<RigidBodyComponent>();
	//	rigid1->SetLimitVelocity(XMFLOAT3(50.0f, 100.0f, 100.0f));
	rigid1->SetGravityFlag(false);
	//auto move1 = terrain1->AddComponent<MoveCarComponent>();
	//move1->SetMoveSpeed(20.0f);
	//Render3DColliderAABBComponent* terrain1CollRend = terrain1->AddComponent<Render3DColliderAABBComponent>();

	auto terrain2 = GameObjectManager::AddObject("terrain2", "Terrain");
	auto terrain2Trans = terrain2->AddComponent<TransformComponent>();
	terrain2Trans->SetPosition(XMFLOAT3(150.0f, -25.0f, 0.0f));
	terrain2Trans->SetScale(XMFLOAT3(50.0f, 30.0f, 50.0f));
	auto terrain2Coll = terrain2->AddComponent<ColliderComponent>();
	terrain2Coll->SetColliderSize_AABB(*terrain2Trans);
	auto terrain2Rend = terrain2->AddComponent<Render3DComponent>();
	terrain2Rend->CreateMesh<CubeMesh>();
	terrain2Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	auto rigid2 = terrain2->AddComponent<RigidBodyComponent>();
	rigid2->SetGravityFlag(false);

	auto terrain3 = GameObjectManager::AddObject("terrain3", "Terrain");
	auto terrain3Trans = terrain3->AddComponent<TransformComponent>();
	terrain3Trans->SetPosition(XMFLOAT3(300.0f, -35.0f, 0.0f));
	terrain3Trans->SetScale(XMFLOAT3(50.0f, 20.0f, 50.0f));
	auto terrain3Coll = terrain3->AddComponent<ColliderComponent>();
	terrain3Coll->SetColliderSize_AABB(*terrain3Trans);
	auto terrain3Rend = terrain3->AddComponent<Render3DComponent>();
	terrain3Rend->CreateMesh<CubeMesh>();
	terrain3Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	auto rigid3 = terrain3->AddComponent<RigidBodyComponent>();
	rigid3->SetGravityFlag(false);

	auto terrain4 = GameObjectManager::AddObject("terrain4", "Terrain");
	auto terrain4Trans = terrain4->AddComponent<TransformComponent>();
	terrain4Trans->SetPosition(XMFLOAT3(450.0f, -35.0f, 0.0f));
	terrain4Trans->SetScale(XMFLOAT3(50.0f, 20.0f, 50.0f));
	auto terrain4Coll = terrain4->AddComponent<ColliderComponent>();
	terrain4Coll->SetColliderSize_AABB(*terrain4Trans);
	auto terrain4Rend = terrain4->AddComponent<Render3DComponent>();
	terrain4Rend->CreateMesh<CubeMesh>();
	terrain4Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	auto rigid4 = terrain4->AddComponent<RigidBodyComponent>();
	rigid4->SetGravityFlag(false);

	{
		auto terrain5 = GameObjectManager::AddObject("terrain5", "Terrain");
		auto terrain5Trans = terrain5->AddComponent<TransformComponent>();
		terrain5Trans->SetPosition(XMFLOAT3(650.0f, -35.0f, 0.0f));
		terrain5Trans->SetScale(XMFLOAT3(100.0f, 20.0f, 50.0f));
		auto terrain5Coll = terrain5->AddComponent<ColliderComponent>();
		terrain5Coll->SetColliderSize_AABB(*terrain5Trans);
		auto terrain5Rend = terrain5->AddComponent<Render3DComponent>();
		terrain5Rend->CreateMesh<CubeMesh>();
		terrain5Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
		auto rigid5 = terrain5->AddComponent<RigidBodyComponent>();
		rigid5->SetGravityFlag(false);

		auto terrain6 = GameObjectManager::AddObject("terrain6", "Terrain");
		auto terrain6Trans = terrain6->AddComponent<TransformComponent>();
		terrain6Trans->SetPosition(XMFLOAT3(750.0f, -10.0f, 0.0f));
		terrain6Trans->SetScale(XMFLOAT3(25.0f, 50.0f, 50.0f));
		auto terrain6Coll = terrain6->AddComponent<ColliderComponent>();
		terrain6Coll->SetColliderSize_AABB(*terrain6Trans);
		auto terrain6Rend = terrain6->AddComponent<Render3DComponent>();
		terrain6Rend->CreateMesh<CubeMesh>();
		terrain6Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
		auto rigid6 = terrain6->AddComponent<RigidBodyComponent>();
		rigid6->SetGravityFlag(false);
	}

	auto terrain7 = GameObjectManager::AddObject("terrain7", "Terrain");
	auto terrain7Trans = terrain7->AddComponent<TransformComponent>();
	terrain7Trans->SetPosition(XMFLOAT3(950.0f, -10.0f, 0.0f));
	terrain7Trans->SetScale(XMFLOAT3(100.0f, 50.0f, 50.0f));
	auto terrain7Coll = terrain7->AddComponent<ColliderComponent>();
	terrain7Coll->SetColliderSize_AABB(*terrain7Trans);
	auto terrain7Rend = terrain7->AddComponent<Render3DComponent>();
	terrain7Rend->CreateMesh<CubeMesh>();
	terrain7Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	auto rigid7 = terrain7->AddComponent<RigidBodyComponent>();
	rigid7->SetGravityFlag(false);

	auto terrain8 = GameObjectManager::AddObject("terrain8", "Terrain");
	auto terrain8Trans = terrain8->AddComponent<TransformComponent>();
	terrain8Trans->SetPosition(XMFLOAT3(1250.0f, -10.0f, 0.0f));
	terrain8Trans->SetScale(XMFLOAT3(100.0f, 50.0f, 50.0f));
	auto terrain8Coll = terrain8->AddComponent<ColliderComponent>();
	terrain8Coll->SetColliderSize_AABB(*terrain8Trans);
	auto terrain8Rend = terrain8->AddComponent<Render3DComponent>();
	terrain8Rend->CreateMesh<CubeMesh>();
	terrain8Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	auto rigid8 = terrain8->AddComponent<RigidBodyComponent>();
	rigid8->SetGravityFlag(false);

	{
		auto terrain9 = GameObjectManager::AddObject("terrain9", "Terrain");
		auto terrain9Trans = terrain9->AddComponent<TransformComponent>();
		terrain9Trans->SetPosition(XMFLOAT3(1550.0f, -20.0f, 0.0f));
		terrain9Trans->SetScale(XMFLOAT3(100.0f, 20.0f, 50.0f));
		auto terrain9Coll = terrain9->AddComponent<ColliderComponent>();
		terrain9Coll->SetColliderSize_AABB(*terrain9Trans);
		auto terrain9Rend = terrain9->AddComponent<Render3DComponent>();
		terrain9Rend->CreateMesh<CubeMesh>();
		terrain9Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
		auto rigid9 = terrain9->AddComponent<RigidBodyComponent>();
		rigid9->SetGravityFlag(false);

		auto terrain10 = GameObjectManager::AddObject("terrain10", "Terrain");
		auto terrain10Trans = terrain10->AddComponent<TransformComponent>();
		terrain10Trans->SetPosition(XMFLOAT3(1650.0f, -10.0f, 0.0f));
		terrain10Trans->SetScale(XMFLOAT3(25.0f, 50.0f, 50.0f));
		auto terrain10Coll = terrain10->AddComponent<ColliderComponent>();
		terrain10Coll->SetColliderSize_AABB(*terrain10Trans);
		auto terrain10Rend = terrain10->AddComponent<Render3DComponent>();
		terrain10Rend->CreateMesh<CubeMesh>();
		terrain10Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
		auto rigid10 = terrain10->AddComponent<RigidBodyComponent>();
		rigid10->SetGravityFlag(false);
	}

	auto terrain11 = GameObjectManager::AddObject("terrain11", "Terrain");
	auto terrain11Trans = terrain11->AddComponent<TransformComponent>();
	terrain11Trans->SetPosition(XMFLOAT3(1850.0f, -35.0f, 0.0f));
	terrain11Trans->SetScale(XMFLOAT3(50.0f, 20.0f, 50.0f));
	auto terrain11Coll = terrain11->AddComponent<ColliderComponent>();
	terrain11Coll->SetColliderSize_AABB(*terrain11Trans);
	auto terrain11Rend = terrain11->AddComponent<Render3DComponent>();
	terrain11Rend->CreateMesh<CubeMesh>();
	terrain11Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	auto rigid11 = terrain11->AddComponent<RigidBodyComponent>();
	rigid11->SetGravityFlag(false);

	auto terrain12 = GameObjectManager::AddObject("terrain12", "Terrain");
	auto terrain12Trans = terrain12->AddComponent<TransformComponent>();
	terrain12Trans->SetPosition(XMFLOAT3(2050.0f, -10.0f, 0.0f));
	terrain12Trans->SetScale(XMFLOAT3(100.0f, 50.0f, 50.0f));
	auto terrain12Coll = terrain12->AddComponent<ColliderComponent>();
	terrain12Coll->SetColliderSize_AABB(*terrain12Trans);
	auto terrain12Rend = terrain12->AddComponent<Render3DComponent>();
	terrain12Rend->CreateMesh<CubeMesh>();
	terrain12Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	auto rigid12 = terrain12->AddComponent<RigidBodyComponent>();
	rigid12->SetGravityFlag(false);

}

void HighwayScene::TerrainsBack() {

	auto terrainBack1 = GameObjectManager::AddObject("terrainBack1", "Terrain");
	auto terrainBack1Trans = terrainBack1->AddComponent<TransformComponent>();
	terrainBack1Trans->SetPosition(XMFLOAT3(600.0f, -10.0f, 100.0f));
	terrainBack1Trans->SetScale(XMFLOAT3(100.0f, 50.0f, 50.0f));
	auto terrainBack1Coll = terrainBack1->AddComponent<ColliderComponent>();
	terrainBack1Coll->SetColliderSize_AABB(*terrainBack1Trans);
	auto terrainBack1Rend = terrainBack1->AddComponent<Render3DComponent>();
	terrainBack1Rend->CreateMesh<CubeMesh>();
	terrainBack1Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	terrainBack1Rend->SetColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	auto rigid1 = terrainBack1->AddComponent<RigidBodyComponent>();
	//	rigid2->SetLimitVelocity(XMFLOAT3(50.0f, 100.0f, 100.0f));
	rigid1->SetGravityFlag(false);
	/*auto move2 = terrain2->AddComponent<MoveCarComponent>();
	move2->SetMoveSpeed(21.0f);*/

	auto terrainBack2 = GameObjectManager::AddObject("terrainBack2", "Terrain");
	auto terrainBack2Trans = terrainBack2->AddComponent<TransformComponent>();
	terrainBack2Trans->SetPosition(XMFLOAT3(1550.0f, -10.0f, 100.0f));
	terrainBack2Trans->SetScale(XMFLOAT3(100.0f, 50.0f, 50.0f));
	auto terrainBack2Coll = terrainBack2->AddComponent<ColliderComponent>();
	terrainBack2Coll->SetColliderSize_AABB(*terrainBack2Trans);
	auto terrainBack2Rend = terrainBack2->AddComponent<Render3DComponent>();
	terrainBack2Rend->CreateMesh<CubeMesh>();
	terrainBack2Rend->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/litTexturePS.hlsl");
	terrainBack2Rend->SetColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	auto rigid2 = terrainBack2->AddComponent<RigidBodyComponent>();
	rigid2->SetGravityFlag(false);



}