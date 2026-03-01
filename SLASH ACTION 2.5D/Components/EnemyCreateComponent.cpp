#include "EnemyCreateComponent.h"
#include "TransformComponent.h"
#include "ColliderAttackComponent.h"
#include "ColliderDamageComponent.h"
#include "Render3DComponent.h"
#include "Render2DComponent.h"
#include "Render3DColliderAABBComponent.h"
#include "Render3DColliderOBBComponent.h"
#include "Manager/GameObjectManager.h"
#include "Mesh/SquareMesh.h"
#include "EnemyJsonComponent.h"
#include "RigidBodyComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include "FighterComponent.h"
#include "EnemyActionBulletComponent.h"
#include "EnemyActionHopperComponent.h"
#include "AttackTimingComponent.h"
#include "PlayerDamageComponent.h"
#include "HitFlashComponent.h"
#include "ProjectileMotionComponent.h"
#include "MeshCut2DComponent.h"
#include "EnemyDeathEventComponent.h"

using namespace DirectX;

EnemyCreateComponent::EnemyCreateComponent(GameObject& obj) : CSVObjectManagerComponent(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("CSV_OBJECT_MANAGER"); // ソート番号を設定
}

void EnemyCreateComponent::Update()
{
}

void EnemyCreateComponent::CreateEnemies(std::vector<EnemyStatus> status)
{
	// この中で生成する
	if (m_csvObjData.empty()) {
		return; // データがない場合は何もしない
	}
	unsigned int num = 0; // 生成したオブジェクトの数をカウント
	for (auto& data : m_csvObjData) {
		std::string kind = data.kind; // 敵の種類
		EnemyStatus eS;
		for (auto& e : status) {
			if (e.kind == kind) {
				eS.kind = e.kind;
				eS.scale = e.scale;
				eS.angle = e.angle;
				eS.atk = e.atk;
				eS.hp = e.hp;
				eS.texture = e.texture;
				eS.shaderVS = e.shaderVS;
				eS.shaderPS = e.shaderPS;
				break;
			}
		}

		std::string newName = "enemy_" + std::to_string(num);
		auto enemyObj = GameObjectManager::AddObject(newName, "Enemy");
		auto transform = enemyObj->AddComponent<TransformComponent>();
		transform->SetPosition({ data.position.x, data.position.y, 0.0f });
		transform->SetScale(eS.scale);
		transform->SetRotation(eS.angle);
		auto rigidBody = enemyObj->AddComponent<RigidBodyComponent>();
		rigidBody->SetGravityFlag(true); // 重力を有効にする
		enemyObj->AddComponent<TestExtrusionJudgeComponent>(); // 地面判定コンポーネントを追加

		CreateKind(eS.kind, *enemyObj);

		ProjectileMotionComponent* proj = enemyObj->AddComponent<ProjectileMotionComponent>();

		MeshCut2DComponent* meshCut = enemyObj->AddComponent<MeshCut2DComponent>();

		EnemyDeathEventComponent* deathEvent = enemyObj->AddComponent<EnemyDeathEventComponent>();
	//	deathEvent->SetEnemyDeathEventState(EnemyDeathEventState::STICKY);
	//	deathEvent->SetEnemyDeathEventState(EnemyDeathEventState::IMMEDIATE);

		auto fighter = enemyObj->AddComponent<FighterComponent>();
		fighter->SetUseDeadFlag(false);
		fighter->SetAtk(eS.atk);
		fighter->SetHp(eS.hp);

		auto collider = enemyObj->AddComponent<ColliderComponent>();
		collider->SetOffsetSizeAABB(XMFLOAT3(0.0f, 1.0f, 1.0f));
		ColliderAttackComponent* collAttack = enemyObj->AddComponent<ColliderAttackComponent>();
		ColliderDamageComponent* collDamage = enemyObj->AddComponent<ColliderDamageComponent>();

		auto render = enemyObj->AddComponent<Render2DComponent>();
		render->CreateMesh<SquareMesh>();
		render->SetShader(eS.shaderVS, eS.shaderPS);
		render->ChangeTexture(eS.texture);
		render->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		auto atk = enemyObj->AddComponent<AttackTimingComponent>();
		atk->SetCoolDownTime(0.1f);

		HitFlashComponent* hitFlash = enemyObj->AddComponent<HitFlashComponent>();
		hitFlash->SetHitFlashColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
		hitFlash->SetHitFlashPower(0.8f);

		
		// 当たり判定の可視化用（デバッグ用）
		/*auto renderColl = enemyObj->AddComponent<Render3DColliderAABBComponent>();
		renderColl->CreateMesh<SquareMesh>();
		renderColl->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		renderColl->ChangeTexture("NoTexture.png");
		renderColl->SetColor(Vector4(1.0f, 0.0f, 0.0f, 0.5f));*/

		num++;
	}
}

void EnemyCreateComponent::CreateKind(const std::string& kind, GameObject& obj)
{
	EnemyActionComponent* enemyAction = nullptr;

	if (kind == "E_Gunner") {
		enemyAction = obj.AddComponent<EnemyActionBulletComponent>();
	}
	else if(kind == "E_Hopper") {
		enemyAction = obj.AddComponent<EnemyActionHopperComponent>();
	}
	else if (kind == "E_Oku_No_TEKI") {
		enemyAction = obj.AddComponent<EnemyActionHopperComponent>();

		auto trans = obj.GetComponent<TransformComponent>();
		trans->SetPosition({ 0.0f, 10.0f, 15.0f });
	}

	PlayerDamageComponent* pd = obj.AddComponent<PlayerDamageComponent>();

	if (enemyAction != nullptr) {
		pd->SetEnemyActionComponent(enemyAction);
	}
}