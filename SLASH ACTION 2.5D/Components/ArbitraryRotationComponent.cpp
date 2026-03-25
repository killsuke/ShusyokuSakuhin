#include "ArbitraryRotationComponent.h"
#include "TransformComponent.h"
#include "TrailRenderComponent.h"
#include "Manager/TimeManager.h"

using namespace DirectX;

namespace {
	constexpr float Deg180 = 180.0f;
}

ArbitraryRotationComponent::ArbitraryRotationComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("GO_AROUND"); // ソート番号を設定
	m_ArbitraryAxis = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
}

void ArbitraryRotationComponent::Update() {

	switch (m_rollingPattern)
	{
	case RollingPattern::DEFAULT_ROLLING:
		DefaultRollingMove();
		break;
	case RollingPattern::SWORD_SLASH:
	//	SwordSlashMove();
		break;
	default:
		break;
	}


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

void ArbitraryRotationComponent::DefaultRollingMove() {

	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	TransformComponent* centerTrans = m_CenterObject->GetComponent<TransformComponent>();
	const float deltaTime = TimeManager::GetFixedDeltaTime();

	if(transform == nullptr || centerTrans == nullptr) {
		return; // トランスフォームが取得できない場合は何もしない
	}

	XMFLOAT3 centerPos = centerTrans->GetPosition();

	// 反転要求があれば、現在の角度を反転
	if (m_flipRequested == true) {

		m_nowAngleRadian = -m_nowAngleRadian;

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

		const float rotationSpeed = deltaTime * m_rotationSpeed;
		// 時計回りか反時計回りか
		if (m_clockwise == true) {
			m_nowAngleRadian -= rotationSpeed; // 角度を更新間隔時間に基づいて計算
		}
		else {
			m_nowAngleRadian += rotationSpeed; // 角度を更新間隔時間に基づいて計算
		}
	}

	// ラジアンに変換
	const float startRad = XMConvertToRadians(m_StartAngle);

	bool reached = false;

	// 到達判定
	if (fabs(m_nowAngleRadian) > m_TargetRotationAmount) {
		reached = true;
	}
	if (reached) {

		// 到達後の猶予時間
		if (m_RecordTime < m_StopTime) {
			m_RecordTime += deltaTime;
			m_IsFinished = true;
		}
		else {
			m_rollingActive = false;
			m_IsActiveFlag = false;
		}

		// 目標角度に到達したら、正確に目標角度に合わせる
		if (m_clockwise == true) {
			m_nowAngleRadian = -m_TargetRotationAmount;
		}
		else {
			m_nowAngleRadian = m_TargetRotationAmount;
		}
	}

	float totalAngle = m_nowAngleRadian + startRad;

	totalAngle = NormalizeAngleRadian(totalAngle);

	m_nowAngleDegree = XMConvertToDegrees(totalAngle);	// ラジアン → ディグリー

	XMVECTOR baseOffset = XMVectorSet(m_radius, 0.0f, 0.0f, 0.0f);
	XMVECTOR axis = m_ArbitraryAxis;	// 回転軸を設定
	axis = XMVector3Normalize(axis);

	XMVECTOR rotationQuat = XMQuaternionRotationAxis(axis, totalAngle);

	XMVECTOR rotatedOffset = XMVector3Rotate(baseOffset, rotationQuat);

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

	rotationQuat = XMQuaternionMultiply(localQuat, rotationQuat);

	transform->SetQuaternion(rotationQuat);

	XMFLOAT3 newPos = {};
	XMStoreFloat3(&newPos, newVec);
	transform->SetPosition(newPos);
}

void ArbitraryRotationComponent::SwordSlashMove() {

	//auto transform = m_Object->GetComponent<TransformComponent>();

	//auto centerTrans = m_CenterObject->GetComponent<TransformComponent>();
	//auto centerPos = centerTrans->GetPosition();

	//float totalAngle = 0.0f;
	//XMVECTOR axis = m_ArbitraryAxis;	// 回転軸を設定
	//XMVECTOR baseOffset = XMVectorSet(m_radius, 0.0f, 0.0f, 0.0f);

	//// 実行順によってはバグる可能性があるので後に確認
	//// 反転要求があれば、現在の角度を反転
	//if (m_flipRequested == true) {

	//	float angle1 = XMConvertToDegrees(m_nowAngleRadian);
	//	m_nowAngleRadian = -m_nowAngleRadian;
	//	float angle2 = XMConvertToDegrees(m_nowAngleRadian);

	//	// スタートとゴール時の回転角度を反転
	//	if (m_StartAngle > 0) {
	//		m_StartAngle = -m_StartAngle + Deg180;
	//	}
	//	else {
	//		m_StartAngle = m_StartAngle + Deg180;
	//	}
	//	if (m_EndAngle > 0) {
	//		m_EndAngle = -m_EndAngle + Deg180;
	//	}
	//	else {
	//		m_EndAngle = m_EndAngle + Deg180;
	//	}

	//	m_flipRequested = false;
	//}



	//if (m_SwordSlashFrameCount == 0) {

	//	// 1フレーム目は構え
	//	totalAngle = XMConvertToRadians(m_StartAngle);
	//	totalAngle = NormalizeAngleRadian(totalAngle);
	//	m_nowAngleRadian = totalAngle;
	//	m_nowAngleDegree = XMConvertToDegrees(totalAngle);	// ラジアン → ディグリー

	//}
	//else if (m_SwordSlashFrameCount == 1) {

	//	// 2フレーム目にワープ
	//	totalAngle = m_WarpRadian + XMConvertToRadians(m_StartAngle);
	//	totalAngle = NormalizeAngleRadian(totalAngle);
	//	if (m_clockwise == true) {
	//		m_nowAngleRadian = (m_WarpRadian * -0.5f); // 現在の角度もワープ角度に更新
	//	}
	//	else {
	//		m_nowAngleRadian = (m_WarpRadian * 0.5f); // 現在の角度もワープ角度に更新
	//	}
	//	m_nowAngleDegree = XMConvertToDegrees(totalAngle);	// ラジアン → ディグリー

	//}
	//else {

	//	// 回転のみを停止、位置更新はする
	//	if (m_rollingActive == true) {

	//		const float rotationSpeed = DeltaTime * m_rotationSpeed;
	//		// 時計回りか反時計回りか
	//		if (m_clockwise == true) {
	//			m_nowAngleRadian -= rotationSpeed; // 角度を更新間隔時間に基づいて計算
	//		}
	//		else {
	//			m_nowAngleRadian += rotationSpeed; // 角度を更新間隔時間に基づいて計算
	//		}
	//	}

	//	// ラジアンに変換
	//	const float startRad = XMConvertToRadians(m_StartAngle);

	//	bool reached = false;

	//	// 到達判定
	//	if (fabs(m_nowAngleRadian) > m_TargetRotationAmount) {
	//		reached = true;
	//	}
	//	if (reached) {

	//		if (m_RecordTime < 0.2f) {
	//			m_RecordTime += DeltaTime;
	//			m_IsFinished = true;
	//		}
	//		else {
	//			m_rollingActive = false;
	//			m_IsActiveFlag = false;
	//		}

	//		// 目標角度に到達したら、正確に目標角度に合わせる
	//		if (m_clockwise == true) {
	//			m_nowAngleRadian = -m_TargetRotationAmount;
	//		}
	//		else {
	//			m_nowAngleRadian = m_TargetRotationAmount;
	//		}
	//	}


	//	totalAngle = m_nowAngleRadian + startRad;

	//	totalAngle = NormalizeAngleRadian(totalAngle);

	//	m_nowAngleDegree = XMConvertToDegrees(totalAngle);	// ラジアン → ディグリー
	//}

	//axis = XMVector3Normalize(axis);
	//XMVECTOR rotationQuat = XMQuaternionRotationAxis(axis, totalAngle);
	//XMVECTOR rotatedOffset = XMVector3Rotate(baseOffset, rotationQuat);
	//XMVECTOR centerPosVec = XMLoadFloat3(&centerPos);
	//XMVECTOR newVec = XMVectorAdd(centerPosVec, rotatedOffset);
	//float pitch = DirectX::XMConvertToRadians(m_LockRotation.x);
	//float yaw = DirectX::XMConvertToRadians(m_LockRotation.y);
	//float roll = DirectX::XMConvertToRadians(m_LockRotation.z);
	//XMVECTOR localQuat = DirectX::XMQuaternionRotationRollPitchYaw(
	//	pitch,
	//	yaw,
	//	roll
	//);
	//rotationQuat = XMQuaternionMultiply(localQuat, rotationQuat);
	//transform->SetQuaternion(rotationQuat);
	//XMFLOAT3 newPos = {};
	//XMStoreFloat3(&newPos, newVec);
	//transform->SetPosition(newPos);

	//// 1フレーム目は構えて、
	//// 2フレーム目にワープ
	//// それ以降は普通に回転
	//m_SwordSlashFrameCount++;

}

void ArbitraryRotationComponent::SimulationMove() {

	TrailRenderComponent* trailRender = m_Object->GetComponent<TrailRenderComponent>();
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	TransformComponent* centerTrans = m_CenterObject->GetComponent<TransformComponent>();
	const float deltaTime = TimeManager::GetFixedDeltaTime();

	if (trailRender != nullptr && transform != nullptr && centerTrans != nullptr) {
		m_worldPosQuats.clear();
		
		const XMFLOAT3 centerPos = centerTrans->GetPosition();
		const XMVECTOR centerQuat = centerTrans->GetQuaternion();
		
		// ラジアンに変換
		const float startRad = XMConvertToRadians(m_StartAngle);
		const float endRad = XMConvertToRadians(m_EndAngle);

		float rotationSpeed = 0.0f;

		// 時計回りか反時計回りか
		if (m_clockwise == true) {
			rotationSpeed = -deltaTime * m_rotationSpeed;
		}
		else {
			rotationSpeed = deltaTime * m_rotationSpeed;
		}

		float simulationRad = m_nowAngleRadian;

		// 剣の軌跡用の挙動の予測
		// １フレーム分軌跡が足りないのでここで足す
		while (fabs(simulationRad) < m_TargetRotationAmount) {

			simulationRad += rotationSpeed; // 角度を更新間隔時間に基づいて計算

			// 正確に合わせるならこの処理が必要だが、なぜかこれを使うとちょっとズレる
			// なので後に修正
			if (fabs(simulationRad) > m_TargetRotationAmount) {
				// 目標角度に到達したら、正確に目標角度に合わせる
				if (m_clockwise == true) {
					simulationRad = -m_TargetRotationAmount;
				}
				else {
					simulationRad = m_TargetRotationAmount;
				}
			}

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

		trailRender->SetTrailPoint(m_worldPosQuats);
	}
}
