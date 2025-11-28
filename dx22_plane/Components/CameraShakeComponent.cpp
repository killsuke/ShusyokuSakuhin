#include "CameraShakeComponent.h"
#include "Camera.h"
#include "Transform.h"
#include <cmath>

using namespace DirectX;

namespace {
	constexpr float DeltaTime = 0.016f;
}

CameraShakeComponent::CameraShakeComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("CAMERASHAKE"); // ソート番号を設定
}

void CameraShakeComponent::Update() {

	Camera* camera = m_Object->GetComponent<Camera>();
	TransformComponent* trans = m_Object->GetComponent<TransformComponent>();

	if (camera != nullptr && trans != nullptr && m_RequestTime != 0.0f) {
		const XMMATRIX view = camera->GetView3D();

		XMVECTOR right = view.r[0];	// 1列目
		XMVECTOR up = view.r[1];	// 2列目
		XMVECTOR forward = view.r[2];	// 3列目

		// 正規化
		right = XMVector3Normalize(right);
		up = XMVector3Normalize(up);
		forward = XMVector3Normalize(forward);

		// 横揺れ用オフセット
		const float offsetX = sinf(m_RecordTime * m_ShakePower) * m_ShakeSpeed;

		// 正規化したRightベクトルをオフセットに適用
		const XMVECTOR offset = XMVectorScale(right, offsetX);
		XMFLOAT3 newPos;
		XMStoreFloat3(&newPos, offset);

		// 位置更新
		trans->AddPosition(newPos);
		camera->AddTarget(newPos);

		m_RecordTime += DeltaTime;
		if (m_RecordTime > m_RequestTime) {
			m_RecordTime = 0.0f;
			m_RequestTime = 0.0f;
			m_ShakePower = 0.0f;
			m_ShakeSpeed = 0.0f;
		}
	}
}

DirectX::XMFLOAT3 CameraShakeComponent::WidthShake(const DirectX::XMMATRIX& view) {

	XMVECTOR right = view.r[0];	// 1列目
	XMVECTOR up = view.r[1];	// 2列目
	XMVECTOR forward = view.r[2];	// 3列目

	// 正規化
	right = XMVector3Normalize(right);
	up = XMVector3Normalize(up);
	forward = XMVector3Normalize(forward);

	// 横揺れ用オフセット
	const float offsetX = sinf(m_RecordTime * m_ShakePower) * m_ShakeSpeed;

	// 正規化したRightベクトルをオフセットに適用
	const XMVECTOR offset = XMVectorScale(right, offsetX);
	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, offset);

	return newPos;
}

DirectX::XMFLOAT3 CameraShakeComponent::HeightShake(const DirectX::XMMATRIX& view) {

	XMVECTOR right = view.r[0];	// 1列目
	XMVECTOR up = view.r[1];	// 2列目
	XMVECTOR forward = view.r[2];	// 3列目

	// 正規化
	right = XMVector3Normalize(right);
	up = XMVector3Normalize(up);
	forward = XMVector3Normalize(forward);

	// 横揺れ用オフセット
	const float offsetX = sinf(m_RecordTime * m_ShakePower) * m_ShakeSpeed;

	// 正規化したRightベクトルをオフセットに適用
	const XMVECTOR offset = XMVectorScale(up, offsetX);
	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, offset);

	return newPos;
}

DirectX::XMFLOAT3 CameraShakeComponent::DepthShake(const DirectX::XMMATRIX& view) {

	XMVECTOR right = view.r[0];	// 1列目
	XMVECTOR up = view.r[1];	// 2列目
	XMVECTOR forward = view.r[2];	// 3列目

	// 正規化
	right = XMVector3Normalize(right);
	up = XMVector3Normalize(up);
	forward = XMVector3Normalize(forward);

	// 横揺れ用オフセット
	const float offsetX = sinf(m_RecordTime * m_ShakePower) * m_ShakeSpeed;

	// 正規化したRightベクトルをオフセットに適用
	const XMVECTOR offset = XMVectorScale(forward, offsetX);
	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, offset);

	return newPos;
}
