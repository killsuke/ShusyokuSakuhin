#pragma once
#include "Component.h"
#include <DirectXMath.h>

class BulletComponent : public Component
{
private:
	DirectX::XMFLOAT3 m_FiringVector = DirectX::XMFLOAT3();
	DirectX::XMFLOAT3 m_RotateValue = DirectX::XMFLOAT3();
	float m_FiringSpeed = 0.0f;	// 飛ぶ速度
	float m_RimitTime = 2.0f;
	bool m_IsRotateFlag = false;		// 回転するかどうかのフラグ
	bool m_IsDestroyOrStop = true;		// 破壊するか止まるかのフラグ（true:破壊、false:停止）

public:
	BulletComponent(GameObject& obj);
	~BulletComponent() = default;

	void Update();

	void SetFiringVector(DirectX::XMFLOAT3 vector) { m_FiringVector = vector; };
	void SetFiringSpeed(const float speed) { m_FiringSpeed = speed; };
	void SetRimitTime(const float time) { m_RimitTime = time; };
	void SetRotateFlag(const bool flag) { m_IsRotateFlag = flag; };
	void SetRotateValue(const DirectX::XMFLOAT3 rotate) { m_RotateValue = rotate; };
	void SetDestroyOrStopFlag(const bool flag) { m_IsDestroyOrStop = flag; };
};
