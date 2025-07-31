#include "Transform.h"

TransformComponent::TransformComponent(GameObject& obj) :Component(obj) {
	m_sortNum = TRANSFORM; // ソート番号を設定
}

// 更新処理
void TransformComponent::Update() {

	// ワールド行列計算
	MakeWorldMatrix();

	// ローカル行列計算
	MakeLocalMatrix();
}

void TransformComponent::MakeWorldMatrix() {
	// 「 度 」から「 ラジアン 」へ変換
	float PitchRadians = DirectX::XMConvertToRadians(m_transform.m_Rotation.x); // X軸回転
	float YawRadians = DirectX::XMConvertToRadians(m_transform.m_Rotation.y);     // Y軸回転
	float RollRadians = DirectX::XMConvertToRadians(m_transform.m_Rotation.z);   // Z軸回転

	// SRT情報作成
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationRollPitchYaw(PitchRadians, YawRadians, RollRadians);
	DirectX::XMMATRIX t = DirectX::XMMatrixTranslation(m_transform.m_Position.x, m_transform.m_Position.y, m_transform.m_Position.z);
	DirectX::XMMATRIX s = DirectX::XMMatrixScaling(m_transform.m_Scale.x, m_transform.m_Scale.y, m_transform.m_Scale.z);

	// ワールド行列を作成し、保存
	m_transform.worldMatrix = s * r * t;
	m_transform.worldMatrix = DirectX::XMMatrixTranspose(m_transform.worldMatrix); // 行列を転置
}

void TransformComponent::MakeLocalMatrix() {
	float PitchRadians = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.x); // X軸回転
	float YawRadians = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.y);     // Y軸回転
	float RollRadians = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.z);   // Z軸回転

	// SRT情報作成
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationRollPitchYaw(PitchRadians, YawRadians, RollRadians);
	DirectX::XMMATRIX t = DirectX::XMMatrixTranslation(m_transform.m_LocalPosition.x, m_transform.m_LocalPosition.y, m_transform.m_LocalPosition.z);
	DirectX::XMMATRIX s = DirectX::XMMatrixScaling(m_transform.m_LocalScale.x, m_transform.m_LocalScale.y, m_transform.m_LocalScale.z);

	// ローカル行列を作成し、保存
	m_transform.localMatrix = s * r * t;
	m_transform.localMatrix = DirectX::XMMatrixTranspose(m_transform.localMatrix); // 行列を転置
}