#pragma once
#include "Component.h"
#include <SimpleMath.h>

class MeshCut2DComponent : public Component
{
private:
	DirectX::SimpleMath::Vector3 m_StartCutPoint = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 m_GoalCutPoint = DirectX::SimpleMath::Vector3::Zero;

public:
	
	MeshCut2DComponent(GameObject& obj);
	~MeshCut2DComponent() = default;
	void Update() override;


};