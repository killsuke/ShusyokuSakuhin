#include "GameObject.h"

//今のところ記述内容は無し
// コンストラクタ
GameObject::GameObject() {

}

// デストラクタ
GameObject::~GameObject() {

}

void GameObject::Update() {
	// コンポーネントの更新
	for (auto& component : components) {
		if (component) {
			component->Update();
		}
	}
}

void GameObject::SortComponents(){
	std::sort(components.begin(), components.end(),
		[](const std::unique_ptr<Component>& a, const std::unique_ptr<Component>& b) {
			return a->GetSortNum() < b->GetSortNum();
		});
}