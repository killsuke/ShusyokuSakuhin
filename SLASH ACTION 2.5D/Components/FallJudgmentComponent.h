#pragma once
#include "Component.h"
#include <DirectXMath.h>

class FallJudgmentComponent : public Component
{
private:

	DirectX::XMFLOAT3 m_Resurrection = {};

	void ResurrentionProcess();
	void PlayerActiveProcess();

public:
	FallJudgmentComponent(GameObject& obj);
	~FallJudgmentComponent() = default;
	void Update()override;

	void SetResurrection(const DirectX::XMFLOAT3& res) { m_Resurrection = res; };
};