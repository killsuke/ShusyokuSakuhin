#include "TestExtrusionComponent.h"
#include "Collider.h"
#include "GameObjectManager.h"
#include "RigidBodyComponent.h"
#include "JumpComponent.h"
#include "Input.h"

TestExtrusionComponent::TestExtrusionComponent(GameObject& obj) :Component(obj) {
	m_sortNum = TEST_EXTRUSION; // ソート番号を設定
}

// 更新処理
void TestExtrusionComponent::Update() {
	bool isPressed = false;
	bool isTrigger = false;

	if (Input::GetKeyTrigger(VK_I) == true || Input::GetButtonTrigger(XINPUT_A) == true) {
		isTrigger = true;
	}

	if (Input::GetKeyPress(VK_I) == true || Input::GetButtonPress(XINPUT_A) == true) {
		isPressed = true;
	}

	auto coll = p_object->GetComponent<ColliderComponent>();
	auto rigid = p_object->GetComponent<RigidBodyComponent>();
	auto jump = p_object->GetComponent<JumpComponent>();

	auto obj = GameObjectManager::GameObjectFindName("cube2");
	auto obj2 = GameObjectManager::GameObjectFindName("cube3");
	auto collObj = obj->GetComponent<ColliderComponent>();
	auto collObj2 = obj2->GetComponent<ColliderComponent>();

	if (coll == nullptr || rigid == nullptr || obj == nullptr || obj2 == nullptr || collObj == nullptr || collObj2 == nullptr || jump == nullptr) {
		return; // いずれかのコンポーネントが存在しない場合は処理を中断、このやり方はないかな
	}

	jump->JumpAction(isPressed, isGround, isTrigger);

	isGround = false;

	rigid->UseGravity(true, 120.0f, 12.0f);

	coll->Update();

	DirectX::XMFLOAT3 hitNormal = {};
	if (coll->CheckHit_CubeAndCube_NoTrigger2D_Normal(*collObj, *coll, hitNormal)) {
		if (hitNormal.y < -0.5f) {	// 天井
			rigid->UseGravity(false, 120.0f, 12.0f);
			isGround = true;
		}
		else if (hitNormal.y > 0.5f) {	// 地面
		}
		else if (abs(hitNormal.x) > 0.5f) { // 左右の壁
		}
	}

	if (coll->CheckHit_CubeAndCube_NoTrigger2D_Normal(*collObj2, *coll, hitNormal)) {
		if (hitNormal.y < -0.5f) {	// 天井
			rigid->UseGravity(false, 120.0f, 12.0f);
			isGround = true;
		}
		else if (hitNormal.y > 0.5f) {	// 地面
		}
		else if (abs(hitNormal.x) > 0.5f) { // 左右の壁
		}
	}
}