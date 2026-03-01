#pragma once
#include "RenderComponent.h"
#include "ArbitraryRotationComponent.h"
#include "TrailMakeComponent.h"
#include <vector>
#include <map>
#include <DirectXMath.h>

struct TrailPoint
{
	DirectX::XMFLOAT3 basePosition = {};
	DirectX::XMFLOAT3 tipPosition = {};
	DirectX::XMFLOAT3 centerLocalPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMVECTOR localQuat = { 0.0f,0.0f,0.0f,1.0f };
	float lifeTime = 0.0f;	// 生存時間（フェード用）
};

class TrailMakeComponent;

class TrailRenderComponent final : public RenderComponent
{
private:

	std::vector<TrailPoint> m_TrailPoints; // トレイルエフェクトのポイント
	std::vector<int> m_SampleDivisions;
	float m_TipOffset = 0.0f;	// 先端のオフセット
	float m_BaseOffset = 0.0f; // 根元のオフセット
	bool m_RightLeftFlag = true; // 右向きか左向きか
	int m_TrailCount = 0;
	int m_AverageSamplingNum = 0;
	int m_TrailDivisionsCount = 1;
	unsigned int m_TrailIndex = 0;
	TrailMakeComponent* m_TrailMakeComponent = nullptr;

public:

	TrailRenderComponent(GameObject& obj);
	~TrailRenderComponent() = default;

	void Update()override;

	void TrailCountUp();
	void TrailUpdate();
	void AddTrailPoints(const DirectX::XMFLOAT3& center, const DirectX::XMVECTOR& quaternion, const float trailSpeed);
	void ClearTrail() {
		m_TrailPoints.clear();
		m_TrailCount = 0;

	};

	void SetTrailPoint(const std::vector<PosAndQuaternion>& points);
	void SetTipPoint(const float tip) { m_TipOffset = tip; };
	void SetBasePoint(const float base) { m_BaseOffset = base; };
	void SetRightLeftFlag(const bool flag) { m_RightLeftFlag = flag; };
	void SetTrailDivisionsCount(const int count = 1) { m_TrailDivisionsCount = count; };

	void ResetTrailCount() { m_TrailCount = 0; };
	void RequestInversion() { m_Inversion = RightLeft::RIGHT; };
	void InversionEvent();
};