#include    "SpringComponent.h"
#include    "TransformComponent.h"
#include    "RigidBodyComponent.h"
#include <iostream>

using namespace DirectX;

SpringComponent::SpringComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("SPRING"); // ソート番号を設定
}

void SpringComponent::Update()
{

}

void SpringComponent::SpringAction2D()
{
	TransformComponent* transformP1 = m_Object->GetComponent<TransformComponent>();
	RigidBodyComponent* rigidBodyP1 = m_Object->GetComponent<RigidBodyComponent>();

	TransformComponent* transformP2 = m_springPartner->GetComponent<TransformComponent>();
	RigidBodyComponent* rigidBodyP2 = m_springPartner->GetComponent<RigidBodyComponent>();

	XMFLOAT3 posP1 = transformP1->GetPosition();
	XMFLOAT3 posP2 = transformP2->GetPosition();

	XMFLOAT3 velocityP1 = rigidBodyP1->GetVelocity();
	XMFLOAT3 velocityP2 = rigidBodyP2->GetVelocity();

	// 2Dなのでz座標は無視
	posP1.z = 0.0f;
	posP2.z = 0.0f;
	velocityP1.z = 0.0f;
	velocityP2.z = 0.0f;

	XMFLOAT3 delta = posP1 - posP2;

	const XMVECTOR targetPosVec = XMLoadFloat3(&delta);
	const float distance = XMVectorGetX(XMVector3Length(targetPosVec));

	if (distance < 1e-6f) return;           // ゼロ除算対策

	XMFLOAT3 direction = delta / distance;   // 正規化
	// フックの法則
	float springForceMagnitude = (distance - m_restLength) * m_K;

	// 減衰（相対速度に沿った力）
	XMFLOAT3 relativeVelocity = velocityP1 - velocityP2;

	relativeVelocity = relativeVelocity * m_DAMPING;
	XMVECTOR vec1 = XMLoadFloat3(&relativeVelocity);
	XMVECTOR vec2 = XMLoadFloat3(&direction);

	const float dampingForceMagnitude = XMVectorGetX(XMVector3Dot(vec1,vec2));

	// 合力
	XMFLOAT3 force = direction * (springForceMagnitude - dampingForceMagnitude);

	rigidBodyP1->AddForce(-force); // p1に対しては反対方向の力を加える
	rigidBodyP2->AddForce(force);  // p2に対しては正方向の力を加える

	//	std::cout << force.Length() << std::endl;

// 強制停止チェック
	const XMFLOAT3 diff = posP2 - posP1;
	const XMVECTOR diffVec = XMLoadFloat3(&diff);
	const float diffLength = XMVectorGetX(XMVector3Length(diffVec));

	const XMVECTOR velcityVec1 = XMLoadFloat3(&velocityP1);
	const float velocityLength1 = XMVectorGetX(XMVector3Length(velcityVec1));

	const XMVECTOR forceVec = XMLoadFloat3(&force);
	const float forceLength = XMVectorGetX(XMVector3Length(forceVec));

	if (diffLength < 1.0f && velocityLength1 < 1.0f && forceLength < 1.0f) {
		rigidBodyP1->SetVelocity(XMFLOAT3(0, 0, 0));

		transformP1->SetPosition({ posP2.x,posP2.y,transformP1->GetPosition().z }); // ← ここでスナップ
		m_finSpringAction = true;
		//m_isSpringAction = false;
		//transformP1->SetPosition(transformP2->GetPosition());
	}
}

void SpringComponent::SpringAction3D()
{
	auto transformP1 = m_Object->GetComponent<TransformComponent>();
	auto rigidBodyP1 = m_Object->GetComponent<RigidBodyComponent>();

	auto transformP2 = m_springPartner->GetComponent<TransformComponent>();
	auto rigidBodyP2 = m_springPartner->GetComponent<RigidBodyComponent>();

	auto posP1 = transformP1->GetPosition();
	auto posP2 = transformP2->GetPosition();

	auto velocityP1 = rigidBodyP1->GetVelocity();
	auto velocityP2 = rigidBodyP2->GetVelocity();

	XMFLOAT3 delta = posP1 - posP2;

	const XMVECTOR targetPosVec = XMLoadFloat3(&delta);
	const float distance = XMVectorGetX(XMVector3Length(targetPosVec));

	if (distance < 1e-6f) return;           // ゼロ除算対策

	XMFLOAT3 direction = delta / distance;   // 正規化
	// フックの法則
	float springForceMagnitude = (distance - m_restLength) * m_K;

	// 減衰（相対速度に沿った力）
	XMFLOAT3 relativeVelocity = velocityP1 - velocityP2;

	relativeVelocity = relativeVelocity * m_DAMPING;

	XMVECTOR vec1 = XMLoadFloat3(&relativeVelocity);
	XMVECTOR vec2 = XMLoadFloat3(&direction);

	const float dampingForceMagnitude = XMVectorGetX(XMVector3Dot(vec1, vec2));

	// 合力
	XMFLOAT3 force = direction * (springForceMagnitude - dampingForceMagnitude);

	rigidBodyP1->AddForce(-force); // p1に対しては反対方向の力を加える
	rigidBodyP2->AddForce(force);  // p2に対しては正方向の力を加える
}

void SpringComponent::SpringActionTransform() {

	TransformComponent* transformP1 = m_Object->GetComponent<TransformComponent>();
	TransformComponent* transformP2 = m_springPartner->GetComponent<TransformComponent>();

	XMFLOAT3 posP1 = transformP1->GetPosition();
	XMFLOAT3 posP2 = transformP2->GetPosition();

	// 2Dなのでz座標は無視
	posP1.z = 0.0f;
	posP2.z = 0.0f;

	XMFLOAT3 delta = posP1 - posP2;
	const XMVECTOR targetPosVec = XMLoadFloat3(&delta);
	const float distance = XMVectorGetX(XMVector3Length(targetPosVec));

	if (distance < 1e-6f) return;           // ゼロ除算対策

	const XMFLOAT3 direction = delta / distance;   // 正規化

	m_K = 15.0f;
	m_DAMPING = 20 * sqrt(0.05f);


	// フックの法則
	const float springForceMagnitude = distance - m_restLength;
	const XMFLOAT3 springOffsest = -direction * (springForceMagnitude * m_K);


//	XMFLOAT3 velAndOffset = m_SpringVelocity + XMFLOAT3(springOffsest.x, springOffsest.y, springOffsest.z);

	float dt = 0.016f; // フレーム時間
	m_SpringVelocity += springOffsest * dt;
	m_SpringVelocity *= (1.0f - m_DAMPING * dt);

	XMFLOAT3 newPos{

		 m_SpringVelocity.x * 0.016f,
		 m_SpringVelocity.y * 0.016f,
		 m_SpringVelocity.z * 0.016f
	};

	XMVECTOR springVec = XMLoadFloat3(&m_SpringVelocity);
	XMVECTOR len = XMVector3Length(springVec);
	const float length = XMVectorGetX(len);

	transformP1->AddPosition(newPos);

	float prevDistance = m_PreviousDistance;

	const XMFLOAT3 delta2 = transformP1->GetPosition() - posP2;

	const XMVECTOR targetPosVec2 = XMLoadFloat3(&delta2);
	const float distance2 = XMVectorGetX(XMVector3Length(targetPosVec2));

	if (distance2 > prevDistance && prevDistance != 0.0f) {
		transformP1->SetPosition({ posP2.x, posP2.y, transformP1->GetPosition().z });
		m_SpringVelocity = XMFLOAT3(0, 0, 0);
		m_finSpringAction = true;
		m_PreviousDistance = 0.0f;
		return;
	}

	m_PreviousDistance = distance2;
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
	auto rigid = m_Object->GetComponent<RigidBodyComponent>();
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
