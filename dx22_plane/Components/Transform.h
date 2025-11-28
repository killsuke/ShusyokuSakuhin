#pragma once
#include "Component.h"
#include <SimpleMath.h>
#include <iostream>
#include "HelpMath.h"
#include "XMFLOAT_Helper.h"

struct Transform {
	// SRT情報（ワールド行列、姿勢情報）
	DirectX::SimpleMath::Vector3 m_Scale = DirectX::SimpleMath::Vector3::One;
	DirectX::XMFLOAT3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Vector3 m_Position = DirectX::SimpleMath::Vector3::Zero;

	// SRT情報（ローカル行列、姿勢情報）
	DirectX::SimpleMath::Vector3 m_LocalScale = DirectX::SimpleMath::Vector3::One;
	DirectX::XMFLOAT3 m_LocalRotation = { 0.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Vector3 m_LocalPosition = DirectX::SimpleMath::Vector3::Zero;

	DirectX::XMVECTOR m_Quaternion = DirectX::XMQuaternionIdentity();

	DirectX::XMVECTOR m_LocalQuaternion = DirectX::SimpleMath::Quaternion::Identity; // ローカルクォータニオン（左手系）

	// ワールド行列
	DirectX::XMMATRIX worldMatrix = DirectX::SimpleMath::Matrix::Identity;

	// ローカル行列
	DirectX::XMMATRIX localMatrix = DirectX::SimpleMath::Matrix::Identity;
};

class TransformComponent final : public Component
{
private:
	Transform m_transform;	// トランスフォーム情報
public:
	TransformComponent(GameObject& obj);

	~TransformComponent() = default;

	// 更新処理
	void Update() override;

	inline void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.m_Position = position; };
	void SetRotation(const DirectX::XMFLOAT3& rotation) { 
		m_transform.m_Rotation = rotation;
	
		const float pitch = DirectX::XMConvertToRadians(rotation.x);
		const float yaw = DirectX::XMConvertToRadians(rotation.y);
		const float roll = DirectX::XMConvertToRadians(rotation.z);

		m_transform.m_Quaternion = DirectX::XMQuaternionRotationRollPitchYaw(
			pitch,
			yaw,
			roll
		);
	};
	inline void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_transform.m_Scale = scale; };
	inline void SetQuaternion(const DirectX::XMVECTOR& quaternion) { 
		m_transform.m_Quaternion = quaternion;

		// 左手系に変換
		m_transform.m_Rotation = QuaternionToEulerDeg(m_transform.m_Quaternion);
	};

	inline void SetLocalPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.m_LocalPosition = position; };
	void SetLocalRotation(const DirectX::XMFLOAT3& rotation) {
		m_transform.m_LocalRotation = rotation; 

		const float pitch = DirectX::XMConvertToRadians(rotation.x);
		const float yaw = DirectX::XMConvertToRadians(rotation.y);
		const float roll = DirectX::XMConvertToRadians(rotation.z);

		m_transform.m_LocalQuaternion = DirectX::XMQuaternionRotationRollPitchYaw(
			pitch,
			yaw,
			roll
		);
	};
	inline void SetLocalScale(const DirectX::SimpleMath::Vector3& scale) { m_transform.m_LocalScale = scale; };
	inline void SetLocalQuaternion(const DirectX::XMVECTOR& quaternion) {
		m_transform.m_LocalQuaternion = quaternion;

		// 左手系に変換
		m_transform.m_LocalRotation = QuaternionToEulerRad(m_transform.m_LocalQuaternion) * (180.0f / DirectX::XM_PI);
	};

	inline DirectX::SimpleMath::Vector3 GetPosition() const { return m_transform.m_Position; };
	inline DirectX::XMFLOAT3 GetRotation() const { return m_transform.m_Rotation; };
	inline DirectX::SimpleMath::Vector3 GetScale() const { return m_transform.m_Scale; };
	inline DirectX::XMVECTOR GetQuaternion() const { return m_transform.m_Quaternion; };

	inline DirectX::SimpleMath::Vector3 GetLocalPosition() const { return m_transform.m_LocalPosition; };
	inline DirectX::XMFLOAT3 GetLocalRotation() const { return m_transform.m_LocalRotation; };
	inline DirectX::SimpleMath::Vector3 GetLocalScale() const { return m_transform.m_LocalScale; };
	inline DirectX::XMVECTOR GetLocalQuaternion() const { return m_transform.m_LocalQuaternion; };

	inline DirectX::XMMATRIX GetWorldMatrix() const { return m_transform.worldMatrix; };
	inline DirectX::XMMATRIX GetLocalMatrix() const { return m_transform.localMatrix; };

	inline void AddPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.m_Position += position; };
	inline void AddRotation(const DirectX::XMFLOAT3& rotation) {
		m_transform.m_Rotation += rotation;
		
		const float pitch = DirectX::XMConvertToRadians(m_transform.m_Rotation.x);
		const float yaw = DirectX::XMConvertToRadians(m_transform.m_Rotation.y);
		const float roll = DirectX::XMConvertToRadians(m_transform.m_Rotation.z);

		m_transform.m_Quaternion = DirectX::XMQuaternionRotationRollPitchYaw(
			pitch,
			yaw,
			roll
		);
	};
	inline void AddScale(const DirectX::SimpleMath::Vector3& scale) { m_transform.m_Scale += scale; };

	inline void AddLocalPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.m_LocalPosition += position; };
	inline void AddLocalRotation(const DirectX::XMFLOAT3& rotation) {
		m_transform.m_LocalRotation += rotation; 

		const float pitch = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.x);
		const float yaw = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.y);
		const float roll = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.z);

		m_transform.m_LocalQuaternion = DirectX::XMQuaternionRotationRollPitchYaw(
			pitch,
			yaw,
			roll
		);
	};
	inline void AddLocalScale(const DirectX::SimpleMath::Vector3& scale) { m_transform.m_LocalScale += scale; };

	DirectX::XMMATRIX MakeWorldMatrix();
	DirectX::XMMATRIX MakeLocalMatrix();

	void MakeChildWorld();

};