#pragma once
#include "Render.h"
#include "TestBoneData.h"
#include <SimpleMath.h>

class RenderLineComponent : public RenderComponent
{
private:
	GameObject* p_startObj = nullptr;
	GameObject* p_endObj = nullptr;
	float m_thickness = 1.0f;

public:
	RenderLineComponent(GameObject& obj);
	~RenderLineComponent() = default;

	void Update() override;

	void SetStartPosition(const DirectX::SimpleMath::Vector3& startPos);
	void SetEndPosition(const DirectX::SimpleMath::Vector3& endPos);
	void SetStartAndEndPosition(const DirectX::SimpleMath::Vector3& startPos, const DirectX::SimpleMath::Vector3& endPos);
	void SetStartAndEndFollowObject(GameObject* objS, GameObject* objE);
	void SetStartAndEndBone(TestBone* start, TestBone* end);
	void SetStartActive(const ActiveState as) { p_startObj->SetActiveState(as); };
	void SetEndActive(const ActiveState as) { p_endObj->SetActiveState(as); };
	void SetThickness(const float thickness) { m_thickness = thickness; };

	GameObject* GetStartPoint()const { return p_startObj; };
	GameObject* GetEndPoint()const { return p_endObj; };
};