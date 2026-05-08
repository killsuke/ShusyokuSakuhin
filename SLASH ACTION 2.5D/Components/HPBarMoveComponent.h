#pragma once
#include "Component.h"
#include "Structs/HPParam.h"

class HPBarMoveComponent : public Component
{
private:
	GameObject* m_ReferenceHpObj = nullptr;
	float m_HpScale = 3.3f; // HPの値「１」辺りの縦のサイズ
public:
	HPBarMoveComponent(GameObject& obj);
	~HPBarMoveComponent() = default;

	void Update()override;

	void SetReferenceHPObj(GameObject& obj) { m_ReferenceHpObj = &obj; };
	void SetHpScale(float scale) { m_HpScale = scale; } // HPの値「１」辺りの縦のサイズを設定
};
