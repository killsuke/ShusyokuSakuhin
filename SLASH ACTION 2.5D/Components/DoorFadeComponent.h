#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <string>

class DoorFadeComponent : public Component
{
private:
	bool m_openCloseFlag = false;	// 開く：false、閉じる：true
	bool m_doorMoveEndFlag = true; // ドアの移動が終わったかどうかのフラグ
	bool m_bootDoor = false;	// 起動する
	bool m_winloseFlag = false; // 勝敗フラグ、true:勝ち、false:負け
	float timer = 0.0f; // ドアの移動にかかる時間
	GameObject* doorUp = nullptr;
	GameObject* doorDown = nullptr;
	DirectX::XMFLOAT3 doorUpFirstPos = DirectX::XMFLOAT3();
	DirectX::XMFLOAT3 doorDownFirstPos = DirectX::XMFLOAT3();

	std::string m_nextSceneName = ""; // 次のシーン名

public:
	DoorFadeComponent(GameObject& obj);
	~DoorFadeComponent() = default;
	virtual void Update() override;

	void OpenDoor();
	void CloseDoor();

	inline void SetOpenCloseFlag(const bool flag) { m_openCloseFlag = flag; };
	inline bool GetOpenCloseFlag() const { return m_openCloseFlag; };

	inline void SetBootDoor(const bool boot) { m_bootDoor = boot; };

	inline void SetNextSceneName(const std::string& name) { m_nextSceneName = name; };

	inline void SetWinLoseFlag(const bool flag) { m_winloseFlag = flag; };
	inline bool GetWinLoseFlag() const { return m_winloseFlag; };
};

