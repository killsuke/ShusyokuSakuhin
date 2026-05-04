#pragma once
#include "Component.h"
#include "EnemyDeathEventComponent.h"
#include <DirectXMath.h>

class CutObjectActionComponent :  public Component
{
private:
	uint32_t m_CutObj1ID = 0;
	uint32_t m_CutObj2ID = 0;
	float m_RecordTime = 0.0f;

	bool m_IsFirstAction = false;
	bool m_IsFirstCamPos = false;
	DirectX::XMFLOAT3 m_CutObj1Pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_CutObj2Pos = { 0.0f,0.0f,0.0f };

	DirectX::XMVECTOR m_ShakeVector = { 0.0f,0.0f,0.0f,0.0f };	// ランダムな方向に揺らすために使う
	DirectX::XMVECTOR m_PrevShakeOffset = { 0.0f,0.0f,0.0f,0.0f };	// ランダムな方向に揺らすために使う

	GameObject* m_Crack1 = nullptr;
	GameObject* m_Crack2 = nullptr;

	uint32_t m_Crack1ID = 0;
	uint32_t m_Crack2ID = 0;

	std::vector<GameObject*> m_Debris;	// 飛び出した破片
	EnemyDeathEventState m_DeathState = EnemyDeathEventState::STICKY;

	void ImmediateStartProcess();
	void ImmediateEndProcess();

	void ScreenClashStart();
	void CutObjsMove();
	void CreateCracksAndDebris();
	void ShakeAndClash();
	void FollowCamera();
	void FallStart();
	void FallProcess();
	void FallEnd();
	void DummyFunc() {};

public:

	CutObjectActionComponent(GameObject& obj);
	~CutObjectActionComponent() = default;
	void Update() override;

	void SetCutObjs(const uint32_t& obj1ID, const uint32_t& obj2ID) {
		m_CutObj1ID = obj1ID;
		m_CutObj2ID = obj2ID;
	};

	void SetDeathState(const EnemyDeathEventState& state) {
		m_DeathState = state;
	};
};
