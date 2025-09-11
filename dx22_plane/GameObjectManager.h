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
#include <algorithm>
#include <string>
#include <codecvt>
#include <locale>
#include <utility> // std::move
#include <deque>
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
	};

	// デストラクタ
	~GameObjectManager() = default;
	static void UnInit() { ListClear(); };

	// リストにゲームオブジェクトを追加
	static GameObject* AddObject(const std::string& _name = "Noname", const std::string& _tag = "Notag");

	static GameObject* AddChild(const std::string& _name = "Noname", const std::string& _tag = "Notag");

	static GameObject* AddUI(const std::string& _name = "Noname", const std::string& _tag = "Notag");

	static GameObject* AddAbsFront(const std::string& _name = "Noname", const std::string& _tag = "Notag");

	static void RemoveObject();	 // オブジェクトを削除する（後に使いやすいように改造）
	static void RemoveTagObject(const std::string& tag);
	static void Update();
	static void Draw();
	static void OtherThanTagClear(const std::string& tag);
	static void ListClear();		// ベクター内をクリア

	static int ListSize() { return static_cast<int>(objects.size()); };	// オブジェクトをいくつ格納しているのかを返す

	static void ChangeContainer();

	// ソートをして、描画順の問題解決、が、不必要なら消去する
	static void DrawSort();

	// ゲームオブジェクトを見つければ返す
	static GameObject*	GameObjectFindName(const std::string&);	// 名前検索（１体のみ）でゲームオブジェクトを持ってくるか考える
	static std::vector<GameObject*> GameObjectFindTag(const std::string&);	// タグ検索（複数体）でゲームオブジェクトを持ってくるか考える
	static GameObject* GameObjectFindNameUI(const std::string&);	// 名前検索（１体のみ）でゲームオブジェクトを持ってくるか考える
	static std::vector<GameObject*> GameObjectFindTagUI(const std::string&);	// タグ検索（複数体）でゲームオブジェクトを持ってくるか考える
	static GameObject* GameObjectFindNameAbsFront(const std::string&);	// 名前検索（１体のみ）でゲームオブジェクトを持ってくるか考える
	static std::vector<GameObject*> GameObjectFindTagAbsFront(const std::string&);	// タグ検索（複数体）でゲームオブジェクトを持ってくるか考える
	static void SizeUP();
private:

	static std::vector<std::unique_ptr<GameObject>> objects;			 // シーンをnewする度に様々なオブジェクトを格納するようにする
	static std::vector<std::unique_ptr<GameObject>> child_Objects;	 // ゲーム内で、実際に更新をかけるベクター
	static std::vector<std::unique_ptr<GameObject>> objects_UI;			 // シーンをnewする度に様々なオブジェクトを格納するようにする
	static std::vector<std::unique_ptr<GameObject>> objects_Absfront;	 // シーンをnewする度に様々なオブジェクトを格納するようにする

	static GameObject* HelperAddObject(std::vector<std::unique_ptr<GameObject>>& objs, const std::string& _name, const std::string& _tag);
	static void HelperRemoveObject(std::vector<std::unique_ptr<GameObject>>& objs);
	static void HelperRemoveTagObject(std::vector<std::unique_ptr<GameObject>>& objs, const std::string& tag);
	static void HelperUpdate(std::vector<std::unique_ptr<GameObject>>& objs);
	static void HelperDraw(std::vector<std::unique_ptr<GameObject>>& objs);
	static void HelperChangeContainer(std::vector<std::unique_ptr<GameObject>>& objs);
};

