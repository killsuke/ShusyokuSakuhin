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

	// クォータニオン作成
	DirectX::SimpleMath::Quaternion q = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);

	// SRT情報作成
	// 各行列を生成
	DirectX::SimpleMath::Matrix r = DirectX::SimpleMath::Matrix::CreateFromQuaternion(q);
	DirectX::SimpleMath::Matrix s = DirectX::SimpleMath::Matrix::CreateScale(m_transform.m_Scale);
	DirectX::SimpleMath::Matrix t = DirectX::SimpleMath::Matrix::CreateTranslation(m_transform.m_Position);


	// ワールド行列を作成し、保存
	m_transform.worldMatrix = s * r * t;
	m_transform.worldMatrix = m_transform.worldMatrix.Transpose(); // 行列を転置
}

void TransformComponent::MakeLocalMatrix() {
	float PitchRadians = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.x); // X軸回転
	float YawRadians = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.y);     // Y軸回転
	float RollRadians = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.z);   // Z軸回転

	// クォータニオン作成
	DirectX::SimpleMath::Quaternion q = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);

	// SRT情報作成
	DirectX::SimpleMath::Matrix r = DirectX::SimpleMath::Matrix::CreateFromQuaternion(q);
	DirectX::SimpleMath::Matrix s = DirectX::SimpleMath::Matrix::CreateScale(m_transform.m_LocalScale);
	DirectX::SimpleMath::Matrix t = DirectX::SimpleMath::Matrix::CreateTranslation(m_transform.m_LocalPosition);

	// ローカル行列を作成し、保存
	m_transform.localMatrix = s * r * t;
	m_transform.localMatrix = m_transform.localMatrix.Transpose(); // 行列を転置
}