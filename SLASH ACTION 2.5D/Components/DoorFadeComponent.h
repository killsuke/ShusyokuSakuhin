#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <string>

class DoorFadeComponent : public Component
{
private:
	bool m_IsOpenCloseFlag = false;	// 開く：false、閉じる：true
	bool m_IsDoorMoveEndFlag = true; // ドアの移動が終わったかどうかのフラグ
	bool m_IsDootDoor = false;	// 起動する
	bool m_winloseFlag = false; // 勝敗フラグ、true:勝ち、false:負け
	float timer = 0.0f; // ドアの移動にかかる時間
	GameObject* m_DoorUpObj = nullptr;
	GameObject* m_DoorDownObj = nullptr;
	DirectX::XMFLOAT3 m_DoorUpFirstPos = DirectX::XMFLOAT3();
	DirectX::XMFLOAT3 m_DoorDownFirstPos = DirectX::XMFLOAT3();

	std::string m_nextSceneName = ""; // 次のシーン名

public:
	DoorFadeComponent(GameObject& obj);
	~DoorFadeComponent() = default;
	virtual void Update() override;

	void OpenDoor();
	void CloseDoor();

	inline void SetOpenCloseFlag(const bool flag) { m_IsOpenCloseFlag = flag; };
	inline bool GetOpenCloseFlag() const { return m_IsOpenCloseFlag; };

	inline void SetBootDoor(const bool boot) { m_IsDootDoor = boot; };

	inline void SetNextSceneName(const std::string& name) { m_nextSceneName = name; };

	inline void SetWinLoseFlag(const bool flag) { m_winloseFlag = flag; };
	inline bool GetWinLoseFlag() const { return m_winloseFlag; };
};

