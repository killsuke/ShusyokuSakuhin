#include "GameObjectManager.h"
#include "SceneManager.h"
#include "Transform.h"
#include "DirectXRender.h"

// 静的な宣言として必要
std::vector<std::unique_ptr<GameObject>>
GameObjectManager::objects;		// ゲーム内で、実際に更新をかけるベクター
std::vector<std::unique_ptr<GameObject>>
GameObjectManager::child_Objects;		// ゲーム内で、実際に更新をかけるベクター
std::vector<std::unique_ptr<GameObject>>
GameObjectManager::objects_UI;		// ゲーム内で、実際に更新をかけるベクター
std::vector<std::unique_ptr<GameObject>>
GameObjectManager::objects_Absfront;		// ゲーム内で、実際に更新をかけるベクター

// どうやってオブジェクトをリストから削除するかを考える
void GameObjectManager::RemoveObject() {
	// 条件が合えばベクターの中の要素を削除する
	//objects.erase(
	//	std::remove_if(objects.begin(), objects.end(), [](const std::unique_ptr<GameObject>& obj) {
	//		return obj->GetDeleteFg(); // 削除フラグがtrueのオブジェクトを対象に
	//		}),
	//	objects.end());

	// C++ 20で使えるコンテナの要素削除処理
	std::erase_if(objects, [](const std::unique_ptr<GameObject>& obj) {
		return obj->GetDeleteFg();
		});

	std::erase_if(child_Objects, [](const std::unique_ptr<GameObject>& obj) {
		return obj->GetDeleteFg();
		});

	std::erase_if(objects_UI, [](const std::unique_ptr<GameObject>& obj) {
		return obj->GetDeleteFg();
		});

	std::erase_if(objects_Absfront, [](const std::unique_ptr<GameObject>& obj) {
		return obj->GetDeleteFg();
		});
}

void GameObjectManager::RemoveTagObject(const std::string& tag) {

	for (const auto& obj : objects) { // objects をループで探索
		if (obj->GetTag() == tag) { // タグが一致するかチェック
			obj->SetDeleteFg(true);
		}
	}

	for (const auto& obj : child_Objects) { // child_Objects をループで探索
		if (obj->GetTag() == tag) { // タグが一致するかチェック
			obj->SetDeleteFg(true);
		}
	}

	for (const auto& obj : objects_UI) { // objects_UI をループで探索
		if (obj->GetTag() == tag) { // タグが一致するかチェック
			obj->SetDeleteFg(true);
		}
	}

	for (const auto& obj : objects_Absfront) { // objects_Absfront をループで探索
		if (obj->GetTag() == tag) { // タグが一致するかチェック
			obj->SetDeleteFg(true);
		}
	}
}

// 更新
void GameObjectManager::Update() {
	SizeUP();

	// 描画順を整える
	//DrawSort();

	for (const auto& obj : objects) {
		obj->Update();
		// シーンチェンジが起こったらブレイクしてfor文を抜ける
		if (SceneManager::GetSCFrag() == true) {
			SceneManager::SetSCFrag(false);
			break;
		}
	}

	for (const auto& obj : objects_UI) {
		obj->Update();
		// シーンチェンジが起こったらブレイクしてfor文を抜ける
		if (SceneManager::GetSCFrag() == true) {
			SceneManager::SetSCFrag(false);
			break;
		}
	}

	auto deviceContext = DirectXRender::GetDeviceContext();

	DirectXRender::SetDepthEnable(false);

	for (const auto& obj : objects_Absfront) {
		obj->Update();
		// シーンチェンジが起こったらブレイクしてfor文を抜ける
		if (SceneManager::GetSCFrag() == true) {
			SceneManager::SetSCFrag(false);
			break;
		}
	}
	DirectXRender::SetDepthEnable(true);
	// インスタンスの削除処理
	RemoveObject();
}

// オブジェクトを管理するリストを全て空にする
void GameObjectManager::ListClear() {
	objects.clear();
	child_Objects.clear();
	objects_UI.clear();
	objects_Absfront.clear();
};

// 描画順のソート
void GameObjectManager::DrawSort() {
	auto sortZLambdaObjects = [](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b) {
		return a->GetComponent<TransformComponent>()->GetPosition().z >
			b->GetComponent<TransformComponent>()->GetPosition().z;
		};

	if (!std::is_sorted(objects.begin(), objects.end(), sortZLambdaObjects)) {
		std::sort(objects.begin(), objects.end(), sortZLambdaObjects);  // std::is_sortedではなくstd::sortを呼び出す
	}

	auto sortZLambdaChildObjects = [](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b) {
		return a->GetComponent<TransformComponent>()->GetPosition().z >
			b->GetComponent<TransformComponent>()->GetPosition().z;
		};

	if (!std::is_sorted(child_Objects.begin(), child_Objects.end(), sortZLambdaChildObjects)) {
		std::sort(child_Objects.begin(), child_Objects.end(), sortZLambdaChildObjects);  // std::is_sortedではなくstd::sortを呼び出す
	}

	auto sortZLambdaUI = [](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b) {
		return a->GetComponent<TransformComponent>()->GetPosition().z >
			b->GetComponent<TransformComponent>()->GetPosition().z;
		};

	if (!std::is_sorted(objects_UI.begin(), objects_UI.end(), sortZLambdaUI)) {
		std::sort(objects_UI.begin(), objects_UI.end(), sortZLambdaUI);  // std::is_sortedではなくstd::sortを呼び出す
	}

	auto sortZLambdaAbsfront = [](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b) {
		return a->GetComponent<TransformComponent>()->GetPosition().z >
			b->GetComponent<TransformComponent>()->GetPosition().z;
		};

	if (!std::is_sorted(objects_Absfront.begin(), objects_Absfront.end(), sortZLambdaAbsfront)) {
		std::sort(objects_Absfront.begin(), objects_Absfront.end(), sortZLambdaAbsfront);  // std::is_sortedではなくstd::sortを呼び出す
	}
}

// 検索したオブジェクトを１つ返すが、存在しない場合は止まるので注意
GameObject* GameObjectManager::GameObjectFindName(const std::string& name) {
	for (const auto& obj : objects) { // objects をループで探索
		if (obj->GetName() == name) { // 名前が一致するかチェック
			return obj.get(); // 一致する場合、そのオブジェクトを返す
		}
	}
	// これを使う場合、nullptr、つまり存在しない場所を参照することになるので、
	// 必ずアドレスが存在するかどうかを確認すること。
	return nullptr; // 一致するオブジェクトが見つからなかった場合、nullptrを返す
}

// 検索したオブジェクトを複数返すが、存在しない場合は止まるので注意
std::vector<GameObject*> GameObjectManager::GameObjectFindTag(const std::string& tag) {
	std::vector<GameObject*> matchingObjects;
	for (const auto& obj : objects) { // objects をループで探索
		if (obj->GetTag() == tag) { // タグが一致するかチェック
			matchingObjects.push_back(obj.get()); // 一致するオブジェクトを追加
		}
	}

	// ベクター配列内が空なら空のベクターを返す
	return matchingObjects; // 一致するオブジェクトのベクターを返す
}

// 検索したオブジェクトを１つ返すが、存在しない場合は止まるので注意
GameObject* GameObjectManager::GameObjectFindNameUI(const std::string& name) {
	for (const auto& obj : objects_UI) { // objects をループで探索
		if (obj->GetName() == name) { // 名前が一致するかチェック
			return obj.get(); // 一致する場合、そのオブジェクトを返す
		}
	}
	// これを使う場合、nullptr、つまり存在しない場所を参照することになるので、
	// 必ずアドレスが存在するかどうかを確認すること。
	return nullptr; // 一致するオブジェクトが見つからなかった場合、nullptrを返す
}

// 検索したオブジェクトを複数返すが、存在しない場合は止まるので注意
std::vector<GameObject*> GameObjectManager::GameObjectFindTagUI(const std::string& tag) {
	std::vector<GameObject*> matchingObjects;
	for (const auto& obj : objects_UI) { // objects をループで探索
		if (obj->GetTag() == tag) { // タグが一致するかチェック
			matchingObjects.push_back(obj.get()); // 一致するオブジェクトを追加
		}
	}

	// ベクター配列内が空なら空のベクターを返す
	return matchingObjects; // 一致するオブジェクトのベクターを返す
}

// 検索したオブジェクトを１つ返すが、存在しない場合は止まるので注意
GameObject* GameObjectManager::GameObjectFindNameAbsFront(const std::string& name) {
	for (const auto& obj : objects_Absfront) { // objects をループで探索
		if (obj->GetName() == name) { // 名前が一致するかチェック
			return obj.get(); // 一致する場合、そのオブジェクトを返す
		}
	}
	// これを使う場合、nullptr、つまり存在しない場所を参照することになるので、
	// 必ずアドレスが存在するかどうかを確認すること。
	return nullptr; // 一致するオブジェクトが見つからなかった場合、nullptrを返す
}

// 検索したオブジェクトを複数返すが、存在しない場合は止まるので注意
std::vector<GameObject*> GameObjectManager::GameObjectFindTagAbsFront(const std::string& tag) {
	std::vector<GameObject*> matchingObjects;
	for (const auto& obj : objects_Absfront) { // objects をループで探索
		if (obj->GetTag() == tag) { // タグが一致するかチェック
			matchingObjects.push_back(obj.get()); // 一致するオブジェクトを追加
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

	if (child_Objects.size() >= child_Objects.capacity() * 0.7) {
		child_Objects.reserve(child_Objects.capacity() + 100);
	}

	if (objects_UI.size() >= objects_UI.capacity() * 0.7) {
		objects_UI.reserve(objects_UI.capacity() + 100);
	}

	if (objects_Absfront.size() >= objects_Absfront.capacity() * 0.7) {
		objects_Absfront.reserve(objects_Absfront.capacity() + 100);
	}
}
