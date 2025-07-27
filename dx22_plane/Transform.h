#pragma once
#include "Component.h"
#include <SimpleMath.h>
#include <iostream>

struct Transform {
	// SRT情報（ワールド行列、姿勢情報）
	DirectX::SimpleMath::Vector3 m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	// SRT情報（ローカル行列、姿勢情報）
	DirectX::SimpleMath::Vector3 m_LocalPosition = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_LocalRotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 m_LocalScale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	// ワールド行列
	DirectX::XMMATRIX worldMatrix = {};

	// ローカル行列
	DirectX::XMMATRIX localMatrix = {};
};

class TransformComponent : public Component
{
private:
	Transform m_transform;	// トランスフォーム情報
public:
	TransformComponent() = default;
	TransformComponent(GameObject& obj);

	~TransformComponent() = default;

	// 更新処理
	void Update() override;

	inline void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.m_Position = position; };
	inline void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { m_transform.m_Rotation = rotation; };;
	inline void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_transform.m_Scale = scale; };

	inline void SetLocalPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.m_LocalPosition = position; };
	inline void SetLocalRotation(const DirectX::SimpleMath::Vector3& rotation) { m_transform.m_LocalRotation = rotation; };
	inline void SetLocalScale(const DirectX::SimpleMath::Vector3& scale) { m_transform.m_LocalScale = scale; };

	inline DirectX::SimpleMath::Vector3 GetPosition() const { return m_transform.m_Position; };
	inline DirectX::SimpleMath::Vector3 GetRotation() const { return m_transform.m_Rotation; };
	inline DirectX::SimpleMath::Vector3 GetScale() const { return m_transform.m_Scale; };

	inline DirectX::SimpleMath::Vector3 GetLocalPosition() const { return m_transform.m_LocalPosition; };
	inline DirectX::SimpleMath::Vector3 GetLocalRotation() const { return m_transform.m_LocalRotation; };
	inline DirectX::SimpleMath::Vector3 GetLocalScale() const { return m_transform.m_LocalScale; };

	inline DirectX::XMMATRIX GetWorldMatrix() const { return m_transform.worldMatrix; };
	inline DirectX::XMMATRIX GetLocalMatrix() const { return m_transform.localMatrix; };

	inline void AddPosition(const DirectX::SimpleMath::Vector3& position) { m_transform.m_Position += position; };
	inline void AddRotation(const DirectX::SimpleMath::Vector3& rotation) { m_transform.m_Rotation += rotation; };

	void MakeWorldMatrix();
	void MakeLocalMatrix();
};