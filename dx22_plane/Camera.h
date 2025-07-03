#pragma once

#include	<SimpleMath.h>
#define CARCAMERA (0.01745f)

//-----------------------------------------------------------------------------
//Cameraクラス
//-----------------------------------------------------------------------------
class Camera {
private:
	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	DirectX::SimpleMath::Vector3	m_Target{};
	DirectX::SimpleMath::Matrix		m_ViewMatrix{};

	//const GolfBall& m_GolfBall;	// ゴルフボールの参照
	float m_CameraDirection = 0; // カメラの方向

	float targetCount = 0.0f;	// ターゲットにしてから数える
	float targetSpeed_x = 0.0f;	// ターゲットからの位置、加速の時にのみ増やす
	float targetSpeed_z = 0.0f;	// ターゲットからの位置、加速の時にのみ増やす
	bool targetCarFg = false;	// 車をターゲットにするかどうかのフラグ

	float deltaCamera = 0.0f;

	DirectX::SimpleMath::Vector2 prevMouse = {};	// マウス位置記録

public:

	// コンストラクタ
	//Camera(const GolfBall& golfBall);

	void Init();
	void Update();
	void Draw();
	void Uninit();

	void SetCamera(int mode);	// カメラを設定
};