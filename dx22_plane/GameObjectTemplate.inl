#pragma once

template<typename T3>
// コンポーネントを追加する
T3* GameObject::AddComponent() {
	static_assert(std::is_base_of<Component, T3>::value,
		"型エラー！Compnentクラスを継承していません！");	// プロジェクトをUTF-8に変換しておく

	auto comp = std::make_unique<T3>(*this);	// thisで呼び出した者を取得可能
	T3* ptr = comp.get();	// 一度別で格納してアクセス違反を防ぐ

	Component* p_comp = comp.get();

	// 型IDで登録
	m_ComponentMap[typeid(T3)] = p_comp;

	bool renderFlag = ComponentCheck(p_comp);

	if (renderFlag == true) {
		m_RenderComponents.emplace_back(std::move(comp));
	}
	else {
		m_Components.emplace_back(std::move(comp));
	}

	SortComponents();
	return ptr;
}