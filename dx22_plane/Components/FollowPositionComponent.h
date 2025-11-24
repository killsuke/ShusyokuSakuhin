#pragma once
#include "Component.h"
#include <SimpleMath.h>

class FollowPositionComponent : public Component
{
private:
	GameObject* p_followObj = nullptr; // 追従するオブジェクト
	DirectX::SimpleMath::Vector3 m_offset = DirectX::SimpleMath::Vector3::Zero; // 追従する際のオフセット
	
public:
	FollowPositionComponent(GameObject& obj);
	~FollowPositionComponent() = default;

	void Update() override;

	void SetFollowObject(GameObject* obj) { p_followObj = obj; };
	void SetOffset(const DirectX::SimpleMath::Vector3& offset) { m_offset = offset; };
};