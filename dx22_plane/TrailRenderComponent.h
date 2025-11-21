#pragma once
#include "Render.h"
#include "ArbitraryRotationComponent.h"
#include <vector>
#include <SimpleMath.h>

struct TrailPoint
{
	DirectX::SimpleMath::Vector3 basePosition = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 tipPosition = DirectX::SimpleMath::Vector3::Zero;
	float lifeTime = 0.0f;	// 生存時間（フェード用）
};

class TrailRenderComponent : public RenderComponent
{
private:

	std::vector<TrailPoint> m_TrailPoints; // トレイルエフェクトのポイント
	std::vector<int> m_SampleDivisions;
	DirectX::SimpleMath::Vector3 m_TargetBeforePos = DirectX::SimpleMath::Vector3::Zero; // 追従対象の前回位置
	float m_TipOffset = 0.0f;	// 先端のオフセット
	float m_BaseOffset = 0.0f; // 根元のオフセット
	float m_CumulativeSpeed = 0.0f;
	bool m_RightLeftFlag = true; // 右向きか左向きか
	int m_TrailCount = 0;
	int m_AverageSamplingNum = 0;

public:

	TrailRenderComponent(GameObject& obj);
	~TrailRenderComponent() = default;

	void Update()override;

	void TrailUpdate(const DirectX::SimpleMath::Vector3& base, const DirectX::SimpleMath::Vector3& tip);
	void ClearTrail() { 
		m_TrailPoints.clear(); 
		m_TrailCount = 0;
		m_CumulativeSpeed = 0.0f;
	};

	void SetTrailPoint(const std::vector<PosAndQuaternion>& points);
	void SetTipPoint(const float tip) { m_TipOffset = tip; };
	void SetBasePoint(const float base) { m_BaseOffset = base; };
	void SetRightLeftFlag(const bool flag) { m_RightLeftFlag = flag; };

	void ResetTrailCount() { m_TrailCount = 0; };
	void RequestInversion() { m_InversionFlag = true; };
	void InversionEvent();
};