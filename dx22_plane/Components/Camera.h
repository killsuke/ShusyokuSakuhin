#pragma once

#include	<SimpleMath.h>
#include "GameObject.h"
#include "Component.h"

#define CARCAMERA (0.01745f)

//-----------------------------------------------------------------------------
//Cameraクラス
//-----------------------------------------------------------------------------
class Camera : public Component {
private:

	DirectX::SimpleMath::Vector3	m_Target{};

	DirectX::XMMATRIX		m_ViewMatrix2D{};
	DirectX::XMMATRIX		m_ProjectionMatrix2D{};

	DirectX::XMMATRIX		m_ViewMatrix3D{};
	DirectX::XMMATRIX		m_ProjectionMatrix3D{};

	DirectX::XMMATRIX		m_ViewMatrixSky{};
	DirectX::XMMATRIX		m_ProjectionMatrixSky{};

	//float m_CameraDirection = 0; // カメラの方向

	//float targetCount = 0.0f;	// ターゲットにしてから数える
	//float targetSpeed_x = 0.0f;	// ターゲットからの位置、加速の時にのみ増やす
	//float targetSpeed_z = 0.0f;	// ターゲットからの位置、加速の時にのみ増やす
	//bool targetCarFg = false;	// 車をターゲットにするかどうかのフラグ

	//float deltaCamera = 0.0f;


	DirectX::SimpleMath::Vector2 prevMouse = {};	// マウス位置記録

	void Update2D();
	void Update3D();
	void UpdateSky();

public:

	Camera(GameObject& obj);
	~Camera() = default;

	void Update() override;

	inline void SetTarget(const DirectX::SimpleMath::Vector3& target) { m_Target = target; };
	inline void AddTarget(const DirectX::SimpleMath::Vector3& target) { m_Target += target; };
	inline DirectX::SimpleMath::Vector3 GetTarget() const { return m_Target; }

	// 転置していない状態のモノを返す
	inline DirectX::XMMATRIX GetView2D() const { return m_ViewMatrix2D; };
	inline DirectX::XMMATRIX GetView3D() const { return m_ViewMatrix3D; };

	inline DirectX::XMMATRIX GetProj2D() const { return m_ProjectionMatrix2D; };
	inline DirectX::XMMATRIX GetProj3D() const { return m_ProjectionMatrix3D; };
};