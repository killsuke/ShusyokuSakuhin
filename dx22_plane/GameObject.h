#pragma once
#include <filesystem>
#include "Component.h"
#include <memory>
#include <string>

class Component; // 前方宣言

class GameObject final {	// 変に継承されないようにするためにfinalを付ける
protected:
	std::vector<std::unique_ptr<Component>> components;
	std::vector<GameObject*> children;	// 子オブジェクトを持つことができる
	GameObject* parent = nullptr;	// 親オブジェクトを持つことができる
	std::string tag = "";	// タグを付けて識別する
	std::string name = "";	// オブジェクトの名前
	bool deletefg = false;	// オブジェクトを削除して良いかどうかのフラグ

public:

	GameObject();
	GameObject(const std::string& _name, const std::string& _tag)
		: name(_name), tag(_tag) {
	}; // 名前とタグを指定して初期化
	~GameObject();	// デフォルトデストラクタ

	void Update();

	// セッター
	inline void SetDeleteFg(const bool deletefg) { this->deletefg = deletefg; };
	inline void SetTag(const std::string& tag) { this->tag = tag; };
	inline void SetName(const std::string& name) { this->name = name; };
	inline void SetChild(GameObject* obj) { 
		children.push_back(obj); 
		obj->parent = this;	// 親オブジェクトを設定
	};

	// ゲッター
	inline bool GetDeleteFg()const { return deletefg; };
	inline std::string& GetTag() { return tag; };
	inline std::string& GetName() { return name; };
	inline GameObject* GetParent() { return parent; };

	// コンポーネントのソート番号でソート
	void SortComponents();

	// 装備されているコンポーネントを取得して使用可能にする
	template<typename T1>
	T1* GetComponent() {
		for (auto& component : components) { // ゲームオブジェクト内のコンポーネントをループで見る
			if (auto ptr = dynamic_cast<T1*>(component.get())) {	// ダイナミックキャストでキャスト可能かどうか判定
				return ptr;
			}
		}
		return nullptr; // 指定された型がなかった場合nullptr
	}

	template<typename T2>
	// コンポーネントを追加する
	T2* AddComponent() {
		static_assert(std::is_base_of<Component, T2>::value,
			"型エラー！Compnentクラスを継承していません！");	// プロジェクトをUTF-8に変換しておく

		auto comp = std::make_unique<T2>(*this);	// thisで呼び出した者を取得可能
		T2* ptr = comp.get();	// 一度別で格納してアクセス違反を防ぐ
		
		//auto num = ptr->GetSortNum();

		//if (num == RENDER || num == RENDER_ONE_SKIN_ANIMATION || num == COLLIDER_DEBUG_RENDER) {
		//	// ここで別のコンテナに入れるとか？
		//}
		//else {

			components.emplace_back(std::move(comp));
	//	}
		SortComponents();
		return ptr;
	}
};
