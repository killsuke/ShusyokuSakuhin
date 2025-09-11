#pragma once
#include <filesystem>
#include "Component.h"
#include <memory>
#include <string>

enum class ActiveState {
	ACTIVE,
	UPDATE_STOP,
	DRAW_STOP,
	ALL_STOP,
	MAX
};

enum class DrawContainer {
	Default,
	AbsFfont,
	UI,

	Max
};

class Component; // 前方宣言

class GameObject final {	// 変に継承されないようにするためにfinalを付ける
private:
	std::vector<std::unique_ptr<Component>> components;
	std::vector<std::unique_ptr<Component>> renderComponents;
	std::vector<GameObject*> children;	// 子オブジェクトを持つことができる
	GameObject* parent = nullptr;	// 親オブジェクトを持つことができる
	std::string tag = "";	// タグを付けて識別する
	std::string name = "";	// オブジェクトの名前
	bool deletefg = false;	// オブジェクトを削除して良いかどうかのフラグ
	bool drawContainerChangeFlag = false;	// コンテナを入れ替える
	ActiveState activeState = ActiveState::ACTIVE;
	DrawContainer drawContainer = DrawContainer::Default;
	DrawContainer hopeDrawContainer = DrawContainer::Default;

public:

	//GameObject();
	GameObject(const std::string& _name, const std::string& _tag)
		: name(_name), tag(_tag) {
	}; // 名前とタグを指定して初期化
	~GameObject();	// デフォルトデストラクタ

	void Update();

	void Draw();

	// セッター
	inline void SetDeleteFg(const bool deletefg) { this->deletefg = deletefg; };
	inline void SetTag(const std::string& tag) { this->tag = tag; };
	inline void SetName(const std::string& name) { this->name = name; };
	inline void SetChild(GameObject* obj) { 
		children.push_back(obj); 
		obj->parent = this;	// 親オブジェクトを設定
	};
	inline void SetActiveState(ActiveState as) { activeState = as; };
	inline void SetDrawContainer(DrawContainer dc) { drawContainer = dc; };
	inline void SetDrawContainerChangeFlag(DrawContainer dc, bool dccFlag) { 
		hopeDrawContainer = dc; 
		drawContainerChangeFlag = dccFlag;
	};

	// ゲッター
	inline bool GetDeleteFg()const { return deletefg; };
	inline std::string& GetTag() { return tag; };
	inline std::string& GetName() { return name; };
	inline GameObject* GetParent() { return parent; };
	inline std::vector<GameObject*>& GetChildren() { return children; };
	inline ActiveState GetActiveState()const { return activeState; };
	inline DrawContainer GetDrawContainer()const { return drawContainer; };
	inline DrawContainer GetHopeDrawContainer()const { return hopeDrawContainer; };
	inline bool GetDrawContainerChangeFlag()const { return drawContainerChangeFlag; };

	// コンポーネントのソート番号でソート
	void SortComponents();

	bool ComponentCheck(Component* comp);

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

	// コンポーネントを追加する
	// 実体はinlファイルに記述して、インクルードによる循環参照を防ぐ（保険程度）
	template<typename T2>
	T2* AddComponent();
};

#include "GameObjectTemplate.inl"