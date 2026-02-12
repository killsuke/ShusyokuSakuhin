#include "Transform.h"

using namespace DirectX;

TransformComponent::TransformComponent(GameObject& obj) :Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TRANSFORM"); // ソート番号を設定
}

// 更新処理
void TransformComponent::Update() {

	GameObject* parent = m_Object->GetParent(); // 親オブジェクトを取得
	if (parent != nullptr) {

		MakeChildMatrixAndWorld();
	}
	else {
		// ワールド行列計算
		MakeWorldMatrix();
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

	switch (m_RotationPattern)
	{
	case RotationPattern::SPIN:			// 自転

		m_transform.localMatrix = s * r * t;
		break;
	case RotationPattern::REVOLUTION:	// 公転

		m_transform.localMatrix = s * t * r;
		break;
	default:
		break;
	}

	// ローカル行列を作成し、保存
	return	m_transform.localMatrix;
}

void TransformComponent::MakeChildWorld() {
	GameObject* parent = m_Object->GetParent(); // 親オブジェクトを取得
	if (parent != nullptr) {
		TransformComponent* parentTrans = parent->GetComponent<TransformComponent>();

		XMMATRIX parentMtx = XMMatrixIdentity();

		if (parentTrans == nullptr) {
			return;
		}
		else {
			const XMFLOAT3 parentPos = parentTrans->GetPosition();
			const XMVECTOR pos = XMVectorSet(parentPos.x, parentPos.y, parentPos.z, 1.0f);
			const XMMATRIX t = XMMatrixTranslationFromVector(pos);
			// スケールの影響を受けないようにする
			const XMFLOAT3 parentScale = parentTrans->GetScale();
			XMVECTOR scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
			if (m_IsLockScale == false) {
				scale = XMVectorSet(parentScale.x, parentScale.y, parentScale.z, 1.0f);
			}
			const XMMATRIX s = XMMatrixScalingFromVector(scale);
			const XMVECTOR parentQuat = parentTrans->GetQuaternion();
			const XMMATRIX r = XMMatrixRotationQuaternion(parentQuat);

			parentMtx = s * r * t;
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

// 自転で作る子供の行列
DirectX::XMMATRIX TransformComponent::MakeChildMatrix() {

	GameObject* parent = m_Object->GetParent(); // 親オブジェクトを取得
	if (parent != nullptr) {

		TransformComponent* trans = parent->GetComponent<TransformComponent>();

		if (trans == nullptr) {
			return XMMatrixIdentity();
		}

		const XMMATRIX childMtx = MakeLocalMatrix();

		const XMVECTOR parentQuat = trans->GetQuaternion();
		const XMFLOAT3 parentScale = trans->GetScale();
		const XMFLOAT3 parentPos = trans->GetPosition();

		const XMMATRIX r = XMMatrixRotationQuaternion(parentQuat);

		XMVECTOR scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		if (m_IsLockScale == false) {
			scale = XMVectorSet(parentScale.x, parentScale.y, parentScale.z, 1.0f);
		}
		const XMMATRIX s = XMMatrixScalingFromVector(scale);
		const XMVECTOR pos = XMVectorSet(parentPos.x, parentPos.y, parentPos.z, 1.0f);
		const XMMATRIX t = XMMatrixTranslationFromVector(pos);

		return	m_transform.worldMatrix = childMtx * (s * r * t);
	}

	return XMMatrixIdentity();
}

DirectX::XMMATRIX TransformComponent::MakeChildMatrixAndWorld() {

	XMMATRIX mtx = XMMatrixIdentity();
	mtx = MakeChildMatrix();

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

	return mtx;
}