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
	bool m_IsWinloseFlag = false; // 勝敗フラグ、true:勝ち、false:負け
	float m_RecordTime = 0.0f; // ドアの移動にかかる時間
	GameObject* m_DoorUpObj = nullptr;
	GameObject* m_DoorDownObj = nullptr;
	DirectX::XMFLOAT3 m_DoorUpFirstPos = DirectX::XMFLOAT3();
	DirectX::XMFLOAT3 m_DoorDownFirstPos = DirectX::XMFLOAT3();

	std::string m_NextSceneName = ""; // 次のシーン名

public:
	DoorFadeComponent(GameObject& obj);
	~DoorFadeComponent() = default;
	virtual void Update() override;

	void OpenDoor();
	void CloseDoor();

	void SetOpenCloseFlag(const bool flag) { m_IsOpenCloseFlag = flag; };
	bool GetOpenCloseFlag() const { return m_IsOpenCloseFlag; };

	void SetBootDoor(const bool boot) { m_IsDootDoor = boot; };

	void SetNextSceneName(const std::string& name) { m_NextSceneName = name; };

	void SetWinLoseFlag(const bool flag) { m_IsWinloseFlag = flag; };
	bool GetWinLoseFlag() const { return m_IsWinloseFlag; };
};