#include "GameObject.h"

//今のところ記述内容は無し
// コンストラクタ
GameObject::GameObject(Camera* cam) :m_Camera(cam) {

}

// デストラクタ
GameObject::~GameObject() {

}

//void GameObject::Update() {
//	// コンポーネントの更新
//	for (auto& component : components) {
//		if (component) {
//			component->Update();
//		}
//	}
//}