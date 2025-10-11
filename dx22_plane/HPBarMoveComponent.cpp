#include "HPBarMoveComponent.h"
#include "FighterComponent.h"
#include "DirectXRender.h"
#include <iostream>

HPBarMoveComponent::HPBarMoveComponent(GameObject& obj) : Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("HP_BAR_MOVE"); // ソート番号を設定
}

void HPBarMoveComponent::Update() {
	HPParam hpParam;

	auto hpObj = referenceHpObj->GetComponent<FighterComponent>();

	if(hpObj == nullptr) {
		return; // HPオブジェクトが設定されていない場合は何もしない
	}

	// HPの量を入れる
	hpParam.currentHP = float(hpObj->GetHp());

	// HPの値「１」辺りの縦のサイズを入れる
	hpParam.hpScale = hpScale;

	DirectXRender::GetDeviceContext()->UpdateSubresource(g_pHPBarConstantBuffer, 0, nullptr, &hpParam, 0, 0);
}