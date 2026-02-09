#pragma once
#include "Render.h"
#include <SimpleMath.h>
#include "Helper/HelpMath.h"

class RenderLine2DComponent : public RenderComponent
{
private:
	GameObject* m_StartObj = nullptr;
	GameObject* m_EndObj = nullptr;
	float m_Thickness = 1.0f;

public:
	RenderLine2DComponent(GameObject& obj);
	~RenderLine2DComponent() = default;

	void Update() override;

	void SetStartPosition(const DirectX::SimpleMath::Vector3& startPos);
	void SetEndPosition(const DirectX::SimpleMath::Vector3& endPos);
	void SetStartAndEndPosition(const DirectX::SimpleMath::Vector3& startPos, const DirectX::SimpleMath::Vector3& endPos);
	void SetStartAndEndFollowObject(GameObject* objS, GameObject* objE);
	void SetStartAndEndDrawContainer(const DrawContainer& dcS, const DrawContainer& dcE);
	void SetStartActive(const ActiveState& as) { m_StartObj->SetActiveState(as); };
	void SetEndActive(const ActiveState& as) { m_EndObj->SetActiveState(as); };
	void SetThickness(const float thickness) { m_Thickness = thickness; };

	GameObject* GetStartPoint()const { return m_StartObj; };
	GameObject* GetEndPoint()const { return m_EndObj; };
};