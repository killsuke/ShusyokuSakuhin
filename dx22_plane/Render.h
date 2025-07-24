#pragma once
#include "Component.h"
class Render : public Component
{
private:

public:
	Render() = default;
	~Render() = default;

	void Update() override;

};

