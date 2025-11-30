#include "HitFlashComponent.h"
#include "DirectXRender.h"
#include "Manager/EventBusManager.h"

HitFlashComponent::HitFlashComponent(GameObject& obj) : RenderComponent(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("HIT_FLASH"); // ソート番号を設定
	EventBusManager::Subscribe<HitEvent>([&](const HitEvent& e) {
		OnHitFlash(e);
		});
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

		// ヒットフラッシュが発生したらフラグを立てる
		// 基本エネミー用
		if (m_IsFlash == true) {
			cb.isHit = m_IsFlash;
			m_IsFlash = false;
		}

		// コンスタントバッファ更新
		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();
	
		ID3D11Buffer* buffer = DirectXRender::GetHitFlashBuffer();

		deviceContext->UpdateSubresource(buffer, 0, NULL, &cb, 0, 0);

		//m_IsFlash = false;
	}
}

void HitFlashComponent::OnHitFlash(const HitEvent& event) {

	if(event.target != m_Object) {
		return;
	}

	m_IsFlash = true;
}
