// -----------------------------------------------------------
// ゲームオブジェクトマネージャー
// ゲームオブジェクトの管理をし、
// シーンが切り替わる度にリストの中身を入れ替える
// なお、シーンマネージャーに結びついているため、
// このゲームオブジェクトマネージャーの解放は
// シーンマネージャーに任せています。
// -----------------------------------------------------------

#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_set>
#include "GameObject/GameObject.h"

class GameObject;	// 相互インクルードしないように前方宣言

class GameObjectManager final
{
private:

	static inline std::vector<std::unique_ptr<GameObject>> m_Objects;
	static inline std::vector<std::unique_ptr<GameObject>> m_Child_Objects;
	static inline std::vector<std::unique_ptr<GameObject>> m_Objects_UI;
	static inline std::vector<std::unique_ptr<GameObject>> m_Objects_Absfront;
	static inline std::vector<std::unique_ptr<GameObject>> m_TemporaryContainer; // 一時的にオブジェクトを保管するコンテナ
	static inline std::vector<TagAndID> m_TagAndIDList; // タグとIDのリスト

	static inline uint32_t m_NextID = 0;
	static uint16_t TagToIDGet(const std::string& tag);
	static uint16_t TagToIDRegister(const std::string& tag);

	static GameObject* HelperAddObject(const DrawContainer& dc, const std::string& _name, const std::string& _tag);
	static void HelperRemoveObject(std::vector<std::unique_ptr<GameObject>>& objs);
	static void HelperRemoveTagObject(std::vector<std::unique_ptr<GameObject>>& objs, const uint16_t& ids);
	static void HelperUpdate(std::vector<std::unique_ptr<GameObject>>& objs);
	static void HelperDraw(std::vector<std::unique_ptr<GameObject>>& objs);
	static void HelperChangeContainer(std::vector<std::unique_ptr<GameObject>>& objs);
	static std::vector<GameObject*> HelperFindTag(const std::vector<std::unique_ptr<GameObject>>& objs, const uint16_t& id);
	static std::vector<GameObject*> HelperFindTags(const std::vector<std::unique_ptr<GameObject>>& objs, const std::unordered_set<uint16_t>& ids);
	static std::vector<GameObject*> HelperFindTagsOtherThan(const std::vector<std::unique_ptr<GameObject>>& objs, const std::unordered_set<uint16_t>& ids);
	static GameObject* HelperFindInstanceID(const std::vector<std::unique_ptr<GameObject>>& objs, const uint32_t& id);

public:

	// コンストラクタ・デストラクタを削除
	GameObjectManager() = delete;
	~GameObjectManager() = delete;

	// コピー・ムーブも削除
	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager(GameObjectManager&&) = delete;
	GameObjectManager& operator=(const GameObjectManager&) = delete;
	GameObjectManager& operator=(GameObjectManager&&) = delete;

	static void Init() {
		m_Objects.reserve(100);
		m_Child_Objects.reserve(100);
		m_Objects_UI.reserve(100);
		m_Objects_Absfront.reserve(100);
		m_TemporaryContainer.reserve(100);
	};

	static void UnInit() { ListClear(); };

	// リストにゲームオブジェクトを追加
	static GameObject* AddObject(const std::string& _name = "Noname", const std::string& _tag = "Notag");

	static GameObject* AddChild(const std::string& _name = "Noname", const std::string& _tag = "Notag");

	static GameObject* AddUI(const std::string& _name = "Noname", const std::string& _tag = "Notag");

	static GameObject* AddAbsFront(const std::string& _name = "Noname", const std::string& _tag = "Notag");

	static void TransferAddObjects();

	static void RemoveObject();	 // オブジェクトを削除する（後に使いやすいように改造）
	static void RemoveTagObject(const std::string& tag);
	static void Update();
	static void Draw();
	static void OtherThanClear();
	static void ListClear();		// ベクター内をクリア

	static int ListSize() { return static_cast<int>(m_Objects.size()); };	// オブジェクトをいくつ格納しているのかを返す

	static void ChangeContainer();

	static void SizeUP();

	// ソートをして、描画順の問題解決、が、不必要なら消去する
	static void DrawSort();

	// ゲームオブジェクトを見つければ返す
	static GameObject* GameObjectFindName(const std::string&);	// 名前検索（１体のみ）でゲームオブジェクトを持ってくるか考える
	static std::vector<GameObject*> GameObjectFindTag(const std::string&);	// タグ検索（複数体）でゲームオブジェクトを持ってくるか考える
	static GameObject* GameObjectFindNameUI(const std::string&);	// 名前検索（１体のみ）でゲームオブジェクトを持ってくるか考える
	static std::vector<GameObject*> GameObjectFindTagUI(const std::string&);	// タグ検索（複数体）でゲームオブジェクトを持ってくるか考える
	static GameObject* GameObjectFindNameAbsFront(const std::string&);	// 名前検索（１体のみ）でゲームオブジェクトを持ってくるか考える
	static std::vector<GameObject*> GameObjectFindTagAbsFront(const std::string&);	// タグ検索（複数体）でゲームオブジェクトを持ってくるか考える
	static std::vector<GameObject*> GameObjectFindAllTag(const std::string& tag);	// タグ検索（複数体）でゲームオブジェクトを持ってくるか考える
	static std::vector<GameObject*> GameObjectFindAllTag(const std::vector<std::string>& tags);	// タグ検索（複数体）でゲームオブジェクトを持ってくるか考える
	static std::vector<GameObject*> GameObjectFindAllTag(const std::unordered_set<std::string>& tags);	// タグ検索（複数体）でゲームオブジェクトを持ってくるか考える

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindTags(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return 	HelperFindTags(m_Objects, ids); // 一致するオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindUITags(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return 	HelperFindTags(m_Child_Objects, ids); // 一致するオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindChildUITags(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return 	HelperFindTags(m_Objects_UI, ids); // 一致するオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindAbsFrontTags(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return 	HelperFindTags(m_Objects_Absfront, ids); // 一致するオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindTagsOtherThan(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return HelperFindTagsOtherThan(m_Objects,ids); // 一致しないオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindChildTagsOtherThan(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return HelperFindTagsOtherThan(m_Child_Objects, ids); // 一致しないオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindUITagsOtherThan(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return HelperFindTagsOtherThan(m_Objects_UI, ids); // 一致しないオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindAbsFrontTagsOtherThan(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return HelperFindTagsOtherThan(m_Objects_Absfront, ids); // 一致しないオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindAllTags(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		std::vector<GameObject*> matchingObjects = HelperFindTags(m_Objects, ids);
		std::vector<GameObject*> matchingObjectsChild = HelperFindTags(m_Child_Objects, ids);
		std::vector<GameObject*> matchingObjectsUI = HelperFindTags(m_Objects_UI, ids);
		std::vector<GameObject*> matchingObjectsAbs = HelperFindTags(m_Objects_Absfront, ids);

		std::vector<GameObject*> result;

		result.reserve(
			matchingObjects.size() +
			matchingObjectsChild.size() +
			matchingObjectsUI.size() +
			matchingObjectsAbs.size()
		);

		if(result.capacity() == 0) {
			return result; // 空のベクターを返す
		}

		result.insert(result.end(), matchingObjects.begin(), matchingObjects.end());
		result.insert(result.end(), matchingObjectsChild.begin(), matchingObjectsChild.end());
		result.insert(result.end(), matchingObjectsUI.begin(), matchingObjectsUI.end());
		result.insert(result.end(), matchingObjectsAbs.begin(), matchingObjectsAbs.end());

		return 	result; // 一致するオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindAllTagsOtherThan(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		std::vector<GameObject*> matchingObjects = HelperFindTagsOtherThan(m_Objects, ids);
		std::vector<GameObject*> matchingObjectsChild = HelperFindTagsOtherThan(m_Child_Objects, ids);
		std::vector<GameObject*> matchingObjectsUI = HelperFindTagsOtherThan(m_Objects_UI, ids);
		std::vector<GameObject*> matchingObjectsAbs = HelperFindTagsOtherThan(m_Objects_Absfront, ids);

		std::vector<GameObject*> result;

		result.reserve(
			matchingObjects.size() +
			matchingObjectsChild.size() +
			matchingObjectsUI.size() +
			matchingObjectsAbs.size()
		);

		if (result.capacity() == 0) {
			return result; // 空のベクターを返す
		}

		result.insert(result.end(), matchingObjects.begin(), matchingObjects.end());
		result.insert(result.end(), matchingObjectsChild.begin(), matchingObjectsChild.end());
		result.insert(result.end(), matchingObjectsUI.begin(), matchingObjectsUI.end());
		result.insert(result.end(), matchingObjectsAbs.begin(), matchingObjectsAbs.end());

		return 	result; // 一致するオブジェクトのベクターを返す
	}

	static GameObject* GameObjectFindInstanceID(const uint32_t& id) { // インスタンスID検索でゲームオブジェクトを持ってくるか考える
		return HelperFindInstanceID(m_Objects, id);
	}

	static GameObject* GameObjectFindInstanceIDAll(const uint32_t& id) { // インスタンスID検索でゲームオブジェクトを持ってくるか考える
		GameObject* obj = HelperFindInstanceID(m_Objects, id);
		if (obj != nullptr) {
			return obj;
		}
		obj = HelperFindInstanceID(m_Child_Objects, id);
		if (obj != nullptr) {
			return obj;
		}
		obj = HelperFindInstanceID(m_Objects_UI, id);
		if (obj != nullptr) {
			return obj;
		}
		obj = HelperFindInstanceID(m_Objects_Absfront, id);
		if (obj != nullptr) {
			return obj;
		}
		return nullptr;
	}
};

