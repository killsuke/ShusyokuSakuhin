#pragma once
#include "Component.h"
#include "HPParam.h"

class HPBarMoveComponent : public Component
{
private:
	GameObject* referenceHpObj = nullptr;
	float hpScale = 5.0f; // HPの値「１」辺りの縦のサイズ
public:
	HPBarMoveComponent(GameObject& obj);
	~HPBarMoveComponent() = default;

	void Update()override;

	inline void SetReferenceHPObj(GameObject& obj) { referenceHpObj = &obj; };
	inline void SetHpScale(float scale) { hpScale = scale; } // HPの値「１」辺りの縦のサイズを設定
};
