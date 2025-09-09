#include "TestMoveComponent.h"
#include "RigidBodyComponent.h"
#include "FighterComponent.h"
#include "Render3D.h"
#include "HPBarMoveComponent.h"
#include "GameObjectManager.h"
#include "input.h"

TestMoveComponent::TestMoveComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を設定
}

// 更新処理
void TestMoveComponent::Update() {
	bool keyJ = false;
	bool keyL = false;

	auto fighter = p_object->GetComponent<FighterComponent>();
	auto rigid = p_object->GetComponent<RigidBodyComponent>();
	
	if(/*fighter == nullptr || */rigid == nullptr)
	{
		return;
	}

	if (Input::GetKeyPress(VK_A) == true || Input::GetButtonPress(XINPUT_LEFT))
	{
		m_rightLeft = false; // 左向き
		keyJ = true;
	}
	if (Input::GetKeyPress(VK_D) == true || Input::GetButtonPress(XINPUT_RIGHT)) {
		m_rightLeft = true; // 右向き
		keyL = true;
	}

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