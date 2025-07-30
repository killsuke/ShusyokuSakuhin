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
//	DirectX::SimpleMath::Matrix		m_ViewMatrix{};

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

	void Init();
	void Update();
	void Draw();
	void Uninit();

//	void SetCamera(int mode);	// カメラを設定

	inline void SetTarget(DirectX::SimpleMath::Vector3& target) { m_Target = target; };
};