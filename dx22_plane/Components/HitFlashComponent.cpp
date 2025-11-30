#include "HitFlashComponent.h"
#include "DirectXRender.h"
#include "FighterComponent.h"

HitFlashComponent::HitFlashComponent(GameObject& obj) : RenderComponent(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("HIT_FLASH"); // ソート番号を設定
}

void HitFlashComponent::Update() {

	const FighterComponent* fighter = m_Object->GetComponent<FighterComponent>();

	if(fighter != nullptr) {

		HitFlashBuffer cb = {};

		cb.hitFlashColor = m_HitFlashColor;
		cb.hitFlashPower = m_HitFlashPower;

		// 無敵時間を使用するかどうかでフラグを変える
		const bool useInvincible = fighter->GetUseInvincible();

		if (useInvincible == true) {
			cb.isHit = fighter->GetInvincibleFlag();
		}
		else {
			// もっとダメージフラッシュの時間を延ばしてもいいかも
			cb.isHit = fighter->GetIsAttacked();
		}

		// コンスタントバッファ更新
		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();
	
		ID3D11Buffer* buffer = DirectXRender::GetHitFlashBuffer();

		deviceContext->UpdateSubresource(buffer, 0, NULL, &cb, 0, 0);
	}
}