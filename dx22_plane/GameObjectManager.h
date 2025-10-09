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

class GameObjectManager final
{
private:

	std::vector<std::unique_ptr<GameObject>> objects;			 // シーンをnewする度に様々なオブジェクトを格納するようにする
	std::vector<std::unique_ptr<GameObject>> child_Objects;	 // ゲーム内で、実際に更新をかけるベクター
	std::vector<std::unique_ptr<GameObject>> objects_UI;			 // シーンをnewする度に様々なオブジェクトを格納するようにする
	std::vector<std::unique_ptr<GameObject>> objects_Absfront;	 // シーンをnewする度に様々なオブジェクトを格納するようにする

	std::vector<std::unique_ptr<GameObject>> temporaryContainer; // 一時的にオブジェクトを保管するコンテナ

	// コンストラクタ
	GameObjectManager() = default;
	// デストラクタ
	~GameObjectManager() = default;

	GameObject* HelperAddObject(const DrawContainer& dc, const std::string& _name, const std::string& _tag);
	void HelperRemoveObject(std::vector<std::unique_ptr<GameObject>>& objs);
	void HelperRemoveTagObject(std::vector<std::unique_ptr<GameObject>>& objs, const std::string& tag);
	void HelperUpdate(std::vector<std::unique_ptr<GameObject>>& objs);
	void HelperDraw(std::vector<std::unique_ptr<GameObject>>& objs);
	void HelperChangeContainer(std::vector<std::unique_ptr<GameObject>>& objs);

public:	

	void Init() { 
		objects.reserve(100);
		child_Objects.reserve(100);
		objects_UI.reserve(100);
		objects_Absfront.reserve(100);
		temporaryContainer.reserve(100);
	};

	
	void UnInit() { ListClear(); };

	static GameObjectManager& GetInstance();

	// リストにゲームオブジェクトを追加
	GameObject* AddObject(const std::string& _name = "Noname", const std::string& _tag = "Notag");

	GameObject* AddChild(const std::string& _name = "Noname", const std::string& _tag = "Notag");

	GameObject* AddUI(const std::string& _name = "Noname", const std::string& _tag = "Notag");

	GameObject* AddAbsFront(const std::string& _name = "Noname", const std::string& _tag = "Notag");

	void TransferAddObjects();

	void RemoveObject();	 // オブジェクトを削除する（後に使いやすいように改造）
	void RemoveTagObject(const std::string& tag);
	void Update();
	void Draw();
	void OtherThanClear();
	void ListClear();		// ベクター内をクリア

	int ListSize() { return static_cast<int>(objects.size()); };	// オブジェクトをいくつ格納しているのかを返す

	void ChangeContainer();

	// ソートをして、描画順の問題解決、が、不必要なら消去する
	void DrawSort();

	// ゲームオブジェクトを見つければ返す
	GameObject*	GameObjectFindName(const std::string&);	// 名前検索（１体のみ）でゲームオブジェクトを持ってくるか考える
	std::vector<GameObject*> GameObjectFindTag(const std::string&);	// タグ検索（複数体）でゲームオブジェクトを持ってくるか考える
	GameObject* GameObjectFindNameUI(const std::string&);	// 名前検索（１体のみ）でゲームオブジェクトを持ってくるか考える
	std::vector<GameObject*> GameObjectFindTagUI(const std::string&);	// タグ検索（複数体）でゲームオブジェクトを持ってくるか考える
	GameObject* GameObjectFindNameAbsFront(const std::string&);	// 名前検索（１体のみ）でゲームオブジェクトを持ってくるか考える
	std::vector<GameObject*> GameObjectFindTagAbsFront(const std::string&);	// タグ検索（複数体）でゲームオブジェクトを持ってくるか考える
	void SizeUP();
};

