#include "SpringComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "Manager/TimeManager.h"
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

	TransformComponent* transformP2 = m_SpringPartner->GetComponent<TransformComponent>();
	RigidBodyComponent* rigidBodyP2 = m_SpringPartner->GetComponent<RigidBodyComponent>();

	if(transformP1 == nullptr || transformP2 == nullptr || rigidBodyP1 == nullptr || rigidBodyP2 == nullptr) {
		return; // TransformComponentまたはRigidBodyComponentがない場合は何もしない
	}

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
	float springForceMagnitude = (distance - m_RestLength) * m_Konstante;

	// 減衰（相対速度に沿った力）
	XMFLOAT3 relativeVelocity = velocityP1 - velocityP2;

	relativeVelocity = relativeVelocity * m_Damping;
	XMVECTOR vec1 = XMLoadFloat3(&relativeVelocity);
	XMVECTOR vec2 = XMLoadFloat3(&direction);

	const float dampingForceMagnitude = XMVectorGetX(XMVector3Dot(vec1,vec2));

	// 合力
	XMFLOAT3 force = direction * (springForceMagnitude - dampingForceMagnitude);

	rigidBodyP1->AddForce(-force); // p1に対しては反対方向の力を加える
	rigidBodyP2->AddForce(force);  // p2に対しては正方向の力を加える


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
		m_FinSpringAction = true;
	}
}

void SpringComponent::SpringAction3D()
{
	TransformComponent* transformP1 = m_Object->GetComponent<TransformComponent>();
	RigidBodyComponent* rigidBodyP1 = m_Object->GetComponent<RigidBodyComponent>();

	TransformComponent* transformP2 = m_SpringPartner->GetComponent<TransformComponent>();
	RigidBodyComponent* rigidBodyP2 = m_SpringPartner->GetComponent<RigidBodyComponent>();

	if(transformP1 == nullptr || transformP2 == nullptr || rigidBodyP1 == nullptr || rigidBodyP2 == nullptr) {
		return; // TransformComponentやRigidBodyComponentがない場合は何もしない
	}

	XMFLOAT3 posP1 = transformP1->GetPosition();
	XMFLOAT3 posP2 = transformP2->GetPosition();

	XMFLOAT3 velocityP1 = rigidBodyP1->GetVelocity();
	XMFLOAT3 velocityP2 = rigidBodyP2->GetVelocity();

	XMFLOAT3 delta = posP1 - posP2;

	const XMVECTOR targetPosVec = XMLoadFloat3(&delta);
	const float distance = XMVectorGetX(XMVector3Length(targetPosVec));

	if (distance < 1e-6f) return;           // ゼロ除算対策

	XMFLOAT3 direction = delta / distance;   // 正規化
	// フックの法則
	float springForceMagnitude = (distance - m_RestLength) * m_Konstante;

	// 減衰（相対速度に沿った力）
	XMFLOAT3 relativeVelocity = velocityP1 - velocityP2;

	relativeVelocity = relativeVelocity * m_Damping;

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
	TransformComponent* transformP2 = m_SpringPartner->GetComponent<TransformComponent>();

	if(transformP1 == nullptr || transformP2 == nullptr) {
		return; // TransformComponentがない場合は何もしない
	}

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

	m_Konstante = 15.0f;
	m_Damping = 20 * sqrt(0.05f);


	// フックの法則
	const float springForceMagnitude = distance - m_RestLength;
	const XMFLOAT3 springOffsest = -direction * (springForceMagnitude * m_Konstante);

	const float deltaTime = TimeManager::GetFixedDeltaTime();

	m_SpringVelocity += springOffsest * deltaTime;
	m_SpringVelocity *= (1.0f - m_Damping * deltaTime);

	XMFLOAT3 newPos{

		 m_SpringVelocity.x * deltaTime,
		 m_SpringVelocity.y * deltaTime,
		 m_SpringVelocity.z * deltaTime
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
		m_FinSpringAction = true;
		m_PreviousDistance = 0.0f;
		return;
	}

	m_PreviousDistance = distance2;
}

// ばね定数をセットする
void SpringComponent::SetK(float k) {
	m_Konstante = k;
}

// ダンピング定数をセットする
void SpringComponent::SetDAMPING(float damping) {
	m_Damping = damping;
}

// ばねの初期の長さをセットする
void SpringComponent::Setrestlng(float restLength) {
	m_RestLength = restLength;
}

// ダンピング定数を作成する
void SpringComponent::MakeDamping() {
	RigidBodyComponent* rigid = m_Object->GetComponent<RigidBodyComponent>();

	if (rigid != nullptr) {

		// m_DAMPING = 0.0f; だと、減衰がないのでバネ挙動を続ける
		m_Damping = -ComputeCriticalDamping(rigid->GetMass(), m_Konstante);
	}
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
