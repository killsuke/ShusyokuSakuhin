#include "GoAroundComponent.h"
#include "Transform.h"
#include <SimpleMath.h>
#include "GameObjectManager.h"
#include "HelpMath.h"
#include <iostream>

using namespace DirectX::SimpleMath;

GoAroundComponent::GoAroundComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("GO_AROUND"); // ソート番号を設定
}

void GoAroundComponent::Update()
{
	auto transform = m_Object->GetComponent<TransformComponent>();
	auto rollingObjPos = transform->GetPosition();

	auto centerTrans = m_CenterObject->GetComponent<TransformComponent>();
	auto centerPos = centerTrans->GetPosition();

	// 回転のみを停止、位置更新はする
	if (m_rollingActive == true) {
		// 時計回りか反時計回りか
		if (m_clockwise == true) {
			m_nowAngleRadian -= m_deltaTime * m_rotationSpeed; // 角度を更新間隔時間に基づいて計算
		}
		else {
			m_nowAngleRadian += m_deltaTime * m_rotationSpeed; // 角度を更新間隔時間に基づいて計算
		}
	}

	// 反転要求があれば、現在の角度を反転
	if (m_flipRequested == true) {

		m_nowAngleRadian = -m_nowAngleRadian;
		m_flipRequested = false;
	}

	float totalAngle = m_nowAngleRadian + m_initialAngleRadian;

	m_nowAngleDegree = totalAngle * (180.0f / DirectX::XM_PI);	// ラジアン → ディグリー

	// 現在の位置 == 角度として捉えたいので、戻し処理を入れる
	if ((std::abs(m_nowAngleDegree) + std::abs(m_initialAngleDegree)) > 360.0f && m_clockwise == true) {	// 時計回り
		m_nowAngleRadian = 0.0f;
	}
	else if ((std::abs(m_nowAngleDegree) - std::abs(m_initialAngleDegree)) > 360.0f && m_clockwise == false) {	// 反時計回り
		m_nowAngleRadian = 0.0f;
	}

	Vector3 baseOffset = Vector3(m_radius, 0.0f, 0.0f);
	Vector3 axis = m_RotationAxis;	// 回転軸を設定
	axis.Normalize();

	Quaternion rotationQuat = Quaternion::CreateFromAxisAngle(axis, totalAngle);

	Vector3 rotatedOffset = Vector3::Transform(baseOffset, rotationQuat);

	Vector3 directionToCenter = rotatedOffset;
	directionToCenter.Normalize();

	// 半径方向 (中心 → オブジェクト）
	Vector3 radial = rotatedOffset;
	radial.Normalize();

	// 接線方向（回転方向に垂直・進行方向）
	Vector3 tangent = axis.Cross(radial);
	tangent.Normalize();

	rotationQuat = Quaternion(-rotationQuat.x, -rotationQuat.y, -rotationQuat.z, rotationQuat.w);

	Vector3 rotDeg = QuaternionToEulerDeg(rotationQuat);

	transform->SetRotation(Vector3(rotDeg.x + m_LockRotation.x, rotDeg.y + m_LockRotation.y, rotDeg.z + m_LockRotation.z));

	transform->SetPosition(centerPos + rotatedOffset);
}