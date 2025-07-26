#include "TestMoveComponent.h"
#include "RigidBodyComponent.h"
#include "input.h"

TestMoveComponent::TestMoveComponent(GameObject& obj) :Component(obj) {
	m_sortNum = TEST_MOVE; // ソート番号を設定
}

// 更新処理
void TestMoveComponent::Update() {
	bool keyJ = false;
	bool keyL = false;

	if (Input::GetKeyPress(VK_J) == true || Input::GetButtonPress(XINPUT_LEFT))
	{
		keyJ = true;
	}
	if (Input::GetKeyPress(VK_L) == true || Input::GetButtonPress(XINPUT_RIGHT)) {
		keyL = true;
	}

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