#pragma once
#include <simplemath.h>

class RayComponent
{
private:
	DirectX::XMVECTOR m_Origin = {}; // レイの始点
	DirectX::XMVECTOR m_Direction = {}; // レイの方向

public:
	RayComponent() = default; // デフォルトコンストラクタ
	~RayComponent() = default; // デフォルトデストラクタ

	inline void SetOriginPosition(const DirectX::XMVECTOR& origin) { m_Origin = origin; };
	inline DirectX::XMVECTOR GetOriginPosition() const { return m_Origin; };

	inline void SetDirection(const DirectX::XMVECTOR& direction) { m_Direction = direction; };
	inline DirectX::XMVECTOR GetDirection() const { return m_Direction; };
};

