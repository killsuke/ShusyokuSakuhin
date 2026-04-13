#pragma once
#include "RenderComponent.h"

class RenderHpComponent : public RenderComponent
{
private:

	GameObject* m_ReferenceHpObj = nullptr;
	float m_HpScale = 3.3f; // HPの値「１」辺りの縦のサイズ

public:
	RenderHpComponent(GameObject& obj);
	~RenderHpComponent() = default;

	void Update()override;

	void SetReferenceHPObj(GameObject& obj) { m_ReferenceHpObj = &obj; };
	void SetHpScale(float scale) { m_HpScale = scale; } // HPの値「１」辺りの縦のサイズを設定
	void ResetReferenceHPObj() { m_ReferenceHpObj = nullptr; }; // 参照するHPオブジェクトをリセット
};
