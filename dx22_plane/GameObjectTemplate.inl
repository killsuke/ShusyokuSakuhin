#pragma once

template<typename T2>
// コンポーネントを追加する
T2* GameObject::AddComponent() {
	static_assert(std::is_base_of<Component, T2>::value,
		"型エラー！Compnentクラスを継承していません！");	// プロジェクトをUTF-8に変換しておく

	auto comp = std::make_unique<T2>(*this);	// thisで呼び出した者を取得可能
	T2* ptr = comp.get();	// 一度別で格納してアクセス違反を防ぐ

	Component* p_comp = comp.get();

	ComponentCheck(p_comp);
	components.emplace_back(std::move(comp));

	SortComponents();
	return ptr;
}