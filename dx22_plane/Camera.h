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
	DirectX::SimpleMath::Matrix		m_ViewMatrix{};

	float m_CameraDirection = 0; // カメラの方向

	float targetCount = 0.0f;	// ターゲットにしてから数える
	float targetSpeed_x = 0.0f;	// ターゲットからの位置、加速の時にのみ増やす
	float targetSpeed_z = 0.0f;	// ターゲットからの位置、加速の時にのみ増やす
	bool targetCarFg = false;	// 車をターゲットにするかどうかのフラグ

	float deltaCamera = 0.0f;

	DirectX::SimpleMath::Vector2 prevMouse = {};	// マウス位置記録

	DirectX::SimpleMath::Matrix projectionMtx2D = {};
	DirectX::SimpleMath::Matrix projectionMtx3D = {};
	DirectX::SimpleMath::Matrix projectionMtxSky = {};

	DirectX::SimpleMath::Matrix viewMtx2D = {};
	DirectX::SimpleMath::Matrix viewMtx3D = {};
	DirectX::SimpleMath::Matrix viewMtxSky = {};

	void Update2D();
	void Update3D();
	void UpdateSky();

public:

	Camera(GameObject& obj);
	~Camera() = default;

	void Init();
	void Update();
	void Draw();
	void Uninit();

	void SetCamera(int mode);	// カメラを設定

	DirectX::SimpleMath::Matrix GetProjMtx2D() const { return projectionMtx2D; };
	DirectX::SimpleMath::Matrix GetProjMtx3D() const { return projectionMtx2D; };
	DirectX::SimpleMath::Matrix GetProjMtxSky() const { return projectionMtx2D; };

	DirectX::SimpleMath::Matrix GetViewMtx2D() const { return viewMtx2D; };
	DirectX::SimpleMath::Matrix GetViewMtx3D() const { return viewMtx3D; };
	DirectX::SimpleMath::Matrix GetViewMtxSky() const { return viewMtxSky; };
};