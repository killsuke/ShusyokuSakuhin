#pragma once
#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "Ray.h"

class Test_Transparent : public GameObject
{
private:
	RayComponent ray;
	DirectX::XMFLOAT4 color = {};
	RigidBodyComponent rigid;
public:
	Test_Transparent(Camera* cam);
	~Test_Transparent();

	void Init();
	void Draw();
	void Update();
	void Uninit();

	DirectX::SimpleMath::Vector3 GetPos() { return m_Position; };

};