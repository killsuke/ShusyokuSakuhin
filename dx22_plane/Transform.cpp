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

		// 親のワールド行列を取得がもしかすると回転辺りで不具合を起こすかもしれない
		m_transform.worldMatrix.Decompose(scale, rotation, translation); // ワールド行列からSRT情報を取得

		m_transform.m_Position = translation; // 位置を更新
		m_transform.m_Scale = scale; // スケールを更新
		m_transform.m_Quaternion = rotation;

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
	
	// SRT情報作成
	// 各行列を生成
	Matrix r = Matrix::CreateFromQuaternion(m_transform.m_Quaternion);
	Matrix s = Matrix::CreateScale(m_transform.m_Scale);
	Matrix t = Matrix::CreateTranslation(m_transform.m_Position);

	// ワールド行列を作成し、保存
	return	m_transform.worldMatrix = s * r * t;
}

Matrix TransformComponent::MakeLocalMatrix() {
	
	// SRT情報作成
	Matrix r = Matrix::CreateFromQuaternion(m_transform.m_LocalQuaternion);
	Matrix s = Matrix::CreateScale(m_transform.m_LocalScale);
	Matrix t = Matrix::CreateTranslation(m_transform.m_LocalPosition);

	// ローカル行列を作成し、保存
	return	m_transform.localMatrix = s * r * t;
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
