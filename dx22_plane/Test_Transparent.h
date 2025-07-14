#pragma once
#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "Ray.h"
#include "JumpComponent.h"

class Test_Transparent : public GameObject
{
private:
	RayComponent ray;
	DirectX::XMFLOAT4 color = {};
	RigidBodyComponent rigid;
	JumpComponent* jump = nullptr;
	bool isGround = false; // ’n–Ê‚É‚¢‚é‚©‚Ç‚¤‚©
public:
	Test_Transparent(Camera* cam);
	~Test_Transparent();

	void Init();
	void Draw();
	void Update();
	void Uninit();

	DirectX::SimpleMath::Vector3 GetPos() { return m_Position; };

};