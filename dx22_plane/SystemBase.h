// Transformなどのコンポネントを使うSystemの基底クラス
// RemoveComponentを呼び出しやすくするために置いているのであまり重要ではない
#include <cstdint>
#pragma once
class SystemBase
{
protected:
	SystemBase() = default;
public:
	virtual ~SystemBase() = default;
	virtual void Update() = 0;
	virtual void RemoveComponent(uint32_t entity) = 0;	// 各システムがこれを実行する
};