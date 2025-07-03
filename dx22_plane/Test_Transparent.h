#pragma once
#include "Object.h"
#include "RigidBodyComponent.h"

class Test_Transparent : public Object
{
private:
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