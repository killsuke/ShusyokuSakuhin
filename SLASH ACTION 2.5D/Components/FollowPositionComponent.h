#pragma once
#include "TransformComponent.h"

class FollowPositionComponent : public Component
{
private:
	GameObject* m_FollowObj = nullptr; // 追従するオブジェクト
	DirectX::XMFLOAT3 m_Offset = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // 追従する際のオフセット

public:
	FollowPositionComponent(GameObject& obj);
	~FollowPositionComponent() = default;

	void Update() override;

	void SetFollowObject(GameObject* obj) { m_FollowObj = obj; };
	void SetOffset(const DirectX::XMFLOAT3& offset) { m_Offset = offset; };
};