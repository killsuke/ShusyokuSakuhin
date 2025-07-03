// =========================================
// シングルトンを作成し、登録する関数
// ただし、シングルトンにするならば、
// シングルトンにしたいクラスのコンストラクタを
// private等に設置
// =========================================

#pragma once
#include <atomic>
#include <memory>
#include <unordered_map>
#include <mutex>

namespace mozc {
	namespace internal {
		void AddSingletonFinalizer(void (*finalizer)());
	}

	void FinalizeSingletons();


	template <typename T>
	class Singleton
	{
	public:
		// ここでインスタンスゲット
		static T* GetInstance() {
			// インスタンスが存在すればそのまま返す
			if (instance_ != nullptr) {
				return instance_;
			}

			// インスタンスがなければ作成
			if (instance_ == nullptr) {
				instance_ = new T();	// ここで新しくインスタンス作成
				internal::AddSingletonFinalizer(&Singleton<T>::Delete);	// 削除対象に登録
			}
			return instance_;
		}

		// インスタンス削除
		static void Delete() {
			delete instance_;
			instance_ = nullptr;
		}

	private:
		// インスタンスを留めておく
		static inline T* instance_ = nullptr;
	};

}