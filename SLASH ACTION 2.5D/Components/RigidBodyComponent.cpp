#include "RigidBodyComponent.h"
#include "Manager/TimeManager.h"

using namespace DirectX;

namespace {
	constexpr float DEFAULT_GRAVITY_STOP = 100.0f; // 重力固定パワー（デフォルト）
	constexpr float DEFAULT_FIRSTFALLMAGNIFICATION = 120.0f; // 初回の落下倍率（デフォルト）
	constexpr float DEFAULT_FALLMAGNIFICATION = 12.0f; // 落下倍率（デフォルト）
	constexpr float GRAVITY = 9.80665f;	  // 重力
	constexpr float FRICTION = 0.1f;	  // 摩擦係数
	constexpr float RESTITUTION = 0.8f;	  // 反発係数（０～１ぐらいが一般的らしい）、この値が大きいほど反発も大きくなる
	constexpr float AIRRESISTANCE = 0.05f; // 空気抵抗係数
	constexpr float MIN_VELOCITY_THRESHOLD = 10.0f;	// 最小速度のしきい値
	constexpr float STOPVELOCITY = 0.03f;	// この値を下回ったら値を０にする 
}

RigidBodyComponent::RigidBodyComponent(GameObject& obj) : m_Velocity{ 0.0f,0.0f,0.0f }, m_Acceleration{ 0.0f,0.0f,0.0f }, m_Mass(1.0f), Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RIGIDBODY"); // ソート番号を設定
	m_StopGravity = DEFAULT_GRAVITY_STOP;
	m_FirstFallMagnification = DEFAULT_FIRSTFALLMAGNIFICATION;
	m_FallMagnification = DEFAULT_FALLMAGNIFICATION;
}

void RigidBodyComponent::Update() {
	if (m_GravityFlag == true) {
		UseGravity(true);
	}

	UpdateVelocity();
}

void RigidBodyComponent::UpdateVelocity() {

	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr) {

		const float FIXED_DELTATIME = TimeManager::GetFixedDeltaTime();	// 固定フレームレートのデルタタイムを取得

		// 力から加速度を更新
		m_Acceleration.x = m_TotalForce.x / m_Mass; // 合力を質量で割って加速度を計算
		m_Acceleration.y = m_TotalForce.y / m_Mass; // 合力を質量で割って加速度を計算
		m_Acceleration.z = m_TotalForce.z / m_Mass; // 合力を質量で割って加速度を計算

		// 加速度から速度を更新
		m_Velocity.x += m_Acceleration.x * FIXED_DELTATIME;
		m_Velocity.y += m_Acceleration.y * FIXED_DELTATIME;
		m_Velocity.z += m_Acceleration.z * FIXED_DELTATIME;

		// 速度制限
		if (m_LimitVelocity.x != 0.0f && fabsf(m_Velocity.x) > fabsf(m_LimitVelocity.x)) {
			m_Velocity.x = (m_Velocity.x > 0) ? m_LimitVelocity.x : -m_LimitVelocity.x;
		}
		if (m_LimitVelocity.y != 0.0f && fabsf(m_Velocity.y) > fabsf(m_LimitVelocity.y)) {
			m_Velocity.y = (m_Velocity.y > 0) ? m_LimitVelocity.y : -m_LimitVelocity.y;
		}
		if (m_LimitVelocity.z != 0.0f && fabsf(m_Velocity.z) > fabsf(m_LimitVelocity.z)) {
			m_Velocity.z = (m_Velocity.z > 0) ? m_LimitVelocity.z : -m_LimitVelocity.z;
		}


		XMFLOAT3 newPos = {};

		// 速度からTransformの位置を更新
		newPos.x += m_Velocity.x * FIXED_DELTATIME;
		newPos.y += m_Velocity.y * FIXED_DELTATIME;
		newPos.z += m_Velocity.z * FIXED_DELTATIME;

		transform->AddPosition(newPos); // 位置を更新
		CheckStopVelocity();

		// 毎フレーム加速度をリセット（次のフレームの外力のみ反映）
		m_Acceleration = { 0.0f, 0.0f, 0.0f };
		m_TotalForce = { 0.0f, 0.0f, 0.0f }; // 合力もリセット

	}
}

// 力を加える
void RigidBodyComponent::ApplyForce(const XMFLOAT3& force) {
	m_Acceleration.x += force.x / m_Mass;
	m_Acceleration.y += force.y / m_Mass;
	m_Acceleration.z += force.z / m_Mass;
}

void RigidBodyComponent::ConstantVelocity(const XMFLOAT3& velocity) {
	m_Velocity.x = velocity.x / m_Mass;
	m_Velocity.y = velocity.y / m_Mass;
	m_Velocity.z = velocity.z / m_Mass;
}

void RigidBodyComponent::ConstantVelocity_X(const float velocity) {
	m_Velocity.x = velocity / m_Mass;
}

void RigidBodyComponent::ConstantVelocity_Y(const float velocity) {
	m_Velocity.y = velocity / m_Mass;
}

void RigidBodyComponent::ConstantVelocity_Z(const float velocity) {
	m_Velocity.z = velocity / m_Mass;
}

void RigidBodyComponent::ReduceVelocity(const XMFLOAT3& velocity) {

	m_Velocity.x *= velocity.x;
	m_Velocity.y *= velocity.y;
	m_Velocity.z *= velocity.z;

	// しきい値を下回ったら止める
	CheckStopVelocity();
}

void RigidBodyComponent::ReduceVelocity_X(const float velocity) {
	m_Velocity.x *= velocity;

	// しきい値を下回ったら止める
	if (fabsf(m_Velocity.x) < STOPVELOCITY) {
		m_Velocity.x = 0.0f;
	}
}

void RigidBodyComponent::ReduceVelocity_Y(const float velocity) {
	m_Velocity.y *= velocity;

	// しきい値を下回ったら止める
	if (fabsf(m_Velocity.y) < STOPVELOCITY) {
		m_Velocity.y = 0.0f;
	}
}

void RigidBodyComponent::ReduceVelocity_Z(const float velocity) {
	m_Velocity.z *= velocity;

	// しきい値を下回ったら止める
	if (fabsf(m_Velocity.z) < STOPVELOCITY) {
		m_Velocity.z = 0.0f;
	}
}

// 自由落下
float RigidBodyComponent::UseGravity(const bool gravityFlag) {

	if (gravityFlag == true) {
		TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

		if(transform == nullptr) {
			return m_Velocity.y;	// TransformComponentがない場合は何もしない
		}

		const float FIXED_DELTATIME = TimeManager::GetFixedDeltaTime();	// 固定フレームレートのデルタタイムを取得

		m_TotalForce.y = -GRAVITY * m_FallMagnification;	// 重力の加速度を設定

		if (m_BeforeGravityFlag == false && gravityFlag == true) {
			m_Velocity.y += (-GRAVITY * m_FirstFallMagnification) * FIXED_DELTATIME;			// 重力の初速を設定
		}

		// 速度を加速度から更新
		m_Velocity.y += m_TotalForce.y * FIXED_DELTATIME;	// 速度を更新、0.016fは1/60秒の固定値

		// 最大落下速度の制限
		if (m_Velocity.y < -m_StopGravity) {	// 重力加速度を一定にする
			m_Velocity.y = -m_StopGravity;
		}

		XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 現在の位置を取得
		// 位置を速度で更新
		pos.y += m_Velocity.y * FIXED_DELTATIME;	// ポジションを更新

		transform->AddPosition(pos);	// 位置を更新
	}
	else {
		m_Velocity.y = 0.0f;	// 地面に着いた状態では速度を０にする
		m_TotalForce.y = 0.0f;	// 加速度もリセット
	}

	m_BeforeGravityFlag = gravityFlag;	// 重力フラグを更新

	// 念のため、デバッグで値を見る
	return m_Velocity.y;
}

// 摩擦
void RigidBodyComponent::ApplyFriction_X() {
	if (m_FallFlag == false) {
		m_Velocity.x -= m_Velocity.x * FRICTION;
	}
}

void RigidBodyComponent::ApplyFriction_Y() {
	if (m_FallFlag == false) {
		m_Velocity.y -= m_Velocity.y * FRICTION;
	}
}

// 空気抵抗
void RigidBodyComponent::ApplyAirRessistance() {
	if (m_FallFlag == true) {
		m_Velocity.x -= m_Velocity.x * AIRRESISTANCE;
		m_Velocity.y -= m_Velocity.y * AIRRESISTANCE;
	}
}

// 反発力
// Ｘ軸の反発力
void RigidBodyComponent::ApplyBounce_X() {
	m_Velocity.x = -m_Velocity.x * RESTITUTION;	// Ⅹ軸に反発させる
}

// Ｙ軸の反発力
void RigidBodyComponent::ApplyBounce_Y() {
	m_Velocity.y = -m_Velocity.y * RESTITUTION;	// Ｙ軸に反発させる

	// しきい値以下なら停止
	if (std::abs(m_Velocity.y) < MIN_VELOCITY_THRESHOLD) {
		m_Velocity.y = 0.0f;
	}
}

// コリジョン
void RigidBodyComponent::ApplyCollision_X(float collision) {
	m_Velocity.x = -m_Velocity.x * collision;	// Ⅹ軸に反発、もしくは止める等
}

void RigidBodyComponent::ApplyCollision_Y(float collision) {
	m_Velocity.y = -m_Velocity.y * collision;	// Ｙ軸に反発、もしくは止める等
}

void RigidBodyComponent::CheckStopVelocity() {
	// しきい値を下回ったら止める
	if (fabsf(m_Velocity.x) < STOPVELOCITY) {
		m_Velocity.x = 0.0f;
	}
	if (fabsf(m_Velocity.y) < STOPVELOCITY) {
		m_Velocity.y = 0.0f;
	}
	if (fabsf(m_Velocity.z) < STOPVELOCITY) {
		m_Velocity.z = 0.0f;
	}
}

void RigidBodyComponent::CheckStopVelocity_X() {
	// しきい値を下回ったら止める
	if (fabsf(m_Velocity.x) < STOPVELOCITY) {
		m_Velocity.x = 0.0f;
	}
}

void RigidBodyComponent::CheckStopVelocity_Y() {
	// しきい値を下回ったら止める
	if (fabsf(m_Velocity.y) < STOPVELOCITY) {
		m_Velocity.y = 0.0f;
	}
}

void RigidBodyComponent::CheckStopVelocity_Z() {
	// しきい値を下回ったら止める
	if (fabsf(m_Velocity.z) < STOPVELOCITY) {
		m_Velocity.z = 0.0f;
	}
}