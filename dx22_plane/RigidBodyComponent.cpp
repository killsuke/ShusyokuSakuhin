#include "RigidBodyComponent.h"

// Transformより、XMFLOAT3の方が良いのでは？
RigidBodyComponent::RigidBodyComponent(float m) : velocity{ 0.0f,0.0f,0.0f }, acceleration{ 0.0f,0.0f,0.0f }, mass(m) {

}


void RigidBodyComponent::Update() {

}

DirectX::XMFLOAT3& RigidBodyComponent::AcceleratorPosition(DirectX::XMFLOAT3& pos) {
	const float fixedDeltaTime = 1.0f / 60.0f;  // 固定フレームレート（60 FPS相当）

	// 加速度から速度を更新
	velocity.x += acceleration.x * fixedDeltaTime;
	velocity.y += acceleration.y * fixedDeltaTime;
	velocity.z += acceleration.z * fixedDeltaTime;

	// 速度からTransformの位置を更新
	pos.x += velocity.x * fixedDeltaTime;
	pos.y += velocity.y * fixedDeltaTime;
	pos.z += velocity.z * fixedDeltaTime;

	// 毎フレーム加速度をリセット（次のフレームの外力のみ反映）
	acceleration = { 0.0f, 0.0f, 0.0f };

	return pos;
}

// 力を加える
void RigidBodyComponent::AppryForce(const DirectX::XMFLOAT3& force) {
	acceleration.x += force.x / mass;
	acceleration.y += force.y / mass;
	acceleration.z += force.z / mass;
}

void RigidBodyComponent::ConstantVelocity(const DirectX::XMFLOAT3& velocity) {
	this->velocity.x = velocity.x / mass;
	this->velocity.y = velocity.y / mass;
	this->velocity.z = velocity.z / mass;
}

void RigidBodyComponent::ConstantVelocity_X(const float velocity) {
	this->velocity.x = velocity / mass;	
}

void RigidBodyComponent::ConstantVelocity_Y(const float velocity) {
	this->velocity.y = velocity / mass;
}

void RigidBodyComponent::ConstantVelocity_Z(const float velocity) {
	this->velocity.z = velocity / mass;
}

void RigidBodyComponent::ReduceVelocity(const DirectX::XMFLOAT3& velocity) {

	this->velocity.x *= velocity.x;
	this->velocity.y *= velocity.y;
	this->velocity.z *= velocity.z;

	// しきい値を下回ったら止める
	if (fabsf(this->velocity.x) < STOPVELOCITY) {
		this->velocity.x = 0.0f;
	}
	if (fabsf(this->velocity.y) < STOPVELOCITY) {
		this->velocity.y = 0.0f;
	}
	if (fabsf(this->velocity.z) < STOPVELOCITY) {
		this->velocity.z = 0.0f;
	}
}

void RigidBodyComponent::ReduceVelocity_X(const float velocity) {
	this->velocity.x *= velocity;

	// しきい値を下回ったら止める
	if (fabsf(this->velocity.x) < STOPVELOCITY) {
		this->velocity.x = 0.0f;
	}
}

void RigidBodyComponent::ReduceVelocity_Y(const float velocity) {
	this->velocity.y *= velocity;

	// しきい値を下回ったら止める
	if (fabsf(this->velocity.y) < STOPVELOCITY) {
		this->velocity.y = 0.0f;
	}
}

void RigidBodyComponent::ReduceVelocity_Z(const float velocity) {
	this->velocity.z *= velocity;

	// しきい値を下回ったら止める
	if (fabsf(this->velocity.z) < STOPVELOCITY) {
		this->velocity.z = 0.0f;
	}
}

// 自由落下
float RigidBodyComponent::UseGravity(DirectX::XMFLOAT3& pos, const bool gravityFlag) {
	float fallvelocity = 0.0f;
	if (gravityFlag == true) {	// 浮いている状態
		TimeStart();

		float time = GetElapsedTime() + 0.5f;
		// 自由落下で位置情報を更新
		fallvelocity = (50.0f * GRAVITY * std::powf(time, 2.0f));	// 取り合えず今は0.5f（変えて試すのも面白そう）
		if (fallvelocity > GRAVITY_STOP) {	// 重力加速度を一定にする
			fallvelocity = GRAVITY_STOP;
		}

		// ポジションに進む分を引いてやる
		acceleration.y = pos.y - fallvelocity;
		if (acceleration.y > 0.0f) {	// 重力の逆ベクトルの修正
			acceleration.y *= -1.0f;
		}
	}
	else {	// 地面に着いた状態
		velocity.y = 0.0f;	// 仮置き、バウンスとかさせずに止めるため
		elapsedTime = 0.0f;
		TimeStop();
	}
	// 念のため、デバッグで値を見る
	return fallvelocity;
}

// 摩擦
void RigidBodyComponent::ApplyFriction_X() {
	if (fallFlag == false) {
		velocity.x -= velocity.x * FRICTION;
	}
}

void RigidBodyComponent::ApplyFriction_Y() {
	if (fallFlag == false) {
		velocity.y -= velocity.y * FRICTION;
	}
}

// 空気抵抗
void RigidBodyComponent::ApplyAirRessistance() {
	if (fallFlag == true) {
		velocity.x -= velocity.x * AIRRESISTANCE;
		velocity.y -= velocity.y * AIRRESISTANCE;
	}
}

// 一旦ここのコードは放置 --------------------------

// 反発力
// Ｘ軸の反発力
void RigidBodyComponent::ApplyBounce_X() {
	velocity.x = -velocity.x * RESTITUTION;	// Ⅹ軸に反発させる
}

// Ｙ軸の反発力
void RigidBodyComponent::ApplyBounce_Y() {
	velocity.y = -velocity.y * RESTITUTION;	// Ｙ軸に反発させる

	// しきい値以下なら停止
	if (std::abs(velocity.y) < MIN_VELOCITY_THRESHOLD) {
		velocity.y = 0.0f;
	}
}
// --------------------------------------------------

// コリジョン
void RigidBodyComponent::ApplyCollision_X(float collision) {
	velocity.x = -velocity.x * collision;	// Ⅹ軸に反発、もしくは止める等
}

void RigidBodyComponent::ApplyCollision_Y(float collision) {
	velocity.y = -velocity.y * collision;	// Ｙ軸に反発、もしくは止める等
}

// 自由落下用の秒数計算
// 空中にいる間
void RigidBodyComponent::TimeStart() {
	if (timeFlag == false) {
		startTime = std::chrono::high_resolution_clock::now();
		//		fallFlag = true;
		timeFlag = true;	// 空中にいる間に一度だけ起動させる
	}
}

// 地面に着いてる間
void RigidBodyComponent::TimeStop() {
	if (velocity.y == 0.0f) {
		timeFlag = false;
	}
}

// 空中にいる時間を返す、自由落下に使う
float RigidBodyComponent::GetElapsedTime() {
	//	if (fallFlag == true) {
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - startTime;
	elapsedTime = deltaTime.count();
	//	}
	return elapsedTime;
}