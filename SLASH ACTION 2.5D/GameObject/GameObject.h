#pragma once
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
	std::vector<GameObject*> m_Children;	// 子オブジェクトを持つことができる
	GameObject* m_Parent = nullptr;	// 親オブジェクトを持つことができる
	TagAndID m_TagAndID = { "",0 }; // タグとIDのペア
	uint32_t m_InstanceID = 0; // インスタンスID
	std::string m_Name = "";	// オブジェクトの名前
	bool m_IsDeletefg = false;	// オブジェクトを削除して良いかどうかのフラグ
	bool m_IsDrawContainerChange = false;	// コンテナを入れ替える
	bool m_CarryOverFlag = false; // シーンを跨いでオブジェクトを持ち越すかどうかのフラグ
	bool m_ChildAbsFrontFlag = false; // 子オブジェクトを絶対前面させるかどうかのフラグ
	ActiveState m_ActiveState = ActiveState::ACTIVE;
	DrawContainer m_DrawContainer = DrawContainer::Default;
	DrawContainer m_HopeDrawContainer = DrawContainer::Default;

public:

	GameObject(const std::string& _name, const uint32_t& id); // 名前とタグを指定して初期化
	~GameObject() = default;	// デフォルトデストラクタ

	GameObject(const GameObject&) = delete;			 // コピーコンストラクタ禁止
	GameObject(GameObject&&) = delete;				 // ムーブコンストラクタ禁止
	GameObject& operator=(const GameObject&) = delete; // コピー代入禁止
	GameObject& operator=(GameObject&&) = delete;		 // ムーブ代入禁止

	void Update();
	void Draw();

	// セッター
	inline void SetDeleteFg(const bool deletefg) { this->m_IsDeletefg = deletefg; };
	inline void SetTag(const std::string& tag) { m_TagAndID.tag = tag; };
	inline void SetID(const uint16_t& id) { m_TagAndID.id = id; };
	inline void SetName(const std::string& name) { this->m_Name = name; };
	inline void SetChild(GameObject* obj) {

        std::vector<GameObject*>::iterator childObj = std::find(m_Children.begin(), m_Children.end(), obj);
        if (childObj != m_Children.end()) {
            return; // すでに子オブジェクトとして登録されている場合は何もしない
        }

        m_Children.push_back(obj);
        obj->m_Parent = this;   // 親オブジェクトを設定
    };
    inline void RemoveChild(GameObject* obj) {
        std::vector<GameObject*>::iterator childObj = std::find(m_Children.begin(), m_Children.end(), obj);
        if (childObj != m_Children.end()) {
            m_Children.erase(childObj);
            obj->m_Parent = nullptr; // 親オブジェクトを解除
        }
    };
	inline void ClearChild() {
		for (GameObject* child : m_Children) {
			child->m_Parent = nullptr; // 親オブジェクトを解除
		}
		m_Children.clear();
	};

	inline void SetActiveState(const ActiveState& as) { m_ActiveState = as; };
	inline void SetDrawContainer(const DrawContainer& dc) { m_DrawContainer = dc; };
	inline void SetDrawContainerChangeFlag(const DrawContainer& dc, const bool dccFlag) {
		m_HopeDrawContainer = dc;
		m_IsDrawContainerChange = dccFlag;
	};

	inline void SetCarryOverFlag(const bool flag) { m_CarryOverFlag = flag; };
	inline void SetChildAbsFrontFlag(const bool flag) { m_ChildAbsFrontFlag = flag; };

	// ゲッター
	inline bool GetDeleteFg()const { return m_IsDeletefg; };
	inline std::string GetTag()const { return m_TagAndID.tag; };
	inline uint16_t GetID()const { return m_TagAndID.id; };
	inline std::string GetName()const { return m_Name; };
	inline GameObject* GetParent() { return m_Parent; };
	inline std::vector<GameObject*>& GetChildren() { return m_Children; };
	inline ActiveState GetActiveState()const { return m_ActiveState; };
	inline DrawContainer GetDrawContainer()const { return m_DrawContainer; };
	inline DrawContainer GetHopeDrawContainer()const { return m_HopeDrawContainer; };
	inline bool GetDrawContainerChangeFlag()const { return m_IsDrawContainerChange; };
	inline bool GetCarryOverFlag()const { return m_CarryOverFlag; };
	inline uint32_t GetInstanceID()const { return m_InstanceID; };

	void Destroy() { m_IsDeletefg = true; };

	// コンポーネントのソート番号でソート
	void SortComponents();

	bool ComponentCheck(Component* comp);

	// 装備されているコンポーネントを取得して使用可能にする
	// ただし、同じ型のコンポーネントを複数持てないことに注意
	template<typename T1>
	T1* GetComponent() {

        std::unordered_map<std::type_index, Component*>::iterator it = m_ComponentMap.find(typeid(T1));
        return (it != m_ComponentMap.end()) ? static_cast<T1*>(it->second): nullptr;
    }

    template<typename T2>
    std::vector<T2*> GetComponents() {
        std::vector<T2*> comps;
        comps.clear();

        for (std::unique_ptr<Component>& component : m_Components) { // ゲームオブジェクト内のコンポーネントをループで見る
            if (T2* ptr = dynamic_cast<T2*>(component.get())) { // ダイナミックキャストでキャスト可能かどうか判定
                comps.push_back(ptr);
            }
        }
        for (std::unique_ptr<Component>& component : m_RenderComponents) { // ゲームオブジェクト内のコンポーネントをループで見る
            if (T2* ptr = dynamic_cast<T2*>(component.get())) { // ダイナミックキャストでキャスト可能かどうか判定
                comps.push_back(ptr);
            }
        }
        return comps; // 指定された型のコンポーネント群を返す
    }

    // コンポーネントを追加する
    template<typename T3>
    // コンポーネントを追加する
    T3* AddComponent() {
        static_assert(std::is_base_of<Component, T3>::value,
            "型エラー！Compnentクラスを継承していません！");  // プロジェクトをUTF-8に変換しておく

        std::unique_ptr<T3> comp = std::make_unique<T3>(*this);   // thisで呼び出した者を取得可能
        T3* ptr = comp.get();  // 一度別で格納してアクセス違反を防ぐ

        Component* p_comp = comp.get();

        // 型IDで登録
        m_ComponentMap[typeid(T3)] = p_comp;

        const bool renderFlag = ComponentCheck(p_comp);

        if (renderFlag == true) {
            m_RenderComponents.emplace_back(std::move(comp));
        }
        else {
            m_Components.emplace_back(std::move(comp));
        }

        SortComponents();
        return ptr;
    }
};