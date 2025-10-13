#pragma once
#include "Component.h"
#include <SimpleMath.h>
#include <iostream>
#include "QuaternionToEuler.h"

struct Transform {
	// SRT情報（ワールド行列、姿勢情報）
	DirectX::SimpleMath::Vector3 m_Scale = DirectX::SimpleMath::Vector3::One;
	DirectX::SimpleMath::Vector3 m_Rotation = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 m_Position = DirectX::SimpleMath::Vector3::Zero;

	// SRT情報（ローカル行列、姿勢情報）
	DirectX::SimpleMath::Vector3 m_LocalScale = DirectX::SimpleMath::Vector3::One;
	DirectX::SimpleMath::Vector3 m_LocalRotation = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 m_LocalPosition = DirectX::SimpleMath::Vector3::Zero;

	DirectX::SimpleMath::Quaternion m_Quaternion = DirectX::SimpleMath::Quaternion::Identity; // クォータニオン

	DirectX::SimpleMath::Quaternion m_LocalQuaternion = DirectX::SimpleMath::Quaternion::Identity; // ローカルクォータニオン

	// ワールド行列
	DirectX::SimpleMath::Matrix worldMatrix = DirectX::SimpleMath::Matrix::Identity;

	// ローカル行列
	DirectX::SimpleMath::Matrix localMatrix = DirectX::SimpleMath::Matrix::Identity;
};

class TransformComponent : public Component
{
private:
	Transform m_transform;	// トランスフォーム情報
public:
	TransformComponent(GameObject& obj);

	~TransformComponent() = default;

	// 更新処理
	void Update() override;

	inline void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.m_Position = position; };
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { 
		m_transform.m_Rotation = rotation;
	
		float pitch = DirectX::XMConvertToRadians(rotation.x);
		float yaw = DirectX::XMConvertToRadians(rotation.y);
		float roll = DirectX::XMConvertToRadians(rotation.z);

		m_transform.m_Quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
	};
	inline void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_transform.m_Scale = scale; };
	inline void SetQuaternion(const DirectX::SimpleMath::Quaternion& quaternion) { 
		m_transform.m_Quaternion = quaternion; 
		m_transform.m_Rotation = QuaternionToEulerRad(quaternion) * (180.0f / DirectX::XM_PI);
	};

	inline void SetLocalPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.m_LocalPosition = position; };
	void SetLocalRotation(const DirectX::SimpleMath::Vector3& rotation) { 
		m_transform.m_LocalRotation = rotation; 

		float pitch = DirectX::XMConvertToRadians(rotation.x);
		float yaw = DirectX::XMConvertToRadians(rotation.y);
		float roll = DirectX::XMConvertToRadians(rotation.z);

		m_transform.m_LocalQuaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
	};
	inline void SetLocalScale(const DirectX::SimpleMath::Vector3& scale) { m_transform.m_LocalScale = scale; };
	inline void SetLocalQuaternion(const DirectX::SimpleMath::Quaternion& quaternion) { 
		m_transform.m_LocalQuaternion = quaternion; 
		m_transform.m_LocalRotation = QuaternionToEulerRad(quaternion) * (180.0f / DirectX::XM_PI);
	};

	inline DirectX::SimpleMath::Vector3 GetPosition() const { return m_transform.m_Position; };
	inline DirectX::SimpleMath::Vector3 GetRotation() const { return m_transform.m_Rotation; };
	inline DirectX::SimpleMath::Vector3 GetScale() const { return m_transform.m_Scale; };
	inline DirectX::SimpleMath::Quaternion GetQuaternion() const { return m_transform.m_Quaternion; };

	inline DirectX::SimpleMath::Vector3 GetLocalPosition() const { return m_transform.m_LocalPosition; };
	inline DirectX::SimpleMath::Vector3 GetLocalRotation() const { return m_transform.m_LocalRotation; };
	inline DirectX::SimpleMath::Vector3 GetLocalScale() const { return m_transform.m_LocalScale; };
	inline DirectX::SimpleMath::Quaternion GetLocalQuaternion() const { return m_transform.m_LocalQuaternion; };

	inline DirectX::SimpleMath::Matrix GetWorldMatrix() const { return m_transform.worldMatrix; };
	inline DirectX::SimpleMath::Matrix GetLocalMatrix() const { return m_transform.localMatrix; };

	inline void AddPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.m_Position += position; };
	inline void AddRotation(const DirectX::SimpleMath::Vector3& rotation) { 
		m_transform.m_Rotation += rotation;
		
		float pitch = DirectX::XMConvertToRadians(m_transform.m_Rotation.x);
		float yaw = DirectX::XMConvertToRadians(m_transform.m_Rotation.y);
		float roll = DirectX::XMConvertToRadians(m_transform.m_Rotation.z);

		m_transform.m_Quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
	};
	inline void AddScale(const DirectX::SimpleMath::Vector3& scale) { m_transform.m_Scale += scale; };

	inline void AddLocalPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.m_LocalPosition += position; };
	inline void AddLocalRotation(const DirectX::SimpleMath::Vector3& rotation) { 
		m_transform.m_LocalRotation += rotation; 

		float pitch = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.x);
		float yaw = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.y);
		float roll = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.z);

		m_transform.m_LocalQuaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
	};
	inline void AddLocalScale(const DirectX::SimpleMath::Vector3& scale) { m_transform.m_LocalScale += scale; };

	DirectX::SimpleMath::Matrix MakeWorldMatrix();
	DirectX::SimpleMath::Matrix MakeLocalMatrix();

	void MakeChildWorld();

};