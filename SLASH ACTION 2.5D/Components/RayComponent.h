#pragma once
#include <DirectXMath.h>
#include "Component.h"

class RayComponent: public Component
{
private:
	DirectX::XMVECTOR m_Origin = {}; // レイの始点
	DirectX::XMVECTOR m_Direction = {}; // レイの方向

public:

	RayComponent(GameObject& obj);
	~RayComponent() = default; // デフォルトデストラクタ

	void Update()override;

	inline void SetOriginPosition(const DirectX::XMVECTOR& origin) { m_Origin = origin; };
	inline DirectX::XMVECTOR GetOriginPosition() const { return m_Origin; };

	inline void SetDirection(const DirectX::XMVECTOR& direction) { m_Direction = direction; };
	inline DirectX::XMVECTOR GetDirection() const { return m_Direction; };
};

