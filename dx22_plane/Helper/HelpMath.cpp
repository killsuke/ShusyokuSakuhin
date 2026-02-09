#include "Helper/HelpMath.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

DirectX::SimpleMath::Vector3 QuaternionToEulerRad(const DirectX::SimpleMath::Quaternion& qf) {

	// クォータニオン → オイラー角（ラジアン）
	Vector3 euler;

	// Pitch (X)
	float sinr_cosp = 2.0f * (qf.w * qf.x + qf.y * qf.z);
	float cosr_cosp = 1.0f - 2.0f * (qf.x * qf.x + qf.y * qf.y);
	euler.x = std::atan2(sinr_cosp, cosr_cosp);

	// Yaw (Y)
	float sinp = 2.0f * (qf.w * qf.y - qf.z * qf.x);
	if (std::abs(sinp) >= 1)
		euler.y = std::copysign(XM_PI / 2, sinp); // 90度クランプ
	else
		euler.y = std::asin(sinp);

	// Roll (Z)
	float siny_cosp = 2.0f * (qf.w * qf.z + qf.x * qf.y);
	float cosy_cosp = 1.0f - 2.0f * (qf.y * qf.y + qf.z * qf.z);
	euler.z = std::atan2(siny_cosp, cosy_cosp);

	return euler; // ラジアン
}

DirectX::SimpleMath::Vector3 QuaternionToEulerDeg(const DirectX::SimpleMath::Quaternion& qf) {
	return QuaternionToEulerRad(qf) * (180.0f / XM_PI); // 度
}

// Roll → Pitch → Yaw の順に回転させることを前提とする
// クォータニオンからオイラー角への変換処理	
DirectX::XMFLOAT3 QuaternionToEulerRad(const DirectX::XMVECTOR& q) {

	XMFLOAT4 qf;
	XMStoreFloat4(&qf, q);

	const float roll = atan2f(2.0f * (qf.w * qf.z + qf.x * qf.y), 1.0f - 2.0f * (qf.z * qf.z + qf.x * qf.x));
	const float pitch = asinf(2.0f * (qf.w * qf.x - qf.y * qf.z));
	const float yaw = atan2f(2.0f * (qf.w * qf.y + qf.z * qf.x), 1.0f - 2.0f * (qf.x * qf.x + qf.y * qf.y));

	return XMFLOAT3(pitch, yaw, roll);
}

DirectX::XMFLOAT3 QuaternionToEulerDeg(const DirectX::XMVECTOR& q) {
	
	XMFLOAT3 rad = QuaternionToEulerRad(q);
	XMFLOAT3 deg = {
		XMConvertToDegrees(rad.x),
		XMConvertToDegrees(rad.y),
		XMConvertToDegrees(rad.z)
	};

	return deg; // 度
}

void DecomposeMatrix(const DirectX::XMMATRIX& matrix, DirectX::XMVECTOR& outScale, DirectX::XMVECTOR& outRotation, DirectX::XMVECTOR& outTranslation) {

	// 位置
	// そのまま取り出せばOK
	outTranslation = XMVectorSet(
		matrix.r[3].m128_f32[0],
		matrix.r[3].m128_f32[1],
		matrix.r[3].m128_f32[2],
		0.0f);

	// スケール
	// 各軸の長さを取る
	XMVECTOR scaleX = XMVector3Length(matrix.r[0]);
	XMVECTOR scaleY = XMVector3Length(matrix.r[1]);
	XMVECTOR scaleZ = XMVector3Length(matrix.r[2]);

	// その中から各軸の成分を取り出し
	outScale = XMVectorSet(
		XMVectorGetX(scaleX),
		XMVectorGetY(scaleY),
		XMVectorGetZ(scaleZ),
		0.0f);

	// 回転
	XMMATRIX rotMatrix = matrix;

	// 軸の向きのみが欲しいのでノーマライズ
	rotMatrix.r[0] = XMVector3Normalize(rotMatrix.r[0]);
	rotMatrix.r[1] = XMVector3Normalize(rotMatrix.r[1]);
	rotMatrix.r[2] = XMVector3Normalize(rotMatrix.r[2]);

	// クォータニオン生成
	outRotation = XMQuaternionRotationMatrix(rotMatrix);
}