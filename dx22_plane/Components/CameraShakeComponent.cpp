#include "CameraShakeComponent.h"
#include "Camera.h"
#include "Transform.h"
#include <cmath>
#include <random>

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

		const XMFLOAT3 newPos = RandomShake2D(view);

		// 位置更新
		trans->AddPosition(newPos);
		camera->AddTarget(newPos);

		m_RecordTime += DeltaTime;
		if (m_RecordTime > m_RequestTime) {
			m_RecordTime = 0.0f;
			m_RequestTime = 0.0f;
			m_ShakePower = 0.0f;
			m_ShakeSpeed = 0.0f;
			m_QuarterRequestTime = 0.0f;
			m_PrevShakeOffset = { 0.0f,0.0f,0.0f,0.0f };
		}
	}
}

DirectX::XMFLOAT3 CameraShakeComponent::WidthShake(const DirectX::XMMATRIX& view)const {

	XMVECTOR right = view.r[0];	// 1列目

	// 正規化
	right = XMVector3Normalize(right);

	// 横揺れ用オフセット
	const float offsetX = sinf(m_RecordTime * m_ShakePower) * m_ShakeSpeed;

	// 正規化したRightベクトルをオフセットに適用
	const XMVECTOR offset = XMVectorScale(right, offsetX);
	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, offset);

	return newPos;
}

DirectX::XMFLOAT3 CameraShakeComponent::HeightShake(const DirectX::XMMATRIX& view)const {

	XMVECTOR up = view.r[1];	// 2列目

	// 正規化
	up = XMVector3Normalize(up);

	// 横揺れ用オフセット
	const float offsetX = sinf(m_RecordTime * m_ShakePower) * m_ShakeSpeed;

	// 正規化したRightベクトルをオフセットに適用
	const XMVECTOR offset = XMVectorScale(up, offsetX);
	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, offset);

	return newPos;
}

DirectX::XMFLOAT3 CameraShakeComponent::DepthShake(const DirectX::XMMATRIX& view) const {

	XMVECTOR forward = view.r[2];	// 3列目

	// 正規化
	forward = XMVector3Normalize(forward);

	// 横揺れ用オフセット
	const float offsetX = sinf(m_RecordTime * m_ShakePower) * m_ShakeSpeed;

	// 正規化したRightベクトルをオフセットに適用
	const XMVECTOR offset = XMVectorScale(forward, offsetX);
	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, offset);

	return newPos;
}

DirectX::XMFLOAT3 CameraShakeComponent::RandomShake2D(const DirectX::XMMATRIX& view){

	const float quarterTime = m_RequestTime * 0.2f;
	
	if (m_RecordTime > m_QuarterRequestTime) {

		m_QuarterRequestTime += quarterTime;

		std::random_device rd;  // 非決定的な乱数の種
		std::mt19937 gen(rd()); // メルセンヌ・ツイスタ
		std::uniform_real_distribution<float> dist(-1.0f, 1.0f); // 範囲指定

		const float rX = dist(gen); // 0.0 ～ 1.0 の乱数
		const float rY = dist(gen); // 0.0 ～ 1.0 の乱数

		m_ShakeVector = XMVectorSet(rX, rY, 0.0f, 0.0f);
		m_ShakeVector = XMVector3Normalize(m_ShakeVector);
	}

	// ランダム揺れ用オフセット
	const float offsetX = sinf(m_RecordTime * m_ShakePower) * m_ShakeSpeed;

	// 正規化したRightベクトルをオフセットに適用
	const XMVECTOR offset = XMVectorScale(m_ShakeVector, offsetX);

	// 前フレームとの差分を取る
	// サイン波がゼロに戻るときに差分もゼロになるので、
	// 最終的に元の位置に戻る
	const XMVECTOR frameOffset = XMVectorSubtract(offset, m_PrevShakeOffset);

	m_PrevShakeOffset = offset;

	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, frameOffset);

	return newPos;
}