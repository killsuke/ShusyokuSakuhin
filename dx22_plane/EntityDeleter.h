// エンティティとそこについているコンポーネントを削除

#pragma once
#include <unordered_set>
#include "EntityManager.h"
#include "SystemManager.h"
#include "Singleton.h"


class EntityDeleter
{
private:
	std::unordered_set<uint32_t> toBeDeleted;	// 削除予定のエンティティ
	EntityManager* entityManager = mozc::Singleton<EntityManager>::GetInstance();	// エンティティを管理する
	SystemManager* systemManager = mozc::Singleton<SystemManager>::GetInstance();	// システムを管理する

public:
	EntityDeleter() = default;
	~EntityDeleter() = default;

	// 更新処理
	void Update() {
		ProcessDeletions();	// フラグが立ったオブジェクトを削除
	};

	// 削除フラグを立てる
	void MarkForDeletion(const uint32_t entity) {
		toBeDeleted.insert(entity);		// 削除予定のコンテナに追加
	}

	// フラグが立っているエンティティを一括削除
	void ProcessDeletions() {
		for (uint32_t entity : toBeDeleted) {
			systemManager->RemoveComponentFromAll(entity);	// 全システムからコンポーネントを削除
			entityManager->DestroyEntity(entity);	// エンティティを削除
		}
		toBeDeleted.clear();	// 配列の中身をクリアする
	}

	// 全てのエンティティとそのエンティティが持っているコンポーネントを全て削除
	void AllDelete() {
		entityManager->DestroyAllEntities();
		//systemManager.RemoveComponentFromAll();
	}

	// 全てのエンティティに付くコンポーネント全てをRemoveComponentする
	void RemoveAllEntitiesAndComponents() {
		// 全てのエンティティのリストをコピー
		std::vector<uint32_t> allEntities(entityManager->GetAllEntities());

		// 全てのエンティティのComponentを削除
		for (uint32_t entity : allEntities) {
			systemManager->RemoveComponentFromAll(entity);	// 各Systemからentityが持つ全てのComponentを削除
		}

		// 全てのエンティティを削除
		entityManager->DestroyAllEntities();
	}
};