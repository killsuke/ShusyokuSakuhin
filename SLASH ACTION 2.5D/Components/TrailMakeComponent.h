#pragma once
#include "Component.h"
#include "TrailRenderComponent.h"

class TrailRenderComponent;

class TrailMakeComponent : public Component
{
private:
	TrailRenderComponent* m_TrailRenderComponent = nullptr;

public:
	
	TrailMakeComponent(GameObject& obj);
	~TrailMakeComponent() = default;

	void Update()override;

	inline void SetTrailRenderComponent(TrailRenderComponent* trailRenderComp) {
		m_TrailRenderComponent = trailRenderComp;
	};
};