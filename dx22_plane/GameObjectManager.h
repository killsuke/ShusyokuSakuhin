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
	static GameObject* AddObject(const std::string& _name = "Noname", const std::string& _tag = "Notag") {

		objects.emplace_back(MakeObject(_name, _tag));

		auto ptr = objects.back().get();

		return ptr;
	}

	static GameObject* AddChild(const std::string& _name = "Noname", const std::string& _tag = "Notag") {

		child_Objects.emplace_back(MakeObject(_name, _tag));

		auto ptr = child_Objects.back().get();

		return ptr;
	}

	static GameObject* AddUI(const std::string& _name = "Noname", const std::string& _tag = "Notag") {
		//static_assert(std::is_base_of<GameObject, T1>::value, "T1 must be derived from GameObject");

		objects_UI.emplace_back(MakeObject(_name, _tag));

		auto ptr = objects_UI.back().get();

		return ptr;
	}

	static GameObject* AddAbsFront(const std::string& _name = "Noname", const std::string& _tag = "Notag") {
		//static_assert(std::is_base_of<GameObject, T1>::value, "T1 must be derived from GameObject");

		objects_Absfront.emplace_back(MakeObject(_name, _tag));

		auto ptr = objects_Absfront.back().get();

		return ptr;
	}

	static void RemoveObject();	 // オブジェクトを削除する（後に使いやすいように改造）
	static void RemoveTagObject(const std::string& tag);
	static void Update();
	static void Draw();
	static void ListClear();		// ベクター内をクリア

	static int ListSize() { return static_cast<int>(objects.size()); };	// オブジェクトをいくつ格納しているのかを返す

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

	// 型変換をして作成の手伝い、GameObjectを継承したクラスであるならば追加する
//	template <typename T2>
	static std::unique_ptr<GameObject> MakeObject(const std::string& _name, const std::string& _tag) {	 // オブジェクトを作成して追加
	//	static_assert(std::is_base_of<GameObject, T2>::value, "T2 must be derived from GameObject");	// コンパイル時にGameObjectを継承したクラスであるかどうかの検査が入る
		return std::make_unique<GameObject>(_name, _tag);
	}
};

