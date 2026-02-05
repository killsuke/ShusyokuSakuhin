#include "PlayerDamageComponent.h"
#include "PlayerOperationComponent.h"
#include "Transform.h"
#include "RigidBodyComponent.h"
#include "AttackTimingComponent.h"
#include "AttackOneTimeComponent.h"
#include "FighterComponent.h"
#include "ColliderAttackComponent.h"
#include "ColliderDamageComponent.h"
#include "Manager/GameObjectManager.h"
#include "Manager/EventBusManager.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

PlayerDamageComponent::PlayerDamageComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("DAMAGE"); // ソート番号を設定
}

void PlayerDamageComponent::Update()
{
	ColliderAttackComponent* collObjMe = m_Object->GetComponent<ColliderAttackComponent>();
	std::vector<GameObject*> objOthers = GameObjectManager::GameObjectFindTag("Player");

	AttackTimingComponent* attack_T = m_Object->GetComponent<AttackTimingComponent>();
	AttackOneTimeComponent* attack_O = m_Object->GetComponent<AttackOneTimeComponent>();

	if (attack_O != nullptr) {
		attack_O->ReSetAttackHitFlag();
	}
	if (attack_T != nullptr) {
		attack_T->ReSetAttackHitFlag();
	}

	// ここでプレイヤーをちょっと吹っ飛ばす処理も追加する予定
	if (collObjMe != nullptr) {

		for (auto& objOther : objOthers) {
			ColliderDamageComponent* collObjOther = objOther->GetComponent<ColliderDamageComponent>();
			if (collObjMe->CheckHit_AABBAndOBB_IsTrigger3D(
				*collObjOther, *collObjMe)) {

				if (m_EnemyActionComp != nullptr) {

					PlayerOperationComponent* playerOp = objOther->GetComponent<PlayerOperationComponent>();
					if (playerOp != nullptr) {
						// チャージスラッシュ中はダメージを与えない
						if (playerOp->GetPlayerState() == PlayerState::CHARGE_SLASH) {
							return;
						}
					}
				}

				const uint32_t myID = m_Object->GetInstanceID();
				const uint32_t otherID = objOther->GetInstanceID();

				const HitEvent he = { myID,otherID };

				// ヒット時の通知テスト
				EventBusManager::Push(he);

				if (attack_O != nullptr) {
					attack_O->AttackAction(*objOther);
				}
				if (attack_T != nullptr) {
					attack_T->AttackAction(*objOther);
				}
			}
		}
	}
}