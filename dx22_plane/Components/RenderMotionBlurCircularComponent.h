#pragma once
#include "Render.h"
#include "TransformComponent.h"

class RenderMotionBlurCircularComponent :  public RenderComponent
{
private:
	GameObject* m_ChildTip = nullptr;
	GameObject* m_ChildRoot = nullptr;
	DirectX::XMFLOAT3 prevTipPos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 prevRootPos = { 0.0f,0.0f,0.0f };	
	DirectX::XMVECTOR prevQuat = DirectX::XMQuaternionIdentity();

public:
	RenderMotionBlurCircularComponent(GameObject& obj);
	~RenderMotionBlurCircularComponent() = default;
	void Update()override;

	void SetTipAndRootPos(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2) {

		TransformComponent* tipTransform = m_ChildTip->GetComponent<TransformComponent>();
		TransformComponent* rootTransform = m_ChildRoot->GetComponent<TransformComponent>();

		tipTransform->SetLocalPosition(pos1);
		rootTransform->SetLocalPosition(pos2);
	}
};