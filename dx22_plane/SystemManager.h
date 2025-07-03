// Transformなどのコンポーネントに付くシステムを管理

#pragma once
#include "SystemBase.h"
#include <typeindex>
#include <memory>
#include <typeinfo>
#include <unordered_map>

class SystemManager
{
private:
	// システムを保持しておくためのコンテナ
	std::unordered_map<std::type_index, std::unique_ptr<SystemBase>> systems;

public:

	SystemManager() = default;
	~SystemManager() = default;

	// システムを追加する
	template <typename T,typename...Args>
	T& AddSystem(Args&&...args) {
		std::type_index type = typeid(T);	// 型情報を取得
		// すでに登録されている場合は既存のシステムを返す
		if (system.find(type) != systems.end()) {
			return *static_cast<T*>(system[type].get());
		}

		// 初めて追加する場合のみ、新しく作成する！
		systems[type] = std::make_unique<T>(std::forward<Args>(args)...);
		// デリファレンスをして参照として返す
		return *static_cast<T*>(systems[type].get());	// 型をキャストして返す
	}

	// 登録済みのSystemを取得する
	template <typename T>
	T& GetSystem() {
		return *static_cast<T*>(systems[typeid(T)].get());	// 型をキャストして返す
	}

	// システムの更新処理
	void Update() {
		for (auto& [type, system] : systems) {
			system->Update();
		}
	}

	// １つのエンティティに付くコンポーネント全てをRemoveComponentする
	void RemoveComponentFromAll(uint32_t entity) {
		for (auto& [type, system] : systems) {
			system->RemoveComponent(entity);	// 全システムからエンティティを削除（１つのエンティティに付くコンポーネント全てをRemoveComponentする）
		}
	}
};