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
#include <ranges>
#include <algorithm>
#include <string>
#include <codecvt>
#include <locale>
#include <utility> // std::move
#include <deque>
#include <cstdint>
#include <unordered_set>
#include "GameObject.h"

class GameObject;	// 相互インクルードしないように前方宣言

class GameObjectManager
{
public:
	// コンストラクタ
	GameObjectManager() = default;

	static void Init() {
		objects.reserve(100);
		child_Objects.reserve(100);
		objects_UI.reserve(100);
		objects_Absfront.reserve(100);
		temporaryContainer.reserve(100);
	};

	// デストラクタ
	~GameObjectManager() = default;
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

	static int ListSize() { return static_cast<int>(objects.size()); };	// オブジェクトをいくつ格納しているのかを返す

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

		return 	HelperFindTags(objects, ids); // 一致するオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindUITags(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return 	HelperFindTags(child_Objects, ids); // 一致するオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindChildUITags(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return 	HelperFindTags(objects_UI, ids); // 一致するオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindAbsFrontTags(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return 	HelperFindTags(objects_Absfront, ids); // 一致するオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindTagsOtherThan(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return HelperFindTagsOtherThan(objects,ids); // 一致しないオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindChildTagsOtherThan(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return HelperFindTagsOtherThan(child_Objects, ids); // 一致しないオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindUITagsOtherThan(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return HelperFindTagsOtherThan(objects_UI, ids); // 一致しないオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindAbsFrontTagsOtherThan(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		return HelperFindTagsOtherThan(objects_Absfront, ids); // 一致しないオブジェクトのベクターを返す
	}

	template<typename ... Tags>
	static std::vector<GameObject*> GameObjectFindAllTags(Tags&& ... tags) {

		std::unordered_set<uint16_t> ids{ TagToIDGet(std::forward<Tags>(tags))... };

		std::vector<GameObject*> matchingObjects = HelperFindTags(objects, ids);
		std::vector<GameObject*> matchingObjectsChild = HelperFindTags(child_Objects, ids);
		std::vector<GameObject*> matchingObjectsUI = HelperFindTags(objects_UI, ids);
		std::vector<GameObject*> matchingObjectsAbs = HelperFindTags(objects_Absfront, ids);

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

		std::vector<GameObject*> matchingObjects = HelperFindTagsOtherThan(objects, ids);
		std::vector<GameObject*> matchingObjectsChild = HelperFindTagsOtherThan(child_Objects, ids);
		std::vector<GameObject*> matchingObjectsUI = HelperFindTagsOtherThan(objects_UI, ids);
		std::vector<GameObject*> matchingObjectsAbs = HelperFindTagsOtherThan(objects_Absfront, ids);

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

private:

	static std::vector<std::unique_ptr<GameObject>> objects;			 // シーンをnewする度に様々なオブジェクトを格納するようにする
	static std::vector<std::unique_ptr<GameObject>> child_Objects;	 // ゲーム内で、実際に更新をかけるベクター
	static std::vector<std::unique_ptr<GameObject>> objects_UI;			 // シーンをnewする度に様々なオブジェクトを格納するようにする
	static std::vector<std::unique_ptr<GameObject>> objects_Absfront;	 // シーンをnewする度に様々なオブジェクトを格納するようにする

	static std::vector<std::unique_ptr<GameObject>> temporaryContainer; // 一時的にオブジェクトを保管するコンテナ

	static std::vector<TagAndID> m_TagAndIDList; // タグとIDのリスト

	static uint16_t TagToIDRegister(const std::string& tag);
	static uint16_t TagToIDGet(const std::string& tag);
	static inline uint32_t m_NextID = 0;

	static GameObject* HelperAddObject(const DrawContainer& dc, const std::string& _name, const std::string& _tag);
	static void HelperRemoveObject(std::vector<std::unique_ptr<GameObject>>& objs);
	static void HelperRemoveTagObject(std::vector<std::unique_ptr<GameObject>>& objs, const uint16_t& ids);
	static void HelperUpdate(std::vector<std::unique_ptr<GameObject>>& objs);
	static void HelperDraw(std::vector<std::unique_ptr<GameObject>>& objs);
	static void HelperChangeContainer(std::vector<std::unique_ptr<GameObject>>& objs);
	static std::vector<GameObject*> HelperFindTag(const std::vector<std::unique_ptr<GameObject>>& objs, const uint16_t& id);
	static std::vector<GameObject*> HelperFindTags(const std::vector<std::unique_ptr<GameObject>>& objs, const std::unordered_set<uint16_t>& ids);
	static std::vector<GameObject*> HelperFindTagsOtherThan(const std::vector<std::unique_ptr<GameObject>>& objs, const std::unordered_set<uint16_t>& ids);
};

