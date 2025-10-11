#include "EnemyManagerComponent.h"
#include "Transform.h"
#include "Collider.h"
#include "Render3D.h"
#include "Render2D.h"
#include "Render3DColliderAABBComponent.h"
#include "Render3DColliderOBBComponent.h"
#include "GameObjectManager.h"
#include "SquareMesh.h"
#include "EnemyJsonComponent.h"
#include "RigidBodyComponent.h"
#include "TestExtrusionJudgeComponent.h"
#include "FighterComponent.h"
#include "EnemyActionBulletComponent.h"
#include "EnemyActionHopperComponent.h"
#include "AttackTimingComponent.h"
#include "PlayerDamageComponent.h"

using namespace DirectX::SimpleMath;

EnemyManagerComponent::EnemyManagerComponent(GameObject& obj) : CSVObjectManagerComponent(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("CSV_OBJECT_MANAGER"); // ソート番号を設定
}

void EnemyManagerComponent::Update()
{
}

void EnemyManagerComponent::CreateEnemies(std::vector<EnemyStatus> status)
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

		auto fighter = enemyObj->AddComponent<FighterComponent>();
		fighter->SetAtk(eS.atk);
		fighter->SetHp(eS.hp);

		auto collider = enemyObj->AddComponent<ColliderComponent>();
		collider->SetOffsetSizeAABB(Vector3(0.0f, 1.0f, 1.0f));
		SquareMesh squareMesh;
		auto render = enemyObj->AddComponent<Render2DComponent>();
		render->SetMesh(squareMesh);
		render->SetShader(eS.shaderVS, eS.shaderPS);
		render->SetTexture(eS.texture);
		render->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

		auto atk = enemyObj->AddComponent<AttackTimingComponent>();
		atk->SetCoolDownTime(1.0f);

		auto pd = enemyObj->AddComponent<PlayerDamageComponent>();

		auto renderColl = enemyObj->AddComponent<Render3DColliderAABBComponent>();
		renderColl->SetMesh(squareMesh);
		renderColl->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		renderColl->SetTexture("assets/texture/NoTexture.png");
		renderColl->SetColor(Vector4(1.0f, 0.0f, 0.0f, 0.5f));

		num++;
	}
}

void EnemyManagerComponent::CreateKind(const std::string& kind, GameObject& obj)
{
	if (kind == "E_Gunner") {
		auto eab = obj.AddComponent<EnemyActionBulletComponent>();
	}
	else if(kind == "E_Hopper") {
		auto eah = obj.AddComponent<EnemyActionHopperComponent>();
	}


}