#pragma once
#include "Render.h"
#include "TestBoneData.h"
#include <SimpleMath.h>
#include "HelpMath.h"

class RenderLineComponent : public RenderComponent
{
private:
	GameObject* m_StartObj = nullptr;
	GameObject* m_EndObj = nullptr;
	float m_Thickness = 1.0f;
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
	void SetStartAndEndDrawContainer(const DrawContainer& dcS, const DrawContainer& dcE);
	void SetStartActive(const ActiveState& as) { m_StartObj->SetActiveState(as); };
	void SetEndActive(const ActiveState& as) { m_EndObj->SetActiveState(as); };
	void SetThickness(const float thickness) { m_Thickness = thickness; };
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

	GameObject* GetStartPoint()const { return m_StartObj; };
	GameObject* GetEndPoint()const { return m_EndObj; };
};