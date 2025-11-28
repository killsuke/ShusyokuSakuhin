#include "Transform.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

TransformComponent::TransformComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TRANSFORM"); // ソート番号を設定
}

// 更新処理
void TransformComponent::Update() {

	GameObject* parent = m_Object->GetParent(); // 親オブジェクトを取得
	if (parent != nullptr) {
		m_transform.worldMatrix = MakeLocalMatrix() * parent->GetComponent<TransformComponent>()->GetWorldMatrix(); // 親のワールド行列を取得

		XMVECTOR scale;
		XMVECTOR rotation;
		XMVECTOR translation;

		XMFLOAT3 nowScale;
		XMFLOAT3 nowTranslation;

		DecomposeMatrix(m_transform.worldMatrix, scale, rotation, translation);	// ワールド行列からSRT情報を取得

		XMStoreFloat3(&nowScale, scale);
		XMStoreFloat3(&nowTranslation, translation);

		m_transform.m_Position = nowTranslation; // 位置を更新
		m_transform.m_Scale = nowScale; // スケールを更新
		m_transform.m_Quaternion = rotation;

		XMFLOAT3 rad = QuaternionToEulerRad(rotation);
		rad *= (180.0f / XM_PI);

		m_transform.m_Rotation = rad; // 回転を更新
	}
	else {
		// ワールド行列計算
		MakeWorldMatrix();

		// ローカル行列計算
	//	MakeLocalMatrix();
	}
}

XMMATRIX TransformComponent::MakeWorldMatrix() {

	// SRT情報作成
	// 各行列を生成
	const XMMATRIX r = XMMatrixRotationQuaternion(m_transform.m_Quaternion);
	const XMVECTOR scale = XMVectorSet(m_transform.m_Scale.x, m_transform.m_Scale.y, m_transform.m_Scale.z, 1.0f);
	const XMMATRIX s = XMMatrixScalingFromVector(scale);
	const XMVECTOR pos = XMVectorSet(m_transform.m_Position.x, m_transform.m_Position.y, m_transform.m_Position.z, 1.0f);
	const XMMATRIX t = XMMatrixTranslationFromVector(pos);

	// ワールド行列を作成し、保存
	return	m_transform.worldMatrix = s * r * t;
}

XMMATRIX TransformComponent::MakeLocalMatrix() {

	// SRT情報作成
	const XMMATRIX r = XMMatrixRotationQuaternion(m_transform.m_LocalQuaternion);
	const XMVECTOR scale = XMVectorSet(m_transform.m_LocalScale.x, m_transform.m_LocalScale.y, m_transform.m_LocalScale.z, 1.0f);
	const XMMATRIX s = XMMatrixScalingFromVector(scale);
	const XMVECTOR pos = XMVectorSet(m_transform.m_LocalPosition.x, m_transform.m_LocalPosition.y, m_transform.m_LocalPosition.z, 1.0f);
	const XMMATRIX t = XMMatrixTranslationFromVector(pos);

	// ローカル行列を作成し、保存
	return	m_transform.localMatrix = s * r * t;
}

void TransformComponent::MakeChildWorld() {
	GameObject* parent = m_Object->GetParent(); // 親オブジェクトを取得
	if (parent != nullptr) {
		TransformComponent* parentTrans = parent->GetComponent<TransformComponent>();

		XMMATRIX parentMtx = XMMatrixIdentity();

		if (parentTrans == nullptr) {
			parentTrans->MakeWorldMatrix(); // 親のワールド行列を更新
		}
		else {
			parentMtx = parentTrans->GetWorldMatrix(); // 親のワールド行列を取得
		}

		m_transform.worldMatrix = MakeLocalMatrix() * parentMtx; // 親のワールド行列とローカル行列を掛け合わせて子のワールド行列を計算

		XMVECTOR scale;
		XMVECTOR rotation;
		XMVECTOR translation;

		XMFLOAT3 nowScale;
		XMFLOAT3 nowTranslation;

		DecomposeMatrix(m_transform.worldMatrix, scale, rotation, translation);	// ワールド行列からSRT情報を取得

		XMStoreFloat3(&nowScale, scale);
		XMStoreFloat3(&nowTranslation, translation);

		m_transform.m_Position = nowTranslation; // 位置を更新
		m_transform.m_Scale = nowScale; // スケールを更新

		XMFLOAT3 rad = QuaternionToEulerRad(rotation);
		rad *= (180.0f / XM_PI);

		m_transform.m_Rotation = rad; // 回転を更新
	}
}
