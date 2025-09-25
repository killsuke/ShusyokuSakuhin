#include    "Spring.h"
#include    "Transform.h"
#include    "RigidBodyComponent.h"
#include <iostream>

using namespace DirectX::SimpleMath;

SpringComponent::SpringComponent(GameObject& obj) : Component(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("SPRING"); // ソート番号を設定
}

void SpringComponent::Update()
{

}

void SpringComponent::SpringAction2D()
{
	auto transformP1 = p_object->GetComponent<TransformComponent>();
	auto rigidBodyP1 = p_object->GetComponent<RigidBodyComponent>();

	auto transformP2 = m_springPartner->GetComponent<TransformComponent>();
	auto rigidBodyP2 = m_springPartner->GetComponent<RigidBodyComponent>();

	auto posP1 = transformP1->GetPosition();
	auto posP2 = transformP2->GetPosition();

	auto velocityP1 = rigidBodyP1->GetVelocity();
	auto velocityP2 = rigidBodyP2->GetVelocity();

	// 2Dなのでz座標は無視
	posP1.z = 0.0f;
	posP2.z = 0.0f;
	velocityP1.z = 0.0f;
	velocityP2.z = 0.0f;

	Vector3 delta = posP1 - posP2;
	float distance = delta.Length();

	if (distance < 1e-6f) return;           // ゼロ除算対策

	Vector3 direction = delta / distance;   // 正規化
	// フックの法則
	float springForceMagnitude = (distance - m_restLength) * m_K;

	// 減衰（相対速度に沿った力）
	Vector3 relativeVelocity = velocityP1 - velocityP2;

	float dampingForceMagnitude = m_DAMPING * relativeVelocity.Dot(direction);

	// 合力
	Vector3 force = direction * (springForceMagnitude - dampingForceMagnitude);

	rigidBodyP1->AddForce(-force); // p1に対しては反対方向の力を加える
	rigidBodyP2->AddForce(force);  // p2に対しては正方向の力を加える

	//	std::cout << force.Length() << std::endl;

// 強制停止チェック
	Vector3 diff = posP2 - posP1;
	if (diff.Length() < 1.0f && velocityP1.Length() < 1.0f && force.Length() < 1.0f) {
		rigidBodyP1->SetVelocity(Vector3(0, 0, 0));

		transformP1->SetPosition({ posP2.x,posP2.y,transformP1->GetPosition().z }); // ← ここでスナップ
		m_finSpringAction = true;
		//m_isSpringAction = false;
		//transformP1->SetPosition(transformP2->GetPosition());
	}
}

void SpringComponent::SpringAction3D()
{
	auto transformP1 = p_object->GetComponent<TransformComponent>();
	auto rigidBodyP1 = p_object->GetComponent<RigidBodyComponent>();

	auto transformP2 = m_springPartner->GetComponent<TransformComponent>();
	auto rigidBodyP2 = m_springPartner->GetComponent<RigidBodyComponent>();

	auto posP1 = transformP1->GetPosition();
	auto posP2 = transformP2->GetPosition();

	auto velocityP1 = rigidBodyP1->GetVelocity();
	auto velocityP2 = rigidBodyP2->GetVelocity();

	Vector3 delta = posP1 - posP2;
	float distance = delta.Length();

	if (distance < 1e-6f) return;           // ゼロ除算対策

	Vector3 direction = delta / distance;   // 正規化
	// フックの法則
	float springForceMagnitude = (distance - m_restLength) * m_K;

	// 減衰（相対速度に沿った力）
	Vector3 relativeVelocity = velocityP1 - velocityP2;

	float dampingForceMagnitude = m_DAMPING * relativeVelocity.Dot(direction);

	// 合力
	Vector3 force = direction * (springForceMagnitude - dampingForceMagnitude);

	rigidBodyP1->AddForce(-force); // p1に対しては反対方向の力を加える
	rigidBodyP2->AddForce(force);  // p2に対しては正方向の力を加える
}

// ばね定数をセットする
void SpringComponent::SetK(float k) {
	m_K = k;
}

// ダンピング定数をセットする
void SpringComponent::SetDAMPING(float damping) {
	m_DAMPING = damping;
}

// ばねの初期の長さをセットする
void SpringComponent::Setrestlng(float restLength) {
	m_restLength = restLength;
}

// ダンピング定数を作成する
void SpringComponent::MakeDamping() {
	auto rigid = p_object->GetComponent<RigidBodyComponent>();
	// m_DAMPING = 0.0f; だと、減衰がないのでバネ挙動を続ける
	m_DAMPING = -ComputeCriticalDamping(rigid->GetMass(), m_K);
}

/**
 * @brief 臨界減衰 (ζ=1) となる減衰係数 c を計算する
 *
 * @param mass 質量 m
 * @param springConstant ばね定数 k
 * @return float 減衰係数 c（ζ=1 となる値）
 */

float SpringComponent::ComputeCriticalDamping(float mass, float springConstant)
{
	if (mass <= 0.0f || springConstant <= 0.0f)
		return 0.0f; // 非常値対策（ゼロや負の場合）

	return 2.0f * std::sqrt(mass * springConstant);
}
