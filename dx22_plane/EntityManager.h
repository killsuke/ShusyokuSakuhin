// エンティティを管理する（生成を主にする）

#pragma once
#include <cstdint>
#include <vector>
#include <unordered_set>

class EntityManager
{
private:
	uint32_t nextEntityID = 0;	// 次に作成されるエンティティのID
	std::vector<uint32_t> availableEntities;	// 再利用可能なIDのリスト
	std::unordered_set<uint32_t> entities;		// エンティティの存在を管理

public:
	EntityManager() = default;
	~EntityManager() = default;

	void Update() {};

	// エンティティを作成する
	uint32_t CreateEntity() {
		// 削除対象のエンティティが入っていた場合
		if (!availableEntities.empty()) {
			uint32_t id = availableEntities.back();	// 再利用するIDを取得
			availableEntities.pop_back();	// リストから削除（使用済みにする）
			entities.insert(id);	// エンティティをセットに追加
			return id;
		}
		// 削除対象で泣ければ、新しいIDを作成する
		uint32_t id = nextEntityID++;
		entities.insert(id);	// 登録
		return id;
	}

	// 存在する全てのエンティティを返す
	std::vector<uint32_t> GetAllEntities() const{
		return std::vector<uint32_t>(entities.begin(),entities.end());
	}

	// エンティティを削除する
	void DestroyEntity(uint32_t entity) {
		// 削除対象が存在するなら
		if (entities.find(entity) != entities.end()) {
			entities.erase(entity);	// エンティティリストから削除
			availableEntities.push_back(entity);	// 削除したIDを再利用リストに追加
		}
	}

	// 全てのエンティティを削除
	void DestroyAllEntities() {
		for (uint32_t entity : entities) {
			availableEntities.push_back(entity);	// IDを再利用リストに追加
		}
		entities.clear();	// クリア
	}

	// エンティティの存在を確認
	bool Exists(uint32_t entity) const{
		return entities.find(entity) != entities.end();	// 見つからなければfalseを返す
	}
};