#include "HPBarMoveComponent.h"
#include "FighterComponent.h"
#include "System/DirectXRender.h"
#include <iostream>

HPBarMoveComponent::HPBarMoveComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER_CONTOROL"); // ソート番号を設定
}

void HPBarMoveComponent::Update() {
	OverVertexParam hpParam;

	FighterComponent* hpObj = referenceHpObj->GetComponent<FighterComponent>();

	if(hpObj == nullptr) {
		return; // HPオブジェクトが設定されていない場合は何もしない
	}

	// HPの量を入れる
	hpParam.currentScale = float(hpObj->GetHp());

	// HPの値「１」辺りの縦のサイズを入れる
	hpParam.baseScale = hpScale;

	ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

	ID3D11Buffer* buffer = DirectXRender::GetOverVertexMoveBuffer();
	deviceContext->VSSetConstantBuffers(UINT(EBufferTypes::OVER_VERTEX), 1, &buffer);

	DirectXRender::GetDeviceContext()->UpdateSubresource(buffer, 0, nullptr, &hpParam, 0, 0);
}