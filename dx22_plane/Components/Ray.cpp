#include "Ray.h"

RayComponent::RayComponent(GameObject& obj) :Component(obj){
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER_ONE_SKIN_ANIMATION"); // ソート番号を設定

}

void RayComponent::Update() {
	// レイの更新処理があればここに記述
}