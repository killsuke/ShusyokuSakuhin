#pragma once
class Component
{
protected:
	Component() = default;
public:
	virtual ~Component() = default;
	virtual void Update() = 0;
};

