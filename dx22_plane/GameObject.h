#pragma once
#include <filesystem>
#include "Components/Component.h"
#include <memory>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <unordered_map>
#include <typeindex>

enum class ActiveState {
	ACTIVE,
	UPDATE_STOP,
	DRAW_STOP,
	ALL_STOP,
	MAX
};

enum class DrawContainer {
	Default,
	Child,
	AbsFront,
	UI,

	Max
};

struct TagAndID
{
	std::string tag = "";
	uint16_t id = 0;
};

class Component; // 前方宣言

class GameObject final {	// 変に継承されないようにするためにfinalを付ける
private:
	std::vector<std::unique_ptr<Component>> m_Components;
	std::vector<std::unique_ptr<Component>> m_RenderComponents;
	std::unordered_map<std::type_index, Component*> m_ComponentMap;
	std::vector<GameObject*> children;	// 子オブジェクトを持つことができる
	GameObject* parent = nullptr;	// 親オブジェクトを持つことができる
	TagAndID m_TagAndID = { "",0 }; // タグとIDのペア
	uint32_t m_InstanceID = 0; // インスタンスID
	std::string name = "";	// オブジェクトの名前
	bool m_Deletefg = false;	// オブジェクトを削除して良いかどうかのフラグ
	bool drawContainerChangeFlag = false;	// コンテナを入れ替える
	bool carryOverFlag = false; // シーンを跨いでオブジェクトを持ち越すかどうかのフラグ
	ActiveState activeState = ActiveState::ACTIVE;
	DrawContainer drawContainer = DrawContainer::Default;
	DrawContainer hopeDrawContainer = DrawContainer::Default;

public:

	GameObject(const std::string& _name, const uint32_t& id)
		: name(_name), m_InstanceID(id) {
		m_Components.reserve(20);
		m_RenderComponents.reserve(4);
	}; // 名前とタグを指定して初期化
	~GameObject();	// デフォルトデストラクタ

	void Update();

	void Draw();

	// セッター
	inline void SetDeleteFg(const bool deletefg) { this->m_Deletefg = deletefg; };
	inline void SetTag(const std::string& tag) { m_TagAndID.tag = tag; };
	inline void SetID(const uint16_t& id) { m_TagAndID.id = id; };
	inline void SetName(const std::string& name) { this->name = name; };
	inline void SetChild(GameObject* obj) {

		auto childObj = std::find(children.begin(), children.end(), obj);
		if (childObj != children.end()) {
			return; // すでに子オブジェクトとして登録されている場合は何もしない
		}

		children.push_back(obj);
		obj->parent = this;	// 親オブジェクトを設定
	};
	inline void RemoveChild(GameObject* obj) {
		auto childObj = std::find(children.begin(), children.end(), obj);
		if (childObj != children.end()) {
			children.erase(childObj);
			obj->parent = nullptr; // 親オブジェクトを解除
		}
	};

	inline void SetActiveState(const ActiveState& as) { activeState = as; };
	inline void SetDrawContainer(const DrawContainer& dc) { drawContainer = dc; };
	inline void SetDrawContainerChangeFlag(const DrawContainer& dc, const bool dccFlag) {
		hopeDrawContainer = dc;
		drawContainerChangeFlag = dccFlag;
	};

	inline void SetCarryOverFlag(const bool flag) { carryOverFlag = flag; };

	// ゲッター
	inline bool GetDeleteFg()const { return m_Deletefg; };
	inline std::string GetTag()const { return m_TagAndID.tag; };
	inline uint16_t GetID()const { return m_TagAndID.id; };
	inline std::string GetName()const { return name; };
	inline GameObject* GetParent() { return parent; };
	inline std::vector<GameObject*>& GetChildren() { return children; };
	inline ActiveState GetActiveState()const { return activeState; };
	inline DrawContainer GetDrawContainer()const { return drawContainer; };
	inline DrawContainer GetHopeDrawContainer()const { return hopeDrawContainer; };
	inline bool GetDrawContainerChangeFlag()const { return drawContainerChangeFlag; };
	inline bool GetCarryOverFlag()const { return carryOverFlag; };
	inline uint32_t GetInstanceID()const { return m_InstanceID; };

	void Destroy() { m_Deletefg = true; };

	// コンポーネントのソート番号でソート
	void SortComponents();

	bool ComponentCheck(Component* comp);

	// 装備されているコンポーネントを取得して使用可能にする
	// ただし、同じ型のコンポーネントを複数持てないことに注意
	template<typename T1>
	T1* GetComponent() {

		auto it = m_ComponentMap.find(typeid(T1));
		return (it != m_ComponentMap.end()) ? static_cast<T1*>(it->second): nullptr;

		//for (auto& component : m_Components) { // ゲームオブジェクト内のコンポーネントをループで見る
		//	if (auto ptr = dynamic_cast<T1*>(component.get())) {	// ダイナミックキャストでキャスト可能かどうか判定
		//		return ptr;
		//	}
		//}

		//for (auto& component : m_RenderComponents) { // ゲームオブジェクト内のコンポーネントをループで見る
		//	if (auto ptr = dynamic_cast<T1*>(component.get())) {	// ダイナミックキャストでキャスト可能かどうか判定
		//		return ptr;
		//	}
		//}

		//return nullptr; // 指定された型がなかった場合nullptr
	}

	template<typename T2>
	std::vector<T2*> GetComponents() {
		std::vector<T2*> comps;
		comps.clear();

		for (auto& component : m_Components) { // ゲームオブジェクト内のコンポーネントをループで見る
			if (auto ptr = dynamic_cast<T2*>(component.get())) {	// ダイナミックキャストでキャスト可能かどうか判定
				comps.push_back(ptr);
			}
		}
		for (auto& component : m_RenderComponents) { // ゲームオブジェクト内のコンポーネントをループで見る
			if (auto ptr = dynamic_cast<T2*>(component.get())) {	// ダイナミックキャストでキャスト可能かどうか判定
				comps.push_back(ptr);
			}
		}
		return comps; // 指定された型のコンポーネント群を返す
	}

	// コンポーネントを追加する
	// 実体はinlファイルに記述して、インクルードによる循環参照を防ぐ（保険程度）
	template<typename T3>
	T3* AddComponent();
};

#include "GameObjectTemplate.inl"