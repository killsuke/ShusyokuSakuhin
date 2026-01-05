#pragma once
#include "Transform.h"

class FollowPositionComponent : public Component
{
private:
	GameObject* p_followObj = nullptr; // 追従するオブジェクト
	DirectX::XMFLOAT3 m_offset = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // 追従する際のオフセット

public:
	FollowPositionComponent(GameObject& obj);
	~FollowPositionComponent() = default;

	void Update() override;

	void SetFollowObject(GameObject* obj) { p_followObj = obj; };
	void SetOffset(const DirectX::XMFLOAT3& offset) { m_offset = offset; };
};