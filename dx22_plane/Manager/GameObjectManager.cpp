#include "GameObjectManager.h"
#include "SceneManager.h"
#include "EventBusManager.h"
#include "Components/Transform.h"
#include "System/DirectXRender.h"
#include <iostream>

// 静的な宣言として必要
std::vector<std::unique_ptr<GameObject>>
GameObjectManager::objects;		// ゲーム内で、実際に更新をかけるベクター
std::vector<std::unique_ptr<GameObject>>
GameObjectManager::child_Objects;		// ゲーム内で、実際に更新をかけるベクター
std::vector<std::unique_ptr<GameObject>>
GameObjectManager::objects_UI;		// ゲーム内で、実際に更新をかけるベクター
std::vector<std::unique_ptr<GameObject>>
GameObjectManager::objects_Absfront;		// ゲーム内で、実際に更新をかけるベクター
std::vector<std::unique_ptr<GameObject>>
GameObjectManager::temporaryContainer; // 一時的にオブジェクトを保管するコンテナ
std::vector<TagAndID>
GameObjectManager::m_TagAndIDList;

// リストにゲームオブジェクトを追加
GameObject* GameObjectManager::AddObject(const std::string& _name, const std::string& _tag) {

	GameObject* ptr = HelperAddObject(DrawContainer::Default, _name, _tag);

	return ptr;
}

GameObject* GameObjectManager::GameObjectManager::AddChild(const std::string& _name, const std::string& _tag) {

	GameObject* ptr = HelperAddObject(DrawContainer::Child, _name, _tag);

	return ptr;
}

GameObject* GameObjectManager::AddUI(const std::string& _name, const std::string& _tag) {

	GameObject* ptr = HelperAddObject(DrawContainer::UI, _name, _tag);

	return ptr;
}

GameObject* GameObjectManager::AddAbsFront(const std::string& _name, const std::string& _tag) {

	GameObject* ptr = HelperAddObject(DrawContainer::AbsFront, _name, _tag);

	return ptr;
}

// タグとIDの登録
uint16_t GameObjectManager::TagToIDRegister(const std::string& tag) {

	const uint16_t id = TagToIDGet(tag);

	if (id != UINT16_MAX) {
		// 既に登録されている場合は何もしない
		return id;
	}

	uint16_t newID = static_cast<uint16_t>(m_TagAndIDList.size());
	m_TagAndIDList.push_back({ tag, newID });

	return newID;
}

uint16_t GameObjectManager::TagToIDGet(const std::string& tag) {

	for (const TagAndID& tagAndId : m_TagAndIDList) {
		if (tagAndId.tag == tag) {
			return tagAndId.id; // タグに対応するIDを返す
		}
	}

	return UINT16_MAX; // 見つからなかった場合のエラー値
}

GameObject* GameObjectManager::HelperAddObject(const DrawContainer& dc, const std::string& _name, const std::string& _tag) {
	
	temporaryContainer.push_back(std::make_unique<GameObject>(_name,m_NextID));

	GameObject* ptr = temporaryContainer.back().get();

	// タグとIDの登録
	const uint16_t id = TagToIDRegister(_tag);
	ptr->SetTag(_tag);
	ptr->SetID(id);

	ptr->SetDrawContainer(dc);

	m_NextID++;

	return ptr;
}

void GameObjectManager::HelperRemoveObject(std::vector<std::unique_ptr<GameObject>>& objs) {
	// C++ 20で使えるコンテナの要素削除処理
	std::erase_if(objs, [](const std::unique_ptr<GameObject>& obj) {
		return obj->GetDeleteFg();
		});
}

void GameObjectManager::HelperRemoveTagObject(std::vector<std::unique_ptr<GameObject>>& objs, const uint16_t& id) {

	for (const auto& obj : objs) { // objects をループで探索
		if (obj->GetID() == id) { // タグが一致するかチェック
			obj->SetDeleteFg(true);
		}
	}
}

void GameObjectManager::HelperUpdate(std::vector<std::unique_ptr<GameObject>>& objs) {
	for (const auto& obj : objs) {
		obj->Update();
		// シーンチェンジが起こったらブレイクしてfor文を抜ける
		if (SceneManager::GetSCFrag() == true) {
			SceneManager::SetSCFrag(false);
			return;
		}
	}
}

void GameObjectManager::HelperDraw(std::vector<std::unique_ptr<GameObject>>& objs) {
	for (const auto& obj : objs) {
		obj->Draw();
		// シーンチェンジが起こったらブレイクしてfor文を抜ける
		if (SceneManager::GetSCFrag() == true) {
			SceneManager::SetSCFrag(false);
			return;
		}
	}
}

void GameObjectManager::HelperChangeContainer(std::vector<std::unique_ptr<GameObject>>& objs) {
	for (auto it = objs.begin(); it != objs.end();) {

		GameObject* obj = it->get();

		const auto hdc = obj->GetHopeDrawContainer();
		const auto dc = obj->GetDrawContainer();
		const bool flag = obj->GetDrawContainerChangeFlag();

		if (flag == true && dc != hdc/* && dc != DrawContainer::Max*/)
		{
			// フラグをクリア
			obj->SetDrawContainerChangeFlag(hdc, false);
			obj->SetDrawContainer(hdc);

			{
				// unique_ptr の所有権を移動
				std::unique_ptr<GameObject> movingObj = std::move(*it);
				it = objs.erase(it);	// 現在のコンテナから削除

				switch (hdc)
				{
				case DrawContainer::Default:
					objects.push_back(std::move(movingObj));
					break;
				case DrawContainer::AbsFront:
					objects_Absfront.push_back(std::move(movingObj));
					break;
				case DrawContainer::UI:
					objects_UI.push_back(std::move(movingObj));
					break;
				case DrawContainer::Max:
					break;
				default:
					break;
				}
			}

			continue;
		}

		++it;
	}
}

std::vector<GameObject*> GameObjectManager::HelperFindTags(const std::vector<std::unique_ptr<GameObject>>& objs, const std::unordered_set<uint16_t>& ids) {

	std::vector<GameObject*> matchingObjects;
	matchingObjects.clear();

	for (const auto& obj : objs) { // objects をループで探索
		if (ids.contains(obj->GetID())) { // タグが一致するかチェック
			matchingObjects.push_back(obj.get()); // 一致するオブジェクトを追加
		}
	}

	return matchingObjects; // 一致するオブジェクトのベクターを返す
}

std::vector<GameObject*> GameObjectManager::HelperFindTagsOtherThan(const std::vector<std::unique_ptr<GameObject>>& objs, const std::unordered_set<uint16_t>& ids) {

	std::vector<GameObject*> matchingObjects;
	matchingObjects.clear();

	for (const auto& obj : objs) { // objects をループで探索
		if (!ids.contains(obj->GetID())) { // タグが一致するかチェック
			matchingObjects.push_back(obj.get()); // 一致するオブジェクトを追加
		}
	}

	return matchingObjects; // 一致するオブジェクトのベクターを返す
}

std::vector<GameObject*> GameObjectManager::HelperFindTag(const std::vector<std::unique_ptr<GameObject>>& objs, const uint16_t& id) {

	std::vector<GameObject*> matchingObjects;
	for (const auto& obj : objs) { // objects をループで探索
		if (obj->GetID() == id) { // タグが一致するかチェック
			matchingObjects.push_back(obj.get()); // 一致するオブジェクトを追加
		}
	}

	// ベクター配列内が空なら空のベクターを返す
	return matchingObjects; // 一致するオブジェクトのベクターを返す
}

GameObject* GameObjectManager::HelperFindInstanceID(const std::vector<std::unique_ptr<GameObject>>& objs, const uint32_t& id) {

	for (const auto& obj : objs) { // objects をループで探索
		if (obj->GetInstanceID() == id) { // タグが一致するかチェック
			return obj.get(); // 一致するオブジェクトを返す
		}
	}
	
	return nullptr; // 見つからなかった場合はnullptrを返す
}

void GameObjectManager::TransferAddObjects() {

	for (auto& p : temporaryContainer)
	{
		auto dc = p->GetDrawContainer();

		if (dc == DrawContainer::Default)
		{
			objects.push_back(std::move(p));
		}
		else if (dc == DrawContainer::Child)
		{
			child_Objects.push_back(std::move(p));
		}
		else if (dc == DrawContainer::UI)
		{
			objects_UI.push_back(std::move(p));
		}
		else if (dc == DrawContainer::AbsFront)
		{
			objects_Absfront.push_back(std::move(p));
		}
	}

	temporaryContainer.clear();
}

// どうやってオブジェクトをリストから削除するかを考える
void GameObjectManager::RemoveObject() {
	// 条件が合えばベクターの中の要素を削除する
	//objects.erase(
	//	std::remove_if(objects.begin(), objects.end(), [](const std::unique_ptr<GameObject>& obj) {
	//		return obj->GetDeleteFg(); // 削除フラグがtrueのオブジェクトを対象に
	//		}),
	//	objects.end());

	HelperRemoveObject(objects);
	HelperRemoveObject(child_Objects);
	HelperRemoveObject(objects_UI);
	HelperRemoveObject(objects_Absfront);
}

void GameObjectManager::RemoveTagObject(const std::string& tag) {

	uint16_t id = TagToIDGet(tag);

	HelperRemoveTagObject(objects, id);
	HelperRemoveTagObject(child_Objects, id);
	HelperRemoveTagObject(objects_UI, id);
	HelperRemoveTagObject(objects_Absfront, id);
}

// 更新
void GameObjectManager::Update() {
	TransferAddObjects();
	// 必要ならコンテナのサイズを増やす
	SizeUP();

	// 描画順を整える
	//DrawSort();
	HelperUpdate(objects);
	HelperUpdate(objects_UI);
	HelperUpdate(objects_Absfront);

	EventBusManager::Update();

	// インスタンスの削除処理
	RemoveObject();

	// コンテナ入れ替え
	ChangeContainer();

	//std::cout << static_cast<int>(objects_Absfront.size()) << std::endl;
	//std::cout << static_cast<int>(objects.size()) << std::endl;
}

// 描画
void GameObjectManager::Draw() {
	auto deviceContext = DirectXRender::GetDeviceContext();

	HelperDraw(objects);

	DirectXRender::SetDepthEnable(false);

	HelperDraw(objects_Absfront);

	DirectXRender::SetDepthEnable(true);

	HelperDraw(objects_UI);
}

void GameObjectManager::OtherThanClear() {
	// C++ 20で使えるコンテナの要素削除処理
	std::erase_if(objects, [](const std::unique_ptr<GameObject>& obj) {
		return obj->GetCarryOverFlag() == false;
		});

	std::erase_if(child_Objects, [](const std::unique_ptr<GameObject>& obj) {
		return obj->GetCarryOverFlag() == false;
		});

	std::erase_if(objects_UI, [](const std::unique_ptr<GameObject>& obj) {
		return obj->GetCarryOverFlag() == false;
		});

	std::erase_if(objects_Absfront, [](const std::unique_ptr<GameObject>& obj) {
		return obj->GetCarryOverFlag() == false;
		});
}

// オブジェクトを管理するリストを全て空にする
void GameObjectManager::ListClear() {
	objects.clear();
	child_Objects.clear();
	objects_UI.clear();
	objects_Absfront.clear();
};

void GameObjectManager::ChangeContainer() {
	HelperChangeContainer(objects);
	HelperChangeContainer(objects_Absfront);
	HelperChangeContainer(objects_UI);
}

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

	uint16_t id = TagToIDGet(tag);

	std::vector<GameObject*> matchingObjects;
	for (const auto& obj : objects) { // objects をループで探索
		if (obj->GetID() == id) { // タグが一致するかチェック
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

	uint16_t id = TagToIDGet(tag);

	std::vector<GameObject*> matchingObjects;
	for (const auto& obj : objects_UI) { // objects をループで探索
		if (obj->GetID() == id) { // タグが一致するかチェック
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

	uint16_t id = TagToIDGet(tag);

	std::vector<GameObject*> matchingObjects;
	for (const auto& obj : objects_Absfront) { // objects をループで探索
		if (obj->GetID() == id) { // タグが一致するかチェック
			matchingObjects.push_back(obj.get()); // 一致するオブジェクトを追加
		}
	}

	// ベクター配列内が空なら空のベクターを返す
	return matchingObjects; // 一致するオブジェクトのベクターを返す
}

std::vector<GameObject*> GameObjectManager::GameObjectFindAllTag(const std::string& tag) {

	uint16_t id = TagToIDGet(tag);

	std::vector<GameObject*> matchingObjects = HelperFindTag(objects, id);
	std::vector<GameObject*> childMatches = HelperFindTag(child_Objects, id);
	std::vector<GameObject*> uiMatches = HelperFindTag(objects_UI, id);
	std::vector<GameObject*> absfrontMatches = HelperFindTag(objects_Absfront, id);

	std::vector<GameObject*> result;

	result.reserve(
		matchingObjects.size() +
		childMatches.size() +
		uiMatches.size() +
		absfrontMatches.size()
	);

	if (result.capacity() == 0) {
		return result; // 空のベクターを返す
	}

	result.insert(result.end(), matchingObjects.begin(), matchingObjects.end());
	result.insert(result.end(), childMatches.begin(), childMatches.end());
	result.insert(result.end(), uiMatches.begin(), uiMatches.end());
	result.insert(result.end(), absfrontMatches.begin(), absfrontMatches.end());

	// ベクター配列内が空なら空のベクターを返す
	return result; // 一致するオブジェクトのベクターを返す
}

std::vector<GameObject*> GameObjectManager::GameObjectFindAllTag(const std::vector<std::string>& tags) {

	std::vector<uint16_t> ids;

	for (const auto& tag : tags) {

		uint16_t id = TagToIDGet(tag);
		ids.push_back(id);
	}

	std::vector<GameObject*> matchingObjects;
	std::vector<GameObject*> childMatches;
	std::vector<GameObject*> uiMatches;
	std::vector<GameObject*> absfrontMatches;

	for (const auto& id : ids) {
		matchingObjects = HelperFindTag(objects, id);
		childMatches = HelperFindTag(child_Objects, id);
		uiMatches = HelperFindTag(objects_UI, id);
		absfrontMatches = HelperFindTag(objects_Absfront, id);
	}

	std::vector<GameObject*> result;

	result.reserve(
		matchingObjects.size() +
		childMatches.size() +
		uiMatches.size() +
		absfrontMatches.size()
	);

	if (result.capacity() == 0) {
		return result; // 空のベクターを返す
	}

	result.insert(result.end(), matchingObjects.begin(), matchingObjects.end());
	result.insert(result.end(), childMatches.begin(), childMatches.end());
	result.insert(result.end(), uiMatches.begin(), uiMatches.end());
	result.insert(result.end(), absfrontMatches.begin(), absfrontMatches.end());

	// ベクター配列内が空なら空のベクターを返す
	return result; // 一致するオブジェクトのベクターを返す
}

std::vector<GameObject*> GameObjectManager::GameObjectFindAllTag(const std::unordered_set<std::string>& tags) {

	std::vector<uint16_t> ids;

	for (const auto& tag : tags) {

		uint16_t id = TagToIDGet(tag);
		ids.push_back(id);
	}

	std::vector<GameObject*> matchingObjects;
	std::vector<GameObject*> childMatches;
	std::vector<GameObject*> uiMatches;
	std::vector<GameObject*> absfrontMatches;

	for (const auto& id : ids) {

		std::vector<GameObject*> result1 = HelperFindTag(objects, id);
		matchingObjects.insert(matchingObjects.end(), result1.begin(), result1.end());
		std::vector<GameObject*> result2 = HelperFindTag(child_Objects, id);
		childMatches.insert(childMatches.end(), result2.begin(), result2.end());
		std::vector<GameObject*> result3 = HelperFindTag(objects_UI, id);
		uiMatches.insert(uiMatches.end(), result3.begin(), result3.end());
		std::vector<GameObject*> result4 = HelperFindTag(objects_Absfront, id);
		absfrontMatches.insert(absfrontMatches.end(), result4.begin(), result4.end());
	}

	std::vector<GameObject*> result;

	result.reserve(
		matchingObjects.size() +
		childMatches.size() +
		uiMatches.size() +
		absfrontMatches.size()
	);

	if (result.capacity() == 0) {
		return result; // 空のベクターを返す
	}

	result.insert(result.end(), matchingObjects.begin(), matchingObjects.end());
	result.insert(result.end(), childMatches.begin(), childMatches.end());
	result.insert(result.end(), uiMatches.begin(), uiMatches.end());
	result.insert(result.end(), absfrontMatches.begin(), absfrontMatches.end());

	// ベクター配列内が空なら空のベクターを返す
	return result; // 一致するオブジェクトのベクターを返す
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
