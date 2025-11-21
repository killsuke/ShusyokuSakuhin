#include "ArbitraryRotationComponent.h"
#include "Transform.h"
#include "TrailRenderComponent.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace {
	constexpr float DeltaTime = 0.016f;
	constexpr float Deg180 = 180.0f;
}

ArbitraryRotationComponent::ArbitraryRotationComponent(GameObject& obj) : Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("GO_AROUND"); // ソート番号を設定
	m_ArbitraryAxis = XMVectorSet(0.0f,0.0f,1.0f,1.0f);
}

void ArbitraryRotationComponent::Update() {

	auto transform = m_Object->GetComponent<TransformComponent>();

	auto centerTrans = m_CenterObject->GetComponent<TransformComponent>();
	auto centerPos = centerTrans->GetPosition();

	// 反転要求があれば、現在の角度を反転
	if (m_flipRequested == true) {
		
		float angle1 = XMConvertToDegrees(m_nowAngleRadian);
		m_nowAngleRadian = -m_nowAngleRadian;
		float angle2 = XMConvertToDegrees(m_nowAngleRadian);

		// スタートとゴール時の回転角度を反転
		if (m_StartAngle > 0) {
			m_StartAngle = -m_StartAngle + Deg180;
		}
		else {
			m_StartAngle = m_StartAngle + Deg180;
		}
		if (m_EndAngle > 0) {
			m_EndAngle = -m_EndAngle + Deg180;
		}
		else {
			m_EndAngle = m_EndAngle + Deg180;
		}

		m_flipRequested = false;
	}

	// 回転のみを停止、位置更新はする
	if (m_rollingActive == true) {

		// 時計回りか反時計回りか
		if (m_clockwise == true) {
			m_nowAngleRadian -= DeltaTime * m_rotationSpeed; // 角度を更新間隔時間に基づいて計算
		}
		else {
			m_nowAngleRadian += DeltaTime * m_rotationSpeed; // 角度を更新間隔時間に基づいて計算
		}
	}

	// ラジアンに変換
	const float startRad = XMConvertToRadians(m_StartAngle);
	const float endRad = XMConvertToRadians(m_EndAngle);

	float totalAngle = m_nowAngleRadian + startRad;

	totalAngle = NormalizeAngleRadian(totalAngle);

	m_nowAngleDegree = totalAngle * (Deg180 / XM_PI);	// ラジアン → ディグリー

	bool reached = false;

	// 到達判定
	if (fabs(m_nowAngleRadian) > m_TargetRotationAmount) {
		reached = true;
	}
	if (reached) {
		m_rollingActive = false;
		m_activeFlag = false;

		// 誤差を完全に抑えて、角度を目標値に固定
		m_nowAngleRadian = XMConvertToRadians(m_EndAngle - m_StartAngle);
	}

	XMVECTOR baseOffset = XMVectorSet(m_radius, 0.0f, 0.0f, 0.0f);
	XMVECTOR axis = m_ArbitraryAxis;	// 回転軸を設定
	axis = XMVector3Normalize(axis);

	XMVECTOR rotationQuat = XMQuaternionRotationAxis(axis, totalAngle);

	XMVECTOR rotatedOffset = XMVector3Rotate(baseOffset, rotationQuat);

	// 半径方向 (中心 → オブジェクト）
//	XMVECTOR radial = XMVector3Normalize(rotatedOffset);

	// 接線方向（回転方向に垂直・進行方向）
	//XMVECTOR tangent = XMVector3Normalize(XMVector3Cross(axis, radial));

	XMVECTOR centerPosVec = XMLoadFloat3(&centerPos);
	XMVECTOR newVec = XMVectorAdd(centerPosVec, rotatedOffset);

	float pitch = DirectX::XMConvertToRadians(m_LockRotation.x);
	float yaw = DirectX::XMConvertToRadians(m_LockRotation.y);
	float roll = DirectX::XMConvertToRadians(m_LockRotation.z);

	XMVECTOR localQuat = DirectX::XMQuaternionRotationRollPitchYaw(
		pitch,
		yaw,
		roll
	);

	rotationQuat = XMQuaternionMultiply(localQuat,rotationQuat);

	transform->SetQuaternion(rotationQuat);

	XMFLOAT3 newPos = {};
	XMStoreFloat3(&newPos, newVec);
	transform->SetPosition(newPos);
}

float ArbitraryRotationComponent::NormalizeAngleDegree(float deg) {

	float result = fmodf(deg, Deg180 * 2.0f);

	if (result < 0.0f) {
		result += Deg180 * 2.0f;
	}

	return result;
}

float ArbitraryRotationComponent::NormalizeAngleRadian(float rad) {

	constexpr float twoPi = XM_2PI;
	float result = fmodf(rad, twoPi);
	if (result < 0.0f) {
		result += twoPi;
	}
	return result;
}

void ArbitraryRotationComponent::SimulationMove() {

	m_worldPosQuats.clear();
	auto transform = m_Object->GetComponent<TransformComponent>();
	auto centerTrans = m_CenterObject->GetComponent<TransformComponent>();
	const auto centerPos = centerTrans->GetPosition();
	const auto centerQuat = centerTrans->GetQuaternion();

	// ラジアンに変換
	const float startRad = XMConvertToRadians(m_StartAngle);
	const float endRad = XMConvertToRadians(m_EndAngle);

	float rotationSpeed = 0.0f;

	// 時計回りか反時計回りか
	if (m_clockwise == true) {
		rotationSpeed = -DeltaTime * m_rotationSpeed;
	}
	else {
		rotationSpeed = DeltaTime * m_rotationSpeed;
	}

	float simulationRad = m_nowAngleRadian;

	// 剣の軌跡用の挙動の予測
	while (fabs(simulationRad) < m_TargetRotationAmount) {

		simulationRad += rotationSpeed; // 角度を更新間隔時間に基づいて計算

		float totalAngle = simulationRad + startRad;
		totalAngle = NormalizeAngleRadian(totalAngle);

		const XMVECTOR baseOffset = XMVectorSet(m_radius, 0.0f, 0.0f, 0.0f);
		XMVECTOR axis = m_ArbitraryAxis;	// 回転軸を設定
		axis = XMVector3Normalize(axis);

		const XMVECTOR rotationQuat = XMQuaternionRotationAxis(axis, totalAngle);

		const XMVECTOR rotatedOffset = XMVector3Rotate(baseOffset, rotationQuat);

		XMFLOAT3 newPos = {};
		XMStoreFloat3(&newPos, rotatedOffset);
		m_worldPosQuats.push_back(PosAndQuaternion(newPos, rotationQuat));
	}

	TrailRenderComponent* trailRender = m_Object->GetComponent<TrailRenderComponent>();
	
	if (trailRender != nullptr) {
		trailRender->SetTrailPoint(m_worldPosQuats);
	}
}