#include "RigidBodyComponent.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;

// Transformより、XMFLOAT3の方が良いのでは？
RigidBodyComponent::RigidBodyComponent(GameObject& obj) : m_velocity{ 0.0f,0.0f,0.0f }, m_acceleration{ 0.0f,0.0f,0.0f }, m_mass(1.0f), Component(obj){
	m_sortNum = RIGIDBODY; // ソート番号を設定
	lastTime = std::chrono::high_resolution_clock::now();
}

void RigidBodyComponent::Update() {
	if (m_gravityFlag == true) {
		UseGravity(true);
	}

	auto transform = p_object->GetComponent<TransformComponent>();

	if (transform != nullptr) {
		const float fixedDeltaTime = 1.0f / 60.0f;  // 固定フレームレート（60 FPS相当）

		// 力から加速度を更新
		m_acceleration.x = m_totalForce.x / m_mass; // 合力を質量で割って加速度を計算
		m_acceleration.y = m_totalForce.y / m_mass; // 合力を質量で割って加速度を計算
		m_acceleration.z = m_totalForce.z / m_mass; // 合力を質量で割って加速度を計算

		// 加速度から速度を更新
		m_velocity.x += m_acceleration.x * fixedDeltaTime;
		m_velocity.y += m_acceleration.y * fixedDeltaTime;
		m_velocity.z += m_acceleration.z * fixedDeltaTime;

		Vector3 newPos = {};

		// 速度からTransformの位置を更新
		newPos.x += m_velocity.x * fixedDeltaTime;
		newPos.y += m_velocity.y * fixedDeltaTime;
		newPos.z += m_velocity.z * fixedDeltaTime;

		transform->AddPosition(newPos); // 位置を更新
		//transform->MakeWorldMatrix(); // ワールド行列を更新

		// 毎フレーム加速度をリセット（次のフレームの外力のみ反映）
		m_acceleration = { 0.0f, 0.0f, 0.0f };
		m_totalForce = { 0.0f, 0.0f, 0.0f }; // 合力もリセット

		// 時間の更新
		auto now = std::chrono::high_resolution_clock::now();
		//m_deltaTime = std::chrono::duration<float>(now - lastTime).count();
		lastTime = now;

		// しきい値を下回ったら止める
		CheckStopVelocity();
	}
}

//DirectX::XMFLOAT3& RigidBodyComponent::AcceleratorPosition(DirectX::XMFLOAT3& pos) {
//
//	return pos;
//}

// 力を加える
void RigidBodyComponent::ApplyForce(const Vector3& force) {
	m_acceleration.x += force.x / m_mass;
	m_acceleration.y += force.y / m_mass;
	m_acceleration.z += force.z / m_mass;
}

void RigidBodyComponent::ConstantVelocity(const Vector3& velocity) {
	m_velocity.x = velocity.x / m_mass;
	m_velocity.y = velocity.y / m_mass;
	m_velocity.z = velocity.z / m_mass;
}

void RigidBodyComponent::ConstantVelocity_X(const float velocity) {
	m_velocity.x = velocity / m_mass;
}

void RigidBodyComponent::ConstantVelocity_Y(const float velocity) {
	m_velocity.y = velocity / m_mass;
}

void RigidBodyComponent::ConstantVelocity_Z(const float velocity) {
	m_velocity.z = velocity / m_mass;
}

void RigidBodyComponent::ReduceVelocity(const Vector3& velocity) {

	m_velocity.x *= velocity.x;
	m_velocity.y *= velocity.y;
	m_velocity.z *= velocity.z;

	// しきい値を下回ったら止める
	CheckStopVelocity();
}

void RigidBodyComponent::ReduceVelocity_X(const float velocity) {
	m_velocity.x *= velocity;

	// しきい値を下回ったら止める
	if (fabsf(m_velocity.x) < STOPVELOCITY) {
		m_velocity.x = 0.0f;
	}
}

void RigidBodyComponent::ReduceVelocity_Y(const float velocity) {
	m_velocity.y *= velocity;

	// しきい値を下回ったら止める
	if (fabsf(m_velocity.y) < STOPVELOCITY) {
		m_velocity.y = 0.0f;
	}
}

void RigidBodyComponent::ReduceVelocity_Z(const float velocity) {
	m_velocity.z *= velocity;

	// しきい値を下回ったら止める
	if (fabsf(m_velocity.z) < STOPVELOCITY) {
		m_velocity.z = 0.0f;
	}
}

// 自由落下
float RigidBodyComponent::UseGravity(const bool gravityFlag) {

	if (gravityFlag == true) {
		auto transform = p_object->GetComponent<TransformComponent>();

		m_totalForce.y = -GRAVITY * m_fallMagnification;	// 重力の加速度を設定

		if (m_beforeGravityFlag == false && gravityFlag == true) {
			m_velocity.y += (-GRAVITY * m_firstFallMagnification) * m_deltaTime;			// 重力の初速を設定
		}

		// 速度を加速度から更新
		m_velocity.y += m_totalForce.y * m_deltaTime;	// 速度を更新、0.016fは1/60秒の固定値

		// 最大落下速度の制限
		if (m_velocity.y < -GRAVITY_STOP) {	// 重力加速度を一定にする
			m_velocity.y = -GRAVITY_STOP;
		}

		Vector3 pos = Vector3::Zero;	// 現在の位置を取得
		// 位置を速度で更新
		pos.y += m_velocity.y * m_deltaTime;	// ポジションを更新

		transform->AddPosition(pos);	// 位置を更新
	//	transform->MakeWorldMatrix();	// ワールド行列を更新
	}
	else {
		m_velocity.y = 0.0f;	// 地面に着いた状態では速度を０にする
		m_totalForce.y = 0.0f;	// 加速度もリセット
	}

	m_beforeGravityFlag = gravityFlag;	// 重力フラグを更新

	// 念のため、デバッグで値を見る
	return m_velocity.y;
}

// 摩擦
void RigidBodyComponent::ApplyFriction_X() {
	if (m_fallFlag == false) {
		m_velocity.x -= m_velocity.x * FRICTION;
	}
}

void RigidBodyComponent::ApplyFriction_Y() {
	if (m_fallFlag == false) {
		m_velocity.y -= m_velocity.y * FRICTION;
	}
}

// 空気抵抗
void RigidBodyComponent::ApplyAirRessistance() {
	if (m_fallFlag == true) {
		m_velocity.x -= m_velocity.x * AIRRESISTANCE;
		m_velocity.y -= m_velocity.y * AIRRESISTANCE;
	}
}

// 一旦ここのコードは放置 --------------------------

// 反発力
// Ｘ軸の反発力
void RigidBodyComponent::ApplyBounce_X() {
	m_velocity.x = -m_velocity.x * RESTITUTION;	// Ⅹ軸に反発させる
}

// Ｙ軸の反発力
void RigidBodyComponent::ApplyBounce_Y() {
	m_velocity.y = -m_velocity.y * RESTITUTION;	// Ｙ軸に反発させる

	// しきい値以下なら停止
	if (std::abs(m_velocity.y) < MIN_VELOCITY_THRESHOLD) {
		m_velocity.y = 0.0f;
	}
}
// --------------------------------------------------

// コリジョン
void RigidBodyComponent::ApplyCollision_X(float collision) {
	m_velocity.x = -m_velocity.x * collision;	// Ⅹ軸に反発、もしくは止める等
}

void RigidBodyComponent::ApplyCollision_Y(float collision) {
	m_velocity.y = -m_velocity.y * collision;	// Ｙ軸に反発、もしくは止める等
}

void RigidBodyComponent::CheckStopVelocity() {
	// しきい値を下回ったら止める
	if (fabsf(this->m_velocity.x) < STOPVELOCITY) {
		this->m_velocity.x = 0.0f;
	}
	if (fabsf(this->m_velocity.y) < STOPVELOCITY) {
		this->m_velocity.y = 0.0f;
	}
	if (fabsf(this->m_velocity.z) < STOPVELOCITY) {
		this->m_velocity.z = 0.0f;
	}
}

void RigidBodyComponent::CheckStopVelocity_X() {
	// しきい値を下回ったら止める
	if (fabsf(this->m_velocity.x) < STOPVELOCITY) {
		this->m_velocity.x = 0.0f;
	}
}

void RigidBodyComponent::CheckStopVelocity_Y() {
	// しきい値を下回ったら止める
	if (fabsf(this->m_velocity.y) < STOPVELOCITY) {
		this->m_velocity.y = 0.0f;
	}
}

void RigidBodyComponent::CheckStopVelocity_Z() {
	// しきい値を下回ったら止める
	if (fabsf(this->m_velocity.z) < STOPVELOCITY) {
		this->m_velocity.z = 0.0f;
	}
}

//// 自由落下用の秒数計算
//// 空中にいる間
//void RigidBodyComponent::TimeStart() {
//	if (timeFlag == false) {
//		startTime = std::chrono::high_resolution_clock::now();
//		//		fallFlag = true;
//		timeFlag = true;	// 空中にいる間に一度だけ起動させる
//	}
//}
//
//// 地面に着いてる間
//void RigidBodyComponent::TimeStop() {
//	if (m_velocity.y == 0.0f) {
//		timeFlag = false;
//	}
//}
//
//// 空中にいる時間を返す、自由落下に使う
//float RigidBodyComponent::GetElapsedTime() {
//	//	if (fallFlag == true) {
//	auto currentTime = std::chrono::high_resolution_clock::now();
//	std::chrono::duration<float> deltaTime = currentTime - startTime;
//	elapsedTime = deltaTime.count();
//	//	}
//	return elapsedTime;
//}