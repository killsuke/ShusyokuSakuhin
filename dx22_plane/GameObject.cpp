#include "GameObject.h"
#include "Render.h"
#include <iostream>

//今のところ記述内容は無し
// コンストラクタ
//GameObject::GameObject() {
//
//}

// デストラクタ
GameObject::~GameObject() {

}

void GameObject::Update() {

	if (activeState == ActiveState::UPDATE_STOP || activeState == ActiveState::ALL_STOP)
		return;

	// コンポーネントの更新
	for (auto& component : components) {
		if (component->GetActiveFlag() == true) {
			component->Update();
		}
	}

	// 子オブジェクトの更新
	if (!children.empty()) {
		for (auto& child : children) {
			child->Update();
		}
	}
}

void GameObject::Draw() {
	if (activeState == ActiveState::DRAW_STOP || activeState == ActiveState::ALL_STOP)
		return;

	// 描画用コンポーネントの更新
	for (auto& renderComp : renderComponents) {
		if (renderComp->GetActiveFlag() == true) {
			renderComp->Update();
		}
	}

	// 子オブジェクトの描画
	if (!children.empty()) {
		for (auto& child : children) {
			child->Draw();
		}
	}
}

void GameObject::SortComponents() {
	std::sort(components.begin(), components.end(),
		[](const std::unique_ptr<Component>& a, const std::unique_ptr<Component>& b) {
			return a->GetSortNum() < b->GetSortNum();
		});

	std::sort(renderComponents.begin(), renderComponents.end(),
		[](const std::unique_ptr<Component>& a, const std::unique_ptr<Component>& b) {
			return a->GetSortNum() < b->GetSortNum();
		});
}

bool GameObject::ComponentCheck(Component* comp) {
	// 描画関係の機能と分けるため、ここで判断
	if (auto renderComp = dynamic_cast<RenderComponent*>(comp)) {

		//std::cout << this->name << "：" << "RenderComponent です" << std::endl;
		return true;
	}

	return false;
}