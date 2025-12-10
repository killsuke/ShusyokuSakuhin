#pragma once
#include "Render.h"
#include "TestBoneData.h"
#include <SimpleMath.h>
#include "HelpMath.h"

class RenderLineComponent : public RenderComponent
{
private:
	GameObject* p_startObj = nullptr;
	GameObject* p_endObj = nullptr;
	float m_thickness = 1.0f;
	//DirectX::SimpleMath::Vector3 m_Rotation = DirectX::SimpleMath::Vector3::Zero;
	//DirectX::SimpleMath::Quaternion m_Quaternion = DirectX::SimpleMath::Quaternion::Identity;

public:
	RenderLineComponent(GameObject& obj);
	~RenderLineComponent() = default;

	void Update() override;

	void SetStartPosition(const DirectX::SimpleMath::Vector3& startPos);
	void SetEndPosition(const DirectX::SimpleMath::Vector3& endPos);
	void SetStartAndEndPosition(const DirectX::SimpleMath::Vector3& startPos, const DirectX::SimpleMath::Vector3& endPos);
	void SetStartAndEndFollowObject(GameObject* objS, GameObject* objE);
	void SetStartActive(const ActiveState as) { p_startObj->SetActiveState(as); };
	void SetEndActive(const ActiveState as) { p_endObj->SetActiveState(as); };
	void SetThickness(const float thickness) { m_thickness = thickness; };
	//void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { 
	//	m_Rotation = rotation; 
	//	float pitch = DirectX::XMConvertToRadians(rotation.x);
	//	float yaw = DirectX::XMConvertToRadians(rotation.y);
	//	float roll = DirectX::XMConvertToRadians(rotation.z);
	//	m_Quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
	//};
	//void SetQuaternion(const DirectX::SimpleMath::Quaternion& quaternion) { 
	//	m_Quaternion = quaternion; 
	//	m_Rotation = QuaternionToEulerDeg(quaternion);
	//};

	GameObject* GetStartPoint()const { return p_startObj; };
	GameObject* GetEndPoint()const { return p_endObj; };
};