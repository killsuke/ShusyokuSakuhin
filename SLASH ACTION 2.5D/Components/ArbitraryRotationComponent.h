#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <vector>
#include "Helper/XMFLOAT_Helper.h"

enum class RollingPattern {
	DEFAULT_ROLLING,
	SWORD_SLASH
};

struct PosAndQuaternion
{
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMVECTOR quaternion = { 0.0f,0.0f,0.0f,1.0f };
};

class ArbitraryRotationComponent : public Component
{
private:

	DirectX::XMVECTOR m_ArbitraryAxis = { { 0.0f, 0.0f, 0.0f } };
	float m_StartAngle = 0.0f;
	float m_EndAngle = 0.0f;
	float m_RecordTime = 0.0f;
	float m_TargetRotationAmount = 0.0f;
	float m_StopTime = 0.0f;

	GameObject* m_CenterObject = nullptr; // 中心となるオブジェクト
	float m_rotationSpeed = 0.0f; // 回転速度
	float m_nowAngleRadian = 0.0f; // 現在の回転角度（ラジアン）
	float m_nowAngleDegree = 0.0f; // 現在の角度（ディグリー）
	float m_radius = 0.0f; // 回転半径
	float m_WarpRadian = 0.0f; // 開始２フレーム目にワープする角度
	bool m_clockwise = true; // 時計回りか反時計回りか
	bool m_rollingActive = false;	// 停止・再開フラグ
	bool m_flipRequested = false; // 反転要求フラグ
	bool m_IsFinished = false; // 回転完了フラグ

	int m_SwordSlashFrameCount = 0; // 剣振り用のフレームカウント
	int m_SampleDivisions = 0;	// 剣の軌跡のサンプルの分割数設定

	RollingPattern m_RollingPattern = RollingPattern::DEFAULT_ROLLING;

	DirectX::XMFLOAT3 m_initialOffset = {}; // 中心と回転するオブジェクトの差分
	DirectX::XMFLOAT3 m_LockRotation = {};
	std::vector<PosAndQuaternion> m_worldPosQuats = { };

public:

	ArbitraryRotationComponent(GameObject& obj);
	~ArbitraryRotationComponent() = default;

	void Update()override;

	void SetArbitraryAxis(const DirectX::XMFLOAT3& axis) {
		DirectX::XMVECTOR vAxis = DirectX::XMVectorSet(axis.x, axis.y, axis.z, 1.0f);
		m_ArbitraryAxis = DirectX::XMVector4Normalize(vAxis);
	};
	void SetStartAngle(const float angle) {
		m_StartAngle = angle;
	};
	void SetEndAngle(const float angle) { m_EndAngle = angle; };
	void SetStartAndEndAngle(const float angle1, const float angle2, const bool clockwise) {
		m_StartAngle = angle1;
		m_EndAngle = angle2;
		m_clockwise = clockwise;
		m_RecordTime = 0.0f;

		float delta = m_EndAngle - m_StartAngle;
		if (m_clockwise == true) {	// 時計回り
			if (delta > 0.0f) {
				delta -= 360.0f;
			}
		}
		else {						// 反時計回り
			if (delta < 0.0f) {
				delta += 360.0f;
			}
		}
		m_TargetRotationAmount = DirectX::XMConvertToRadians(std::abs(delta));
		m_nowAngleRadian = 0.0f;	// 累積角度は０からスタート
		m_RollingPattern = RollingPattern::DEFAULT_ROLLING;
	}

	// ワープ角度の設定
	// 時計回り・反時計回りで処理を分ける
	// ルールとして、360度以上の回転はしないかつ、間違えて反転しないようにする
	void SetStartAndEndAndWarpAngle(const float angle1, const float angle2, const float angle3, const bool clockwise) {
		m_StartAngle = angle1;
		m_EndAngle = angle2;
		m_clockwise = clockwise;
		m_RecordTime = 0.0f;
		m_SwordSlashFrameCount = 0;

		float delta = m_EndAngle - m_StartAngle;
		float deltaWarp = angle3 - m_StartAngle;

		if (m_clockwise == true) {	// 時計回り
			if (delta > 0.0f) {
				delta -= 360.0f;
				deltaWarp -= 360.0f;
			}
		}
		else {						// 反時計回り
			if (delta < 0.0f) {
				delta += 360.0f;
				deltaWarp += 360.0f;
			}
		}

		m_TargetRotationAmount = DirectX::XMConvertToRadians(std::abs(delta));
		m_WarpRadian = DirectX::XMConvertToRadians(std::abs(deltaWarp));
		m_nowAngleRadian = 0.0f;	// 累積角度は０からスタート
		m_RollingPattern = RollingPattern::SWORD_SLASH;
	}

	void SetCenterObject(GameObject* centerObj) { m_CenterObject = centerObj; }

	void SetRotationSpeed(float speed) { m_rotationSpeed = fabsf(speed); }
	void MakeInitialOffset(const DirectX::XMFLOAT3& parent, const DirectX::XMFLOAT3& child) {
		m_initialOffset = child - parent; // 子オブジェクトの位置から親オブジェクトの位置を引いて差分を計算

		const DirectX::XMFLOAT2 offset = DirectX::XMFLOAT2(m_initialOffset.x, m_initialOffset.y);

		m_radius = sqrtf(offset.x * offset.x + offset.y * offset.y);
	}
	void SetClockwise(const bool clockwise) { m_clockwise = clockwise; }	// 時計回りか反時計回りか
	void SetRollingActive(const bool active) { m_rollingActive = active; } // 回転の停止・再開
	void SetFlipRequested(const bool flip) { m_flipRequested = flip; } // 反転要求フラグの設定
	void SetLockAngle(const DirectX::XMFLOAT3& angle) { m_LockRotation = angle; };
	void SetSampleDivisions(const int sample) { m_SampleDivisions = sample; }
	void SetStopTime(const float time) { m_StopTime = time; }
	void ResetNowAngle_Radian() { m_nowAngleRadian = 0.0f; } // 角度をリセット（ラジアン）
	void ResetNowAngle_Degree() { m_nowAngleDegree = 0.0f; } // 角度をリセット（ディグリー）
	void ResetIsFinished() { m_IsFinished = false; } // 回転完了フラグをリセット

	void ResetVariables() {

		m_nowAngleDegree = 0.0f;
		m_nowAngleRadian = 0.0f;
		m_RecordTime = 0.0f;
		m_SampleDivisions = 0;
		m_IsFinished = false;
		m_rollingActive = true;
	}

	void RollingStop() { m_rollingActive = false; } // 回転を停止
	void RollingStart() { m_rollingActive = true; } // 回転を再開

	void AddNowAngle(float angle) {
		float newAngle = angle * (DirectX::XM_PI / 180.0f);
		m_nowAngleRadian += newAngle;
		m_nowAngleDegree += angle;
	};

	float GetNowAngleDegree() const { return m_nowAngleDegree; } // 現在の角度（度数法）を取得
	bool GetRollingActive()const { return m_rollingActive; };
	std::vector<PosAndQuaternion> GetWorldPosAndQuats()const { return m_worldPosQuats; };
	GameObject* GetCenterObject()const { return m_CenterObject; };
	float GetRollingSpeed()const {
		return m_rotationSpeed;
	}
	bool GetIsFinished()const { return m_IsFinished; };

	float NormalizeAngleDegree(float deg);
	float NormalizeAngleRadian(float rad);
	void SimulationMove();
	void DefaultRollingMove();
};