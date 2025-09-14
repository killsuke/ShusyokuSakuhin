#pragma once
#include "Component.h"
#include "SimpleMath.h"
#include <string>

class DoorFadeComponent : public Component
{
private:
	bool m_openCloseFlag = false;	// 開く：false、閉じる：true
	bool m_doorMoveEndFlag = true; // ドアの移動が終わったかどうかのフラグ
	bool m_bootDoor = false;	// 起動する
	float timer = 0.0f; // ドアの移動にかかる時間
	const float deltaTime = 0.016f; // 仮のデルタタイム
	GameObject* doorUp = nullptr;
	GameObject* doorDown = nullptr;
	DirectX::SimpleMath::Vector3 doorUpFirstPos = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 doorDownFirstPos = DirectX::SimpleMath::Vector3::Zero;

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
};

