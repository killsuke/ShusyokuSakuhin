#include "Transform.h"
using namespace DirectX::SimpleMath;

TransformComponent::TransformComponent(GameObject& obj) :Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("TRANSFORM"); // ソート番号を設定
}

// 更新処理
void TransformComponent::Update() {

	auto parent = p_object->GetParent(); // 親オブジェクトを取得
	if (parent != nullptr) {
		m_transform.worldMatrix = MakeLocalMatrix() * parent->GetComponent<TransformComponent>()->GetWorldMatrix(); // 親のワールド行列を取得

		Vector3 scale;
		Quaternion rotation;
		Vector3 translation;

		m_transform.worldMatrix.Decompose(scale, rotation, translation); // ワールド行列からSRT情報を取得

		m_transform.m_Position = translation; // 位置を更新
		m_transform.m_Scale = scale; // スケールを更新

		Vector3 rad = QuaternionToEulerRad(rotation);
		rad * (180.0f / DirectX::XM_PI);

		m_transform.m_Rotation = rad; // 回転を更新
	}
	else {
		// ワールド行列計算
		MakeWorldMatrix();

		// ローカル行列計算
	//	MakeLocalMatrix();
	}

}

Matrix TransformComponent::MakeWorldMatrix() {
	// 「 度 」から「 ラジアン 」へ変換
	float PitchRadians = DirectX::XMConvertToRadians(m_transform.m_Rotation.x); // X軸回転
	float YawRadians = DirectX::XMConvertToRadians(m_transform.m_Rotation.y);     // Y軸回転
	float RollRadians = DirectX::XMConvertToRadians(m_transform.m_Rotation.z);   // Z軸回転

	// クォータニオン作成
	Quaternion q = Quaternion::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);

	// SRT情報作成
	// 各行列を生成
	Matrix r = Matrix::CreateFromQuaternion(q);
	Matrix s = Matrix::CreateScale(m_transform.m_Scale);
	Matrix t = Matrix::CreateTranslation(m_transform.m_Position);


	// ワールド行列を作成し、保存
	return	m_transform.worldMatrix = s * r * t;
}

Matrix TransformComponent::MakeLocalMatrix() {
	float PitchRadians = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.x); // X軸回転
	float YawRadians = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.y);     // Y軸回転
	float RollRadians = DirectX::XMConvertToRadians(m_transform.m_LocalRotation.z);   // Z軸回転

	// クォータニオン作成
	Quaternion q = Quaternion::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);

	// SRT情報作成
	Matrix r = Matrix::CreateFromQuaternion(q);
	Matrix s = Matrix::CreateScale(m_transform.m_LocalScale);
	Matrix t = Matrix::CreateTranslation(m_transform.m_LocalPosition);

	// ローカル行列を作成し、保存
	return	m_transform.localMatrix = s * r * t;
	//	m_transform.localMatrix = m_transform.localMatrix.Transpose(); // 行列を転置
}

void TransformComponent::MakeChildWorld() {
	auto parent = p_object->GetParent(); // 親オブジェクトを取得
	if(parent != nullptr) {
		auto parentMtx = parent->GetComponent<TransformComponent>()->MakeWorldMatrix(); // 親のワールド行列を更新
		m_transform.worldMatrix = MakeLocalMatrix() * parentMtx; // 親のワールド行列とローカル行列を掛け合わせて子のワールド行列を計算

		Vector3 scale;
		Quaternion rotation;
		Vector3 translation;

		m_transform.worldMatrix.Decompose(scale, rotation, translation); // ワールド行列からSRT情報を取得

		m_transform.m_Position = translation; // 位置を更新
		m_transform.m_Scale = scale; // スケールを更新

		Vector3 rad = QuaternionToEulerRad(rotation);
		rad* (180.0f / DirectX::XM_PI);

		m_transform.m_Rotation = rad; // 回転を更新
	}
}

Vector3 TransformComponent::QuaternionToEulerRad(const Quaternion& qf)
{
	// クォータニオン → オイラー角（ラジアン）
	Vector3 euler;

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