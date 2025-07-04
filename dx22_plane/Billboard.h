#pragma once
#include "GameObject.h"
class Billboard : public GameObject
{
private:
	DirectX::SimpleMath::Vector3 carPos{};
	DirectX::SimpleMath::Vector3 carVec{};

	float splitX = 0.0f;	// X軸分割位置
	float splitY = 0.0f;	// Y軸分割位置
	float numU = 0.0f;		// X軸分割数
	float numV = 0.0f;		// Y軸分割数

public:

	Billboard(Camera* cam);	// コンストラクタ
	~Billboard();	// デストラクタ

	void Init();
	void Update();
	void Draw();
	void Uninit();

};

