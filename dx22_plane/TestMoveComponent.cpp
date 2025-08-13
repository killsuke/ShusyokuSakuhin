#include "TestMoveComponent.h"
#include "RigidBodyComponent.h"
#include "FighterComponent.h"
#include "Render3D.h"
#include "HPBarMoveComponent.h"
#include "GameObjectManager.h"
#include "input.h"

TestMoveComponent::TestMoveComponent(GameObject& obj) :Component(obj) {
	m_sortNum = TEST_MOVE; // ソート番号を設定
}

// 更新処理
void TestMoveComponent::Update() {
	bool keyJ = false;
	bool keyL = false;

	auto fighter = p_object->GetComponent<FighterComponent>();

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

	/*if (Input::GetKeyTrigger(VK_RETURN)) {
		fighter->AddDamage(1);
	}*/

	//auto hpUI = GameObjectManager::GameObjectFindNameUI("hpUI");
	//auto hpRend = hpUI->GetComponent<Render3DComponent>();

	//auto maxHp = fighter->GetMaxHp();
	//auto hp = fighter->GetHp();

	//// HPのカラー変更
	//if(hp == 0) {
	//	hpRend->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)); // 透明
	//}
	//else if (hp < (maxHp * 0.3f)) {
	//	hpRend->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)); // 赤色
	//}
	//else if (hp < (maxHp * 0.5f)) {
	//	hpRend->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)); // 黄色
	//}
	//else {
	//	hpRend->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)); // 緑色
	//}

}