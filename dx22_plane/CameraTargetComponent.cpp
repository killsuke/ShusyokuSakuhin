#include "CameraTargetComponent.h"

CameraTargetComponent::CameraTargetComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = CAMERA_TARGET;
	cp = CAMERA_NONE; // ‰Šú‚ÍCHASEƒJƒƒ‰
}

void CameraTargetComponent::Update() {

}