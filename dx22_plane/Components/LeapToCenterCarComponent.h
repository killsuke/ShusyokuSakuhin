#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <string>

class LeapToCenterCarComponent : public Component
{
private:

	float leapSpeed = 150.0f;
	float leapTime = 0.0f;
	float leapDuration = 2.0f;
	int leapState = 0; // 0:待機、1:ジャンプ準備、2:ジャンプ中、3:着地後
	std::string m_NowCar = "";
	std::string m_NextCar = "";

public:
	LeapToCenterCarComponent(GameObject& obj);
	~LeapToCenterCarComponent() = default;

	void Update() override;

	inline void SetNowCar(const std::string& carName) { m_NowCar = carName; };
	inline void SetNextCar(const std::string& carName) { m_NextCar = carName; };
};