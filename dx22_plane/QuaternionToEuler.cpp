#include "QuaternionToEuler.h"

DirectX::SimpleMath::Vector3 QuaternionToEulerRad(const DirectX::SimpleMath::Quaternion& qf) {

	// クォータニオン → オイラー角（ラジアン）
	DirectX::SimpleMath::Vector3 euler;

	// Pitch (X)
	float sinr_cosp = 2.0f * (qf.w * qf.x + qf.y * qf.z);
	float cosr_cosp = 1.0f - 2.0f * (qf.x * qf.x + qf.y * qf.y);
	euler.x = std::atan2(sinr_cosp, cosr_cosp);

	// Yaw (Y)
	float sinp = 2.0f * (qf.w * qf.y - qf.z * qf.x);
	if (std::abs(sinp) >= 1)
		euler.y = std::copysign(DirectX::XM_PI / 2, sinp); // 90度クランプ
	else
		euler.y = std::asin(sinp);

	// Roll (Z)
	float siny_cosp = 2.0f * (qf.w * qf.z + qf.x * qf.y);
	float cosy_cosp = 1.0f - 2.0f * (qf.y * qf.y + qf.z * qf.z);
	euler.z = std::atan2(siny_cosp, cosy_cosp);

	return euler; // ラジアン
}

DirectX::SimpleMath::Vector3 QuaternionToEulerDeg(const DirectX::SimpleMath::Quaternion& qf) {
	return QuaternionToEulerRad(qf) * (180.0f / DirectX::XM_PI); // 度
}