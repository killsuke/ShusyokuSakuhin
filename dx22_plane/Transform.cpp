#include "Transform.h"

// 更新処理
void TransformComponent::Update() {

	// ワールド行列計算
	{
		// 「 度 」から「 ラジアン 」へ変換
		float PitchRadians = DirectX::XMConvertToRadians(m_transform.m_Rotation.x); // X軸回転
		float YawRadians = DirectX::XMConvertToRadians(m_transform.m_Rotation.y);     // Y軸回転
		float RollRadians = DirectX::XMConvertToRadians(m_transform.m_Rotation.z);   // Z軸回転

		// SRT情報作成
		DirectX::SimpleMath::Matrix r = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);
		DirectX::SimpleMath::Matrix t = DirectX::SimpleMath::Matrix::CreateTranslation(m_transform.m_Position.x, m_transform.m_Position.y, m_transform.m_Position.z);
		DirectX::SimpleMath::Matrix s = DirectX::SimpleMath::Matrix::CreateScale(m_transform.m_Scale.x, m_transform.m_Scale.y, m_transform.m_Scale.z);

		// ワールド行列を作成し、保存
		m_transform.worldMatrix = s * r * t;
	}

	// ローカル行列計算
	{
		float PitchRadians = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.x); // X軸回転
		float YawRadians = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.y);     // Y軸回転
		float RollRadians = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.z);   // Z軸回転

		// SRT情報作成
		DirectX::SimpleMath::Matrix r = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);
		DirectX::SimpleMath::Matrix t = DirectX::SimpleMath::Matrix::CreateTranslation(m_transform.m_LocalPosition.x, m_transform.m_LocalPosition.y, m_transform.m_LocalPosition.z);
		DirectX::SimpleMath::Matrix s = DirectX::SimpleMath::Matrix::CreateScale(m_transform.m_LocalScale.x, m_transform.m_LocalScale.y, m_transform.m_LocalScale.z);

		// ローカル行列を作成し、保存
		m_transform.localMatrix = s * r * t;
	}
}