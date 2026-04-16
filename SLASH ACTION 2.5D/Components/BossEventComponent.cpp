#include "BossEventComponent.h"
#include "CameraPointComponent.h"
#include "TransformComponent.h"
#include "Render2DComponent.h"
#include "Render3DComponent.h"
#include "Manager/GameObjectManager.h"
#include "Manager/TimeManager.h"
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
#include "SoundComponent.h"
#include "RenderTerrainComponent.h"
#include "PlayerOperationComponent.h"
#include "CameraShakeComponent.h"
#include "RenderHpComponent.h"
#include "Manager/HitStopManager.h"
#include "Manager/EventBusManager.h"

using namespace DirectX;

BossEventComponent::BossEventComponent(GameObject& obj) : Component(obj) {

	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE");	// 仮にテストムーブを
	m_TimeLine = m_Object->AddComponent<TimeLineComponent>();
}

void BossEventComponent::Update() {

	GameObject* obj = GameObjectManager::GameObjectFindNameUI("fade");

	if (obj == nullptr) {
		return;
	}
	DoorFadeComponent* fade = obj->GetComponent<DoorFadeComponent>();

	if (fade == nullptr) {
		return;
	}

	if (m_IsBossDied == true) {
		return;
	}

	CameraPointComponent* point = m_Object->GetComponent<CameraPointComponent>();
	if (point != nullptr)
	{
		if (point->GetScrollDir() == 1) {
			if (createCompletionFlag == false) {

				// ボス戦が始まる際の演出
				m_TimeLine->AddPointDelayEvent(0.0f, this, [this]() {CreateBossWalls(); });
				m_TimeLine->AddRangeDelayEvent(0.0f, 0.5f, 0.0f, this, [this](float) {PlayerMoveControl(); }, [this]() {PlayerControlStop(); }, nullptr);
				m_TimeLine->AddPointDelayEvent(2.0f, this, [this]() {CreateBossObj(); });
				m_TimeLine->AddPointDelayEvent(2.0f, this, [this]() {PlayerControlRestart(); });

				createCompletionFlag = true;
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

					if (m_IsBossDied == false) {


						m_TimeLine->AddPointDelayEvent(0.0f, this, [this]() {DeadCameraShakeAndScreenStop(); });
						m_TimeLine->AddPointDelayEvent(0.7f, this, [this]() {ScreenReStart(); });
						m_TimeLine->AddPointDelayEvent(3.0f, this, [this]() {TransferResultScene(); });

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
	transform->SetPosition({ 1080.0f, 80.0f, 10.0f });
	transform->SetScale({ 8.0f,60.0f,20.0f });

	ColliderComponent* collider = terrainObj->AddComponent<ColliderComponent>();

	TestExtrusionJudgeComponent* extrusion = terrainObj->AddComponent<TestExtrusionJudgeComponent>();

	RigidBodyComponent* terrainRigid = terrainObj->AddComponent<RigidBodyComponent>();
	terrainRigid->SetGravityFlag(true);
	terrainRigid->SetMass(2.0f);

	RenderTerrainComponent* rend = terrainObj->AddComponent<RenderTerrainComponent>();
	rend->ChangeTexture("testTerrain.png");
	rend->SetShader("TerrainVS.hlsl", "TerrainPS.hlsl");
	rend->SetUVMagnification(XMFLOAT3(0.1f, 0.1f, 0.1f));

}

void BossEventComponent::CreateBossObj() {

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

	TimeLineComponent* timeLine = boss->AddComponent<TimeLineComponent>();

	FighterComponent* fighterPlayer = boss->AddComponent<FighterComponent>();
	fighterPlayer->SetHp(50);
	fighterPlayer->SetMaxHp(50);
	fighterPlayer->SetAtk(10);

	MeshCut2DComponent* meshCut = boss->AddComponent<MeshCut2DComponent>();

	EnemyDeathEventComponent* deathEvent = boss->AddComponent<EnemyDeathEventComponent>();

	EnemyActionBossComponent* enemyAction = boss->AddComponent<EnemyActionBossComponent>();

	Render2DComponent* cubeRe = boss->AddComponent<Render2DComponent>();
	cubeRe->CreateMesh<SquareMesh>();
	cubeRe->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	cubeRe->SetColor(DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));

	m_boss = boss;

	SoundComponent* mySound = m_Object->GetComponent<SoundComponent>();

	if (mySound != nullptr) {
		mySound->Play();
		mySound->SetVolume(0.7f);
	}
}

void BossEventComponent::PlayerControlStop() {

	GameObject* player = GameObjectManager::GameObjectFindName("Player");

	if (player == nullptr) {
		return;
	}

	PlayerOperationComponent* operation = player->GetComponent<PlayerOperationComponent>();

	if (operation == nullptr) {
		return;
	}

	operation->SetActiveFlag(false);
}

void BossEventComponent::PlayerControlRestart() {

	GameObject* player = GameObjectManager::GameObjectFindName("Player");

	if (player == nullptr) {
		return;
	}

	PlayerOperationComponent* operation = player->GetComponent<PlayerOperationComponent>();

	if (operation == nullptr) {
		return;
	}

	operation->SetActiveFlag(true);
}

void BossEventComponent::PlayerMoveControl() {

	GameObject* player = GameObjectManager::GameObjectFindName("Player");

	if (player == nullptr) {
		return;
	}

	TransformComponent* trans = player->GetComponent<TransformComponent>();

	if (trans == nullptr) {
		return;
	}

	trans->AddPosition(XMFLOAT3(1.5f, 0.0f, 0.0f));

	GameObject* camera = GameObjectManager::GameObjectFindName("camera");
	if (camera == nullptr) {
		return;
	}

	SoundComponent* cameraSound = camera->GetComponent<SoundComponent>();
	if (cameraSound == nullptr) {
		return;
	}

	cameraSound->AddVolume(-0.05f);
}

void BossEventComponent::TransferResultScene() {

	GameObject* obj = GameObjectManager::GameObjectFindNameUI("fade");

	if (obj == nullptr) {
		return;
	}
	DoorFadeComponent* fade = obj->GetComponent<DoorFadeComponent>();

	if (fade == nullptr) {
		return;
	}

	fade->SetWinLoseFlag(true);
	fade->SetNextSceneName("ResultScene");
	fade->SetBootDoor(true);
}

void BossEventComponent::DeadCameraShakeAndScreenStop() {

	HitStopManager::SetIsHitStopActive(false);

	GameObject* camera = GameObjectManager::GameObjectFindName("camera");

	if (camera == nullptr) {
		return;
	}

	CameraShakeComponent* camShake = camera->GetComponent<CameraShakeComponent>();
	if (camShake != nullptr) {
		camShake->ShakingPreparation(150.0f, 4.0f, 0.4f);
		camShake->SetShakeType(ShakeType::RANDOM_2D);
	}

	std::vector<GameObject*> stopObjects = GameObjectManager::GameObjectFindAllTags("Player", "Sword", "Effect", "SkyDome", "Bullets");
	for (GameObject* obj : stopObjects) {

		obj->SetActiveState(ActiveState::UPDATE_STOP);
	}
}

void BossEventComponent::ScreenReStart() {

	std::vector<GameObject*> stopObjects = GameObjectManager::GameObjectFindAllTags("Player", "Sword", "Effect", "SkyDome", "Bullets");
	for (GameObject* obj : stopObjects) {

		obj->SetActiveState(ActiveState::ACTIVE);
	}

	GameObject* boss = GameObjectManager::GameObjectFindName("Boss");
	GameObject* hp_Boss = GameObjectManager::GameObjectFindNameUI("hpUI_Boss");

	if (boss == nullptr || hp_Boss == nullptr) {
		return;
	}

	RenderHpComponent* hpRender = hp_Boss->GetComponent<RenderHpComponent>();

	if (hpRender == nullptr) {
		return;
	}

	hpRender->ResetReferenceHPObj();

	DeathEvent de = { boss->GetInstanceID() };
	EventBusManager::Push(de);
}