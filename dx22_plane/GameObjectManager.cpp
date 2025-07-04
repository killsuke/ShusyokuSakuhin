#include "GameObjectManager.h"
#include "SceneManager.h"

// 静的な宣言として必要
std::vector<std::shared_ptr<GameObject>>
GameObjectManager::objects;		// ゲーム内で、実際に更新をかけるベクター

// どうやってオブジェクトをリストから削除するかを考える
void GameObjectManager::RemoveObject() {
	// 条件が合えばベクターの中の要素を削除する
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), [](const std::shared_ptr<GameObject>& obj) {
			return obj->GetDeleteFg(); // 削除フラグがtrueのオブジェクトを対象に
			}),
		objects.end());
}

void GameObjectManager::RemoveTagObject(const std::string& tag) {
	
	std::vector<std::shared_ptr<GameObject>> matchingObjects;
	for (const auto& obj : objects) { // objects をループで探索
		if (obj->GetTag() == tag) { // タグが一致するかチェック
			obj->SetDeleteFg(true);
		}
	}
}

// 更新
void GameObjectManager::Update() {
	SizeUP();
	gpd.Update();
	for (auto obj : objects) {
		obj->Update();
		// シーンチェンジが起こったらブレイクしてfor文を抜ける
		if (SceneManager::GetSCFrag() == true) {
			SceneManager::SetSCFrag(false);
			break;
		}
	}

	// インスタンスの削除処理
	RemoveObject();

	// 描画順を整える
	DrawSort();
}

// 描画
void GameObjectManager::Draw() {
	
	// 描画順を整える
	DrawSort();

	// 全てのGameObjectの描画はここでやる
	for (auto& obj : objects) {	
		obj->Draw();
		// シーンチェンジが起こったらブレイクしてfor文を抜ける
		if (SceneManager::GetSCFrag()) {
			SceneManager::SetSCFrag(false);
			break;
		}
	}
}

// オブジェクトを管理するリストを全て空にする
void GameObjectManager::ListClear() {
	objects.clear();
};

// 片付け
void GameObjectManager::Uninit() {
	ListClear();
};

// 描画順のソート
void GameObjectManager::DrawSort() {
	auto sortZLambda = [](const std::shared_ptr<GameObject>& a, const std::shared_ptr<GameObject>& b) {
		return a->GetComponent<TransformComponent>()->GetPos().z > b->GetComponent<TransformComponent>()->GetPos().z;
	};

	if (!std::is_sorted(objects.begin(), objects.end(), sortZLambda)) {
		std::sort(objects.begin(), objects.end(), sortZLambda);  // std::is_sortedではなくstd::sortを呼び出す
	}
}

// 検索したオブジェクトを１つ返すが、存在しない場合は止まるので注意
std::shared_ptr<GameObject> GameObjectManager::GameObjectFindName(const std::string& name) {
	for (const auto& obj : objects) { // objects をループで探索
		if (obj->GetName() == name) { // 名前が一致するかチェック
			return obj; // 一致する場合、そのオブジェクトを返す
		}
	}
	// これを使う場合、nullptr、つまり存在しない場所を参照することになるので、
	// 必ずアドレスが存在するかどうかを確認すること。
	return nullptr; // 一致するオブジェクトが見つからなかった場合、nullptrを返す
}

// 検索したオブジェクトを複数返すが、存在しない場合は止まるので注意
std::vector<std::shared_ptr<GameObject>> GameObjectManager::GameObjectFindTag(const std::string& tag) {
	std::vector<std::shared_ptr<GameObject>> matchingObjects;
	for (const auto& obj : objects) { // objects をループで探索
		if (obj->GetTag() == tag) { // タグが一致するかチェック
			matchingObjects.push_back(obj); // 一致するオブジェクトを追加
		}
	}

	// ベクター配列内が空なら空のベクターを返す
	return matchingObjects; // 一致するオブジェクトのベクターを返す
}

// 容量が足りなくなった場合追加
void GameObjectManager::SizeUP() {

	if (objects.size() >= objects.capacity() * 0.7) {
		objects.reserve(objects.capacity() + 1000);
	}
}

GetterPlayerBullet GameObjectManager::GetGetterPlayerBulletObj() {
	return gpd; 
};