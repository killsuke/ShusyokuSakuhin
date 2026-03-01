#pragma once
#include "RenderComponent.h"

class RenderHpComponent : public RenderComponent
{
private:

	GameObject* referenceHpObj = nullptr;
	float hpScale = 3.3f; // HPの値「１」辺りの縦のサイズ

public:
	RenderHpComponent(GameObject& obj);
	~RenderHpComponent() = default;

	void Update()override;

	inline void SetReferenceHPObj(GameObject& obj) { referenceHpObj = &obj; };
	inline void SetHpScale(float scale) { hpScale = scale; } // HPの値「１」辺りの縦のサイズを設定
};
