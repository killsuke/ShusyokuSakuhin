#pragma once
#include "Component.h"
#include <string>

class TerrainJsonComponent :  public Component
{
private:

public:
	TerrainJsonComponent(GameObject& obj);
	~TerrainJsonComponent() = default;

	void Update() override;	// XVˆ—

	void RoadJsonFile(const std::string& filename);
};
