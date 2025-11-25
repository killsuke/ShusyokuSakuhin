#pragma once
#include "Component.h"
#include <SimpleMath.h>
#include <vector>

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

	GameObject* m_CenterObject = nullptr; // 中心となるオブジェクト
	float m_rotationSpeed = 0.0f; // 回転速度
	float m_nowAngleRadian = 0.0f; // 現在の回転角度（ラジアン）
	float m_nowAngleDegree = 0.0f; // 現在の角度（ディグリー）
	float m_radius = 0.0f; // 回転半径
	bool m_clockwise = true; // 時計回りか反時計回りか
	bool m_rollingActive = false;	// 停止・再開フラグ
	bool m_flipRequested = false; // 反転要求フラグ
	bool m_IsFinished = false; // 回転完了フラグ

	DirectX::SimpleMath::Vector3 m_initialOffset = DirectX::SimpleMath::Vector3::Zero; // 中心と回転するオブジェクトの差分
	DirectX::XMFLOAT3 m_LockRotation = {};
	std::vector<PosAndQuaternion> m_worldPosQuats = { };

public:

	ArbitraryRotationComponent(GameObject& obj);
	~ArbitraryRotationComponent() = default;

	void Update()override;

	inline void SetArbitraryAxis(const DirectX::XMFLOAT3& axis) {
		DirectX::XMVECTOR vAxis = DirectX::XMVectorSet(axis.x, axis.y, axis.z, 1.0f);
		m_ArbitraryAxis = DirectX::XMVector4Normalize(vAxis);
	};
	inline void SetStartAngle(const float angle) {
		m_StartAngle = angle;
	};
	inline void SetEndAngle(const float angle) { m_EndAngle = angle; };
	inline void SetStartAndEndAngle(const float angle1, const float angle2, const bool clockwise) {
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
	}
	inline void SetCenterObject(GameObject* centerObj) { m_CenterObject = centerObj; }

	inline void SetRotationSpeed(float speed) { m_rotationSpeed = fabsf(speed); }
	inline void MakeInitialOffset(const DirectX::SimpleMath::Vector3& parent, const DirectX::SimpleMath::Vector3& child) {
		m_initialOffset = child - parent; // 子オブジェクトの位置から親オブジェクトの位置を引いて差分を計算

		m_radius = DirectX::SimpleMath::Vector2(m_initialOffset.x, m_initialOffset.y).Length();
	}
	inline void SetClockwise(const bool clockwise) { m_clockwise = clockwise; }	// 時計回りか反時計回りか
	inline void SetRollingActive(const bool active) { m_rollingActive = active; } // 回転の停止・再開
	inline void SetFlipRequested(const bool flip) { m_flipRequested = flip; } // 反転要求フラグの設定
	inline void SetLockAngle(const DirectX::XMFLOAT3& angle) { m_LockRotation = angle; };
	inline void ResetNowAngle_Radian() { m_nowAngleRadian = 0.0f; } // 角度をリセット（ラジアン）
	inline void ResetNowAngle_Degree() { m_nowAngleDegree = 0.0f; } // 角度をリセット（ディグリー）
	inline void ResetIsFinished() { m_IsFinished = false; } // 回転完了フラグをリセット
	
	inline void ResetVariables() {

		m_nowAngleDegree = 0.0f;
		m_nowAngleRadian = 0.0f;
		m_RecordTime = 0.0f;
		m_IsFinished = false;
		m_rollingActive = true;
	}
	
	inline void RollingStop() { m_rollingActive = false; } // 回転を停止
	inline void RollingStart() { m_rollingActive = true; } // 回転を再開

	inline void AddNowAngle(float angle) {
		float newAngle = angle * (DirectX::XM_PI / 180.0f);
		m_nowAngleRadian += newAngle;
		m_nowAngleDegree += angle;
	};

	inline float GetNowAngleDegree() const { return m_nowAngleDegree; } // 現在の角度（度数法）を取得
	inline bool GetRollingActive()const { return m_rollingActive; };
	inline std::vector<PosAndQuaternion> GetWorldPosAndQuats()const { return m_worldPosQuats; };
	inline GameObject* GetCenterObject()const { return m_CenterObject; };
	inline float GetRollingSpeed()const {

		ActiveState activeState = m_Object->GetActiveState();
		/*if (m_IsActiveFlag == false || activeState == ActiveState::ALL_STOP || activeState == ActiveState::UPDATE_STOP) {
			return 0.0f;
		}*/
		return m_rotationSpeed;
	}
	inline bool GetIsFinished()const { return m_IsFinished; };

	float NormalizeAngleDegree(float deg);
	float NormalizeAngleRadian(float rad);
	void SimulationMove();
};