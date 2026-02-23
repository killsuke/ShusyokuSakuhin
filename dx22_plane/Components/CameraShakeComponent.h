#pragma once
#include "Component.h"
#include <DirectXMath.h>

struct ShakeStatus {
	float power = 0.0f;
	float speed = 0.0f;
	float time = 0.0f;
};

enum class ShakeType {
	WIDTH,
	HEIGHT,
	DEPTH,
	RANDOM_2D,
	RANDOM_DEPTH,
	RANDOM_DEPTH_ATTENUATION,
	MAX
};

class CameraShakeComponent final : public Component
{
private:
	float m_RecordTime = 0.0f;
	float m_RequestTime = 0.0f;	// —h‚ê‚ÌŠÔ
	float m_ShakePower = 0.0f;	// —h‚ê‚Ì‘å‚«‚³iü”g”j
	float m_ShakeSpeed = 0.0f;	// —h‚ê‚Ì‘¬‚³iU•j

	DirectX::XMVECTOR m_ShakeVector = { 0.0f,0.0f,0.0f,0.0f };	// ƒ‰ƒ“ƒ_ƒ€‚È•ûŒü‚É—h‚ç‚·‚½‚ß‚Ég‚¤
	DirectX::XMVECTOR m_PrevShakeOffset = { 0.0f,0.0f,0.0f,0.0f };	// ƒ‰ƒ“ƒ_ƒ€‚È•ûŒü‚É—h‚ç‚·‚½‚ß‚Ég‚¤

	DirectX::XMFLOAT3 WidthShake(const DirectX::XMMATRIX& view)const;
	DirectX::XMFLOAT3 HeightShake(const DirectX::XMMATRIX& view)const;
	DirectX::XMFLOAT3 DepthShake(const DirectX::XMMATRIX& view)const;
	DirectX::XMFLOAT3 RandomShake2D(const DirectX::XMMATRIX& view);
	DirectX::XMFLOAT3 RandomShakeDepth(const DirectX::XMMATRIX& view);
	DirectX::XMFLOAT3 RandomShakeDepthAttenuation(const DirectX::XMMATRIX& view);

	ShakeType m_ShakeType = ShakeType::RANDOM_2D;

public:

	CameraShakeComponent(GameObject& obj);
	~CameraShakeComponent() = default;

	void Update() override;

	void ShakingPreparation(const float power,const float speed, const float time) {
		m_ShakePower = power;
		m_ShakeSpeed = speed;
		m_RequestTime = time;
	};

	void ShakingPreparation(const ShakeStatus& status) {
		m_ShakePower = status.power;
		m_ShakeSpeed = status.speed;
		m_RequestTime = status.time;
	};

	void SetShakeType(const ShakeType& type) {
		m_ShakeType = type;
	};
};