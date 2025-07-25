#include "TestMoveComponent.h"
#include "RigidBodyComponent.h"
#include "input.h"

TestMoveComponent::TestMoveComponent(GameObject& obj) :Component(obj) {
	m_sortNum = TEST_MOVE; // ソート番号を設定
}

// 更新処理
void TestMoveComponent::Update() {
	auto keyJ = Input::GetKeyPress(VK_J);
	auto keyL = Input::GetKeyPress(VK_L);

	auto rigid = p_object->GetComponent<RigidBodyComponent>();

	if (rigid != nullptr) {
		if (keyJ == true && keyL == false) {
			rigid->ConstantVelocity_X(-150.0f);
		}
		if (keyL == true && keyJ == false) {
			rigid->ConstantVelocity_X(150.0f);
		}

		rigid->ReduceVelocity_X();
	}

}