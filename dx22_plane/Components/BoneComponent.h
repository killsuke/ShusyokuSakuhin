#pragma once
#include "Component.h"
#include "Structs/BoneData.h"
class BoneComponent : public Component
{
private :
	Bone m_bone;

public:
	BoneComponent(GameObject &obj);
	~BoneComponent() {};

	void Update() override;
};
