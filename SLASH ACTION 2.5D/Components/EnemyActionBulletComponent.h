#pragma once
#include "EnemyActionComponent.h"
#include "Mesh/SquareMesh.h"

class EnemyActionBulletComponent : public EnemyActionComponent
{
private:

	uint64_t m_listenerID_HitEvent_Bullet = 0;

	void FearEvent(const HitEvent& event);
	void ChangeState(const EEnemyState& newState);
	void StateUpdate(const float deltaTime, const DirectX::XMFLOAT3& myPos, const DirectX::XMFLOAT3& playPos, Mesh& mesh);
	void ChangeDirection(const DirectX::XMFLOAT3& myPos, const DirectX::XMFLOAT3& playPos);
	void FiringBullet(const DirectX::XMFLOAT3& myPos);
	void DeadAnimation() override;

public:

	EnemyActionBulletComponent(GameObject& obj);
	~EnemyActionBulletComponent();

	void Init()override;
	void Update()override;

};

