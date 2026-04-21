#include "EnemyDeathEventComponent.h"
#include "CameraComponent.h"
#include "CutObjectActionComponent.h"
#include "TimeLineComponent.h"
#include "Mesh/TriangularPrismMesh.h"
#include "Manager/EventBusManager.h"
#include "Manager/GameObjectManager.h"
#include "RenderBillboardComponent.h"
#include "Mesh/SquareMesh.h"
#include <DirectXMath.h>
#include <random>

using namespace DirectX;

EnemyDeathEventComponent::EnemyDeathEventComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("DOOR_FADE"); // ソート番号を設定（あとで変えよ）

	m_listenerID_DeathEvent = EventBusManager::Subscribe<DeathEvent>([&](const DeathEvent& e) {
		DeathEventAction(e);
		});

	// 最初は生きているのでフラグをOFFに
	m_IsActiveFlag = false;
}

EnemyDeathEventComponent::~EnemyDeathEventComponent() {
	EventBusManager::Unsubscribe(m_listenerID_DeathEvent);
}

void EnemyDeathEventComponent::Update() {

	
}

void EnemyDeathEventComponent::DeathEventAction(const DeathEvent& event) {
	const uint32_t deadID = m_Object->GetInstanceID();

	if (event.deadID != deadID) {
		return; // 自分宛じゃないなら無視
	}

	CutEvent ce;

	CutDirection dir = CutDirection::VERTICAL;
	float ratio1 = 0.5f;
	float ratio2 = 0.5f;

	// 切断方向と割合を決定
	// 斜め横切り
	if (m_SwordActionState == ESwordActionState::SLASH_1ST) {
		dir = CutDirection::HORIZONTAL;
		if (m_RightLeft == RightLeft::RIGHT) {
			ratio1 = 0.8f;
			ratio2 = 0.2f;
		}
		else if (m_RightLeft == RightLeft::LEFT) {
			ratio1 = 0.2f;
			ratio2 = 0.8f;
		}
	}
	// 斜め横切り
	else if (m_SwordActionState == ESwordActionState::SLASH_2ND) {
		dir = CutDirection::HORIZONTAL;
		if (m_RightLeft == RightLeft::RIGHT) {
			ratio1 = 0.2f;
			ratio2 = 0.8f;
		}
		else if (m_RightLeft == RightLeft::LEFT) {
			ratio1 = 0.8f;
			ratio2 = 0.2f;
		}

	}
	// 縦切り
	else if (m_SwordActionState == ESwordActionState::SLASH_3RD || m_SwordActionState == ESwordActionState::NONE) {
		dir = CutDirection::VERTICAL;
		ratio1 = 0.5f;
		ratio2 = 0.5f;
	}

	ce = CutEvent{
		deadID,
		dir,
		ratio1,ratio2
	};

	MeshCut2DComponent* meshCutComp = m_Object->GetComponent<MeshCut2DComponent>();
	std::array<uint32_t,2> cutObjIDs;
	if (meshCutComp != nullptr) {
		cutObjIDs = meshCutComp->MakeMeshCutAction(dir,ratio1,ratio2);
	}

	m_Object->SetActiveState(ActiveState::ALL_STOP);

	// ここはのちに選択式に変更
	/*switch (m_DeathType)
	{
	case RAMDOM:

		break;
	case ABS_STICKY:
		m_DeathState = EnemyDeathEventState::STICKY;
		break;
	case ABS_IMMEDIATE:
		m_DeathState = EnemyDeathEventState::IMMEDIATE;
		break;
	case MAX:
		break;
	default:
		break;
	}*/

	const std::string name = m_Object->GetName();

	if (name == "Boss") {

		m_DeathState = EnemyDeathEventState::STICKY;
	}
	else {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(0, 2);

		int value = dist(gen);

		if (value < 1) {

			m_DeathState = EnemyDeathEventState::STICKY;
		}
		else {
			m_DeathState = EnemyDeathEventState::IMMEDIATE;
		}
	}

	GameObject* cutCompObj = GameObjectManager::AddObject("cutCompObj","CutCompObj");
	TransformComponent* cutCompTrans = cutCompObj->AddComponent<TransformComponent>();
	TimeLineComponent* timeLine = cutCompObj->AddComponent<TimeLineComponent>();
	CutObjectActionComponent* cutCompAction = cutCompObj->AddComponent<CutObjectActionComponent>();
	cutCompAction->SetCutObjs(cutObjIDs[0], cutObjIDs[1]);
	cutCompAction->SetDeathState(m_DeathState);

	m_IsActiveFlag = true;

	m_Object->Destroy(); // オブジェクトを削除フラグを立てる
}