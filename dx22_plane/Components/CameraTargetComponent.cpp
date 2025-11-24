#include "CameraTargetComponent.h"

CameraTargetComponent::CameraTargetComponent(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("CAMERA_TARGET"); // ソート番号を設定
	cp = CameraPattern::CAMERA_NONE; // 初期はCHASEカメラ
}

void CameraTargetComponent::Update() {

}