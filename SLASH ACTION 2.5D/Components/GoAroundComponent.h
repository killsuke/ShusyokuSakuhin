#pragma once
#include "Component.h"
#include <SimpleMath.h>

class GoAroundComponent : public Component
{
private:
	const float m_deltaTime = 0.016f; // 更新間隔時間
	float m_rotationSpeed = 0.5f; // 回転速度
	float m_nowAngleRadian = 0.0f; // 現在の回転角度（ラジアン）
	float m_nowAngleDegree = 0.0f; // 現在の角度（ディグリー）
	float m_radius = 10.0f; // 回転半径
	float m_initialAngleRadian = 0.0f;	// 最初の角度（ラジアン）
	float m_initialAngleDegree = 0.0f;	// 最初の角度（ディグリー）
	bool m_clockwise = true; // 時計回りか反時計回りか
	bool m_rollingActive = true;	// 停止・再開フラグ
	bool m_flipRequested = false; // 反転要求フラグ

	GameObject* m_CenterObject = nullptr; // 中心となるオブジェクト
	DirectX::SimpleMath::Vector3 m_initialOffset = DirectX::SimpleMath::Vector3::Zero; // 中心と回転するオブジェクトの差分
	DirectX::SimpleMath::Vector3 m_RotationAxis = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 m_LockRotation = DirectX::SimpleMath::Vector3::Zero;

public:
	GoAroundComponent(GameObject& obj);
	~GoAroundComponent() = default;

	void Update() override;

	inline void SetCenterObject(GameObject* centerObj) { m_CenterObject = centerObj; }
	inline void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
	inline void SetInitialAngle(float angle) {
		m_initialAngleDegree = angle;
		m_initialAngleRadian = DirectX::XMConvertToRadians(angle);
	};	// 初期角度をラジアンに変換して設定
	inline void MakeInitialOffset(const DirectX::SimpleMath::Vector3& parent, const DirectX::SimpleMath::Vector3& child) {
		m_initialOffset = child - parent; // 子オブジェクトの位置から親オブジェクトの位置を引いて差分を計算

		m_radius = DirectX::SimpleMath::Vector2(m_initialOffset.x, m_initialOffset.y).Length();
	}

	inline void SetClockwise(const bool clockwise) { m_clockwise = clockwise; }	// 時計回りか反時計回りか
	inline void SetRollingActive(const bool active) { m_rollingActive = active; } // 回転の停止・再開
	inline void SetFlipRequested(const bool flip) { m_flipRequested = flip; } // 反転要求フラグの設定
	inline void SetRotationAxis(const DirectX::SimpleMath::Vector3& axis) { m_RotationAxis = axis; };
	inline void SetLockRotation_X(const float x) { m_LockRotation.x = x; };
	inline void SetLockRotation_Y(const float y) { m_LockRotation.y = y; };
	inline void SetLockRotation_Z(const float z) { m_LockRotation.z = z; };
	inline void ResetNowAngle_Radian() { m_nowAngleRadian = 0.0f; } // 角度をリセット（ラジアン）
	inline void ResetNowAngle_Degree() { m_nowAngleDegree = 0.0f; } // 角度をリセット（ディグリー）
	inline void RollingStop() { m_rollingActive = false; } // 回転を停止
	inline void RollingStart() { m_rollingActive = true; } // 回転を再開

	inline void AddNowAngle(float angle) {
		float newAngle = angle * (DirectX::XM_PI / 180.0f);
		m_nowAngleRadian += newAngle;
		m_nowAngleDegree += angle;
	};

	inline float GetNowAngleDegree() const { return m_nowAngleDegree; } // 現在の角度（度数法）を取得
};