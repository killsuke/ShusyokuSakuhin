#include "GameObject/GameObject.h"
#include "Components/Render.h"
#include "System/DirectXRender.h"
#include <iostream>

GameObject::GameObject(const std::string& _name, const uint32_t& id) : m_Name(_name), m_InstanceID(id) {
};

void GameObject::Update() {
	// 更新処理を消すか？
	if (m_ActiveState == ActiveState::UPDATE_STOP || m_ActiveState == ActiveState::ALL_STOP)
		return;

	// コンポーネントの更新
	for (auto& component : m_Components) {
		if (component->GetActiveFlag() == true) {
			component->Update();
		}
	}

	// 子オブジェクトの更新
	if (!m_Children.empty()) {
		for (auto& child : m_Children) {
			child->Update();
		}
	}
}

void GameObject::Draw() {
	// 描画処理を消すか？
	if (m_ActiveState == ActiveState::DRAW_STOP || m_ActiveState == ActiveState::ALL_STOP)
		return;

	// 描画用コンポーネントの更新
	for (auto& renderComp : m_RenderComponents) {
		if (renderComp->GetActiveFlag() == true) {
			renderComp->Update();
		}
	}

	// 子オブジェクトの描画
	if (!m_Children.empty()) {
		for (auto& child : m_Children) {
			if (m_ChildAbsFrontFlag == true) {
				
				const bool currentDepthEnable = DirectXRender::GetIsDepthEnable();
				DirectXRender::SetDepthEnable(false);
				child->Draw();
				DirectXRender::SetDepthEnable(currentDepthEnable);
			}
			else {
				child->Draw();
			}
		}
	}
}

void GameObject::SortComponents() {
	std::sort(m_Components.begin(), m_Components.end(),	// レンダー関係以外のコンポーネントコンテナをソート
		[](const std::unique_ptr<Component>& a, const std::unique_ptr<Component>& b) {
			return a->GetSortNum() < b->GetSortNum();
		});

	std::sort(m_RenderComponents.begin(), m_RenderComponents.end(),	// レンダー関係のコンポーネントコンテナをソート
		[](const std::unique_ptr<Component>& a, const std::unique_ptr<Component>& b) {
			return a->GetSortNum() < b->GetSortNum();
		});
}

bool GameObject::ComponentCheck(Component* comp) {
	// 描画関係の機能と分けるため、ここで判断
	if (auto renderComp = dynamic_cast<RenderComponent*>(comp)) {
		return true;
	}

	return false;
}