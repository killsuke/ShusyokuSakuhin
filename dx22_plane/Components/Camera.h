#pragma once

#include "GameObject.h"
#include "Transform.h"

#define CARCAMERA (0.01745f)

//-----------------------------------------------------------------------------
//Cameraクラス
//-----------------------------------------------------------------------------
class Camera : public Component {
private:

	DirectX::XMFLOAT3		m_Target = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX		m_ViewMatrix2D = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX		m_ProjectionMatrix2D = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX		m_ViewMatrix3D = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX		m_ProjectionMatrix3D = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX		m_ViewMatrixSky = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX		m_ProjectionMatrixSky = DirectX::XMMatrixIdentity();

	//float m_CameraDirection = 0; // カメラの方向

	//float targetCount = 0.0f;	// ターゲットにしてから数える
	//float targetSpeed_x = 0.0f;	// ターゲットからの位置、加速の時にのみ増やす
	//float targetSpeed_z = 0.0f;	// ターゲットからの位置、加速の時にのみ増やす
	//bool targetCarFg = false;	// 車をターゲットにするかどうかのフラグ

	//float deltaCamera = 0.0f;


	DirectX::XMFLOAT2 prevMouse = DirectX::XMFLOAT2(0.0f, 0.0f);	// マウス位置記録

	void Update2D();
	void Update3D();
	void UpdateSky();

public:

	Camera(GameObject& obj);
	~Camera() = default;

	void Update() override;

	inline void SetTarget(const DirectX::XMFLOAT3& target) { m_Target = target; };
	inline void AddTarget(const DirectX::XMFLOAT3& target) { m_Target += target; };
	inline DirectX::XMFLOAT3 GetTarget() const { return m_Target; }

	// 転置していない状態のモノを返す
	inline DirectX::XMMATRIX GetView2D() const { return m_ViewMatrix2D; };
	inline DirectX::XMMATRIX GetView3D() const { return m_ViewMatrix3D; };

	inline DirectX::XMMATRIX GetProj2D() const { return m_ProjectionMatrix2D; };
	inline DirectX::XMMATRIX GetProj3D() const { return m_ProjectionMatrix3D; };
};