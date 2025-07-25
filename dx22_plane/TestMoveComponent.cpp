#include "TestMoveComponent.h"
#include "Transform.h"
#include "input.h"

TestMoveComponent::TestMoveComponent(GameObject& obj) :Component(obj) {
	m_sortNum = TEST_MOVE; // ソート番号を設定
}

// 更新処理
void TestMoveComponent::Update() {
	auto keyJ = Input::GetKeyPress(VK_J);
	auto keyL = Input::GetKeyPress(VK_L);
	auto keyI = Input::GetKeyPress(VK_I);
	auto keyK = Input::GetKeyPress(VK_K);

	auto transform = p_object->GetComponent<TransformComponent>();

	if (transform != nullptr) {
		if (keyJ == true) {
			transform->AddPosition(DirectX::XMFLOAT3(-3.0f,0.0f,0.0f));
		}
		if (keyL == true) {
			transform->AddPosition(DirectX::XMFLOAT3(3.0f, 0.0f, 0.0f));
		}
		if (keyI == true) {
			transform->AddPosition(DirectX::XMFLOAT3(0.0f, 3.0f, 0.0f));
		}
		if (keyK == true) {
			transform->AddPosition(DirectX::XMFLOAT3(0.0f, -3.0f, 0.0f));
		}
	}

}