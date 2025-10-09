#include "DirectXRender.h"
#include "Camera.h"
#include "Application.h"
#include "input.h"
#include "Transform.h"
#include <Windows.h>

using namespace DirectX::SimpleMath;
using namespace std;

Camera::Camera(GameObject& obj) : Component(obj)
{
	m_sortNum = ComponentTypeManager::GetID_FromName("CAMERA"); // ソート番号を設定

	auto transform = p_object->GetComponent<TransformComponent>();

	if (transform != nullptr) {
		auto pos = transform->GetPosition();

		if (pos == m_Target) {
			MessageBoxA(nullptr, "カメラのポジションとターゲットが同じです。", "エラー", MB_OK | MB_ICONERROR);
			assert(false); // もしくは throw などで止める
		}
	}

	// 初期化
	Init();
}

//=======================================
//初期化処理
//=======================================
void Camera::Init()
{
	m_Target = Vector3(0.0f, 0.0f, 0.0f);
	//	m_CameraDirection = 3.14f;
}


//=======================================
//更新処理
//=======================================
void Camera::Update()
{
	auto transform = p_object->GetComponent<TransformComponent>();

	if (transform != nullptr) {
		auto pos = transform->GetPosition();

		if (pos == m_Target) {
			MessageBoxA(nullptr, "カメラのポジションとターゲットが同じです。", "エラー", MB_OK | MB_ICONERROR);
			assert(false); // もしくは throw などで止める
		}

		m_Target.x = pos.x;
		m_Target.y = pos.y;

		Vector3 vec3 = {};	// 初期化
		if (Input::GetKeyPress(VK_J)) {
			vec3.x -= 1.0f;
		}
		if (Input::GetKeyPress(VK_K)) {
			vec3.z -= 1.0f;
		}
		if (Input::GetKeyPress(VK_L)) {
			vec3.x += 1.0f;
		}
		if (Input::GetKeyPress(VK_I)) {
			vec3.z += 1.0f;
		}



		if (Input::GetKeyPress(VK_UP)) {
			vec3.y += 1.0f;
		}
		if (Input::GetKeyPress(VK_DOWN)) {
			vec3.y -= 1.0f;
		}

		// 座標更新
		/*transform->AddPosition(vec3);
		m_Target += vec3;*/

		// 視野角をいじって加速の時だけカメラを引く、FOV



		// ゴルフボールの位置を取得
		/*Vector3 ballPos = m_GolfBall.GetPosition();*/

		//vector<CarModel*>ballpt = Game::GetInstance()->GetObjects<CarModel>();
		//if (ballpt.size() > 0) {
		//	// 加速床を踏んでいる時
		//	if (ballpt[0]->GetAccelerationFg()) {
		//		targetSpeed_x += ballpt[0]->GetRotation().x * 3.0f;
		//		targetSpeed_z += ballpt[0]->GetRotation().z * 3.0f;
		//	}
		//	else {
		//		if (targetSpeed_x > 0.0f) {
		//			targetSpeed_x -= 0.5f;
		//		}
		//		if (targetSpeed_z > 0.0f) {
		//			targetSpeed_z -= 0.5f;
		//		}
		//	}

		//	// これ以上カメラを引かない
		//	if (targetSpeed_x > 200.0f) {
		//		targetSpeed_x = 200.0f;
		//	}

		//	if (targetSpeed_z > 200.0f) {
		//		targetSpeed_z = 200.0f;
		//	}

		//	if (targetSpeed_x < 0.0f) {
		//		targetSpeed_x = 0.0f;
		//	}
		//	if (targetSpeed_z < 0.0f) {
		//		targetSpeed_z = 0.0f;
		//	}

		//	Vector3 ballAngle = ballpt[0]->GetRotation();

		//	if (!ballpt[0]->GetDriftFg()) {
		//		//	m_CameraDirection = 0.0f;

		//		if (!ballpt[0]->GetRightLeft()) {
		//			if (targetCount > 0.0f) {
		//				targetCount -= 0.05f;
		//			}
		//		}
		//		else {
		//			if (targetCount < 0.0f) {
		//				targetCount += 0.05f;
		//			}
		//		}

		//		if (targetCount < 0.1f && targetCount > -0.1f) {
		//			targetCount = 0.0f;
		//		}
		//	}
		//	else {
		//		if (!ballpt[0]->GetRightLeft()) {
		//			targetCount += 0.05f;
		//			if (targetCount > 0.4f) {
		//				targetCount = 0.4f;
		//			}
		//		}
		//		else {
		//			targetCount -= 0.05f;
		//			if (targetCount < -0.4f) {
		//				targetCount = -0.4f;
		//			}
		//		}
		//	}




		//	/*if (m_CameraDirection != ballAngle.y * CARCAMERA && !ballpt[0]->GetDriftFg()) {
		//		deltaCamera = m_CameraDirection - ballAngle.y * CARCAMERA;
		//	}

		//	if (deltaCamera > 0.0f) {
		//		deltaCamera -= 0.5f;
		//	}
		//	else if(deltaCamera < 0.0f) {
		//		deltaCamera += 0.5f;
		//	}*/

		//	Vector3 ballPos = ballpt[0]->GetPosition();
		//	// カメラの位置を更新
		//	m_Position.x = ballPos.x + sin(m_CameraDirection) * 40.0f;
		//	m_Position.y = ballPos.y + 8.0f;
		//	m_Position.z = ballPos.z + cos(m_CameraDirection) * 40.0f;

		//	// カメラの注視点を更新
		//	m_Target = ballPos;

		//	// 左右キーでカメラ回転
		//	if (Input::GetKeyPress(VK_LEFT) || Input::GetKeyPress(VK_RIGHT)) {
		//		if (Input::GetKeyPress(VK_LEFT)) {
		//			m_CameraDirection += 0.02f;

		//		}
		//		if (Input::GetKeyPress(VK_RIGHT)) {
		//			m_CameraDirection -= 0.02f;

		//		}
		//	}
		//	else {
		//		//// コントローラー入力
		//		//auto speed_x = Input::GetLeftAnalogStick().x * 8.0f;
		//		//auto speed_y = Input::GetLeftAnalogStick().y * 8.0f;

		//		//if (speed_x > 1.0f || speed_x < -1.0f) {
		//		//	targetCarFg = true;
		//		//}
		//		//else {
		//		//	targetCarFg = false;
		//		//}

		//		//if (targetCarFg) {
		//		//	targetCount += 0.00001f;
		//		//}
		//		//else {
		//		//	targetCount = 0.0f;
		//		//}

		//		//// ちょっとだけ横を見せたいが、上手くいくようにする
		//		//if (targetCount >= 0.00001f && targetCount < 0.00045f) {
		//		//	Vector3 ballAngle = ballpt[0]->GetRotaiton();
		//		//	m_CameraDirection = ballAngle.y * 0.017f + targetCount;
		//		//}
		////		else {
		//			// 角度の数字は掛け算より足し算でやる方が良いかも？
		//			// ちゃんとラジアンで計算する？
		//			// ドリフトの時に横からちょっと見えるようにする

		////		}
		//	}
		//	m_CameraDirection = ballAngle.y * CARCAMERA + targetCount;
		//}
		//if (Input::GetKeyPress(VK_UP)) {
		//	m_Position.z += 0.2f;
		//}
		//if (Input::GetKeyPress(VK_DOWN)) {
		//	m_Position.z -= 0.2f;
		//}

		/*float px = m_Position.x - psx;
		float pz = m_Position.z - psz;*/

		//m_Rotation.x += 0.01f;
		//m_Rotation.z += 0.01f;

		//static float posx = 0.0f;
		//float px = psx + 50.0f * cos(m_Rotation.x);
		//float pz = psz + 50.0f * sin(m_Rotation.z);
		//m_Position = DirectX::SimpleMath::Vector3(px, 20.0f, pz);
		////	m_Target = DirectX::SimpleMath::Vector3(posx, 0.0f, 0.0f);	// ターゲットとカメラの位置が一緒じゃダメ
		//posx += 1.1f;


		auto rot = transform->GetRotation();
		auto nowPos = transform->GetPosition();

		// マウスの座標を取得
		Vector2 mouseVec2 = Input::GetMousePositionNormalize();

		if (Input::MouseLeftPress() == true) {

			// マウス座標の差分を計算
			float delta_X = mouseVec2.x - prevMouse.x;
			float delta_Y = mouseVec2.y - prevMouse.y;

			// 感度（スピード）をかける
			const float rotationSpeed = 0.1f;
			float rot_x = 0.0f;
			float rot_y = 0.0f;
			rot_y -= delta_X * rotationSpeed;
			rot_x += delta_Y * rotationSpeed;

			transform->AddRotation({ rot_x, rot_y, rot.z });

			auto rot2 = transform->GetRotation();

			rot2.x = std::clamp(rot2.x, -85.0f, 85.0f);

			// ラジアンに変換
			float PitchRadians = DirectX::XMConvertToRadians(rot2.x); // X軸回転
			float YawRadians = DirectX::XMConvertToRadians(rot2.y);     // Y軸回転
			float RollRadians = DirectX::XMConvertToRadians(rot2.z);   // Z軸回転

			// クォータニオンを構成（ピッチ・ヨーを個別に回転軸に適用）
			DirectX::XMVECTOR qPitch = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), PitchRadians);
			DirectX::XMVECTOR qYaw = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), YawRadians);

			// 合成（順番注意：Yawを先にかけると、カメラが左右中心に回る）
			DirectX::XMVECTOR qRotation = DirectX::XMQuaternionMultiply(qPitch, qYaw);

			// 回転行列に変換
			DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationQuaternion(qRotation);

			//// オイラー角の計算
			//// 回転行列作成
			//DirectX::XMVECTOR rotVec = DirectX::XMVectorSet(PitchRadians, YawRadians, RollRadians, 0.0f);
			//DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(rotVec);

			// 前方ベクトル回転
			DirectX::XMVECTOR forward = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), rotMat);

			// ターゲット計算
			m_Target = DirectX::XMVectorAdd(nowPos, forward);
		}
		// ビュー変換後列作成
		//Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		//DirectX::SimpleMath::Matrix viewMatrix = DirectX::XMMatrixLookAtLH(pos, m_Target, up); // 左手系にした　20230511 by suzuki.tomoki

		// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
		// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
		// このコードは確認テストのために残す
		// m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

		// このフレームのマウス位置を次回に備えて保存
		prevMouse.x = mouseVec2.x;
		prevMouse.y = mouseVec2.y;

		//	DirectXRender::SetViewMatrix3D(&viewMatrix);

			// SRT情報更新
	//	transform->MakeWorldMatrix();

		//プロジェクション行列の生成
		//constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角

		//float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
		//float nearPlane = 1.0f;       // ニアクリップ
		//float farPlane = 1000.0f;      // ファークリップ

		////プロジェクション行列の生成
		//Matrix projectionMatrix;
		//projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki
		// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
		// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
		// このコードは確認テストのために残す
		// projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	//	DirectXRender::SetProjectionMatrix3D(&projectionMatrix);

		Update2D();
		Update3D();
		UpdateSky();
		DirectXRender& dxRender = DirectXRender::GetInstance();
		dxRender.GPU_UpdateViewAndProj();
	}
}

void Camera::Update2D() {
	// ビュー変換後列作成
	Vector3 pos = { 0.0f,0.0f,-10.0f };
	Vector3 tgt = { 0.0f,0.0f,1.0f };
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	DirectX::SimpleMath::Matrix viewMatrix = DirectX::XMMatrixLookAtLH(pos, tgt, up);
	//	viewMtx2D = DirectX::XMMatrixTranspose(viewMatrix);

	m_ViewMatrix2D = viewMatrix;

	DirectXRender& dxRender = DirectXRender::GetInstance();
	dxRender.SetViewMatrix2D(&viewMatrix);

	// プロジェクション行列の生成
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 1000.0f;      // ファークリップ

	Matrix projectionMatrix = DirectX::XMMatrixOrthographicLH(static_cast<float>(Application::GetWidth()), static_cast<float>(Application::GetHeight()), nearPlane, farPlane);

	m_ProjectionMatrix2D = projectionMatrix;

	dxRender.SetProjectionMatrix2D(&projectionMatrix);
}

void Camera::Update3D() {
	auto transform = p_object->GetComponent<TransformComponent>();
	auto pos = transform->GetPosition();

	// ビュー変換後列作成
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Matrix viewMatrix;
	viewMatrix = DirectX::XMMatrixLookAtLH(pos, m_Target, up); // 左手系にした　20230511 by suzuki.tomoki
	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

	m_ViewMatrix3D = viewMatrix;

	DirectXRender& dxRender = DirectXRender::GetInstance();
	dxRender.SetViewMatrix3D(&viewMatrix);

	//プロジェクション行列の生成
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角

	float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 1000.0f;      // ファークリップ

	//プロジェクション行列の生成
	Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki
	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	m_ProjectionMatrix3D = projectionMatrix;

	dxRender.SetProjectionMatrix3D(&projectionMatrix);
}

void Camera::UpdateSky() {
	// ビュー変換後列作成
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	m_ViewMatrixSky = DirectX::XMMatrixLookAtLH(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f), up); // 左手系にした　20230511 by suzuki.tomoki
	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

	DirectXRender& dxRender = DirectXRender::GetInstance();
	dxRender.SetViewMatrixSkyDome(&m_ViewMatrixSky);

	//プロジェクション行列の生成
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角

	float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 1000.0f;      // ファークリップ

	//プロジェクション行列の生成

	m_ProjectionMatrixSky = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki

	dxRender.SetProjectionMatrixSkyDome(&m_ProjectionMatrixSky);

	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
}

//=======================================
//描画処理
//=======================================
void Camera::Draw()
{
	//// マウスの座標を取得
	//Vector2 mouseVec2 = Input::GetMousePositionNormalize();

	//if (Input::MouseLeftPress() == true) {

	//	// マウス座標の差分を計算
	//	float delta_X = mouseVec2.x - prevMouse.x;
	//	float delta_Y = mouseVec2.y - prevMouse.y;

	//	// 感度（スピード）をかける
	//	const float rotationSpeed = 0.1f;
	//	m_Rotation.y -= delta_X * rotationSpeed;
	//	m_Rotation.x += delta_Y * rotationSpeed;

	//	// ラジアンに変換
	//	float PitchRadians = DirectX::XMConvertToRadians(m_Rotation.x); // X軸回転
	//	float YawRadians = DirectX::XMConvertToRadians(m_Rotation.y);     // Y軸回転
	//	float RollRadians = DirectX::XMConvertToRadians(m_Rotation.z);   // Z軸回転

	//	// クォータニオンを構成（ピッチ・ヨーを個別に回転軸に適用）
	//	DirectX::XMVECTOR qPitch = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), PitchRadians);
	//	DirectX::XMVECTOR qYaw = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), YawRadians);

	//	// 合成（順番注意：Yawを先にかけると、カメラが左右中心に回る）
	//	DirectX::XMVECTOR qRotation = DirectX::XMQuaternionMultiply(qPitch, qYaw);

	//	// 回転行列に変換
	//	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationQuaternion(qRotation);

	//	//// オイラー角の計算
	//	//// 回転行列作成
	//	//DirectX::XMVECTOR rotVec = DirectX::XMVectorSet(PitchRadians, YawRadians, RollRadians, 0.0f);
	//	//DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(rotVec);

	//	// 前方ベクトル回転
	//	DirectX::XMVECTOR forward = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), rotMat);

	//	// ターゲット計算
	//	m_Target = DirectX::XMVectorAdd(m_Position, forward);
	//}
	//// ビュー変換後列作成
	//Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	//m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, m_Target, up); // 左手系にした　20230511 by suzuki.tomoki

	//// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	//// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	//// このコードは確認テストのために残す
	//// m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

	//// このフレームのマウス位置を次回に備えて保存
	//prevMouse.x = mouseVec2.x;
	//prevMouse.y = mouseVec2.y;

	//DirectXRender::SetViewMatrix(&m_ViewMatrix);

	////プロジェクション行列の生成
	//constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角

	//float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
	//float nearPlane = 1.0f;       // ニアクリップ
	//float farPlane = 1000.0f;      // ファークリップ

	////プロジェクション行列の生成
	//Matrix projectionMatrix;
	//projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki
	//// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	//// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	//// このコードは確認テストのために残す
	//// projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	//DirectXRender::SetProjectionMatrix(&projectionMatrix);
}


//=======================================
//終了処理
//=======================================
void Camera::Uninit()
{

}

// ================================
// プロジェクション行列を指定
// ================================
//void Camera::SetCamera(int mode) {
//	// 3D
//	if (mode == 0) {
//		// ビュー変換後列作成
//		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
//		m_ViewMatrix = DirectX::XMMatrixLookAtLH(m_Position, m_Target, up); // 左手系にした　20230511 by suzuki.tomoki
//		// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
//		// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
//		// このコードは確認テストのために残す
//		// m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					
//
//		DirectXRender::SetViewMatrix(&m_ViewMatrix);
//
//		//プロジェクション行列の生成
//		constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角
//
//		float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
//		float nearPlane = 1.0f;       // ニアクリップ
//		float farPlane = 1000.0f;      // ファークリップ
//
//		//プロジェクション行列の生成
//		Matrix projectionMatrix;
//		projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki
//		// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
//		// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
//		// このコードは確認テストのために残す
//		// projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
//
//		DirectXRender::SetProjectionMatrix(&projectionMatrix);
//	}
//
//	// 2D
//	else if (mode == 1) {
//		// ビュー変換後列作成
//		Vector3 pos = { 0.0f,0.0f,-10.0f };
//		Vector3 tgt = { 0.0f,0.0f,1.0f };
//		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
//		m_ViewMatrix = DirectX::XMMatrixLookAtLH(pos, tgt, up);
//		DirectXRender::SetViewMatrix(&m_ViewMatrix);
//
//		// プロジェクション行列の生成
//		float nearPlane = 1.0f;       // ニアクリップ
//		float farPlane = 1000.0f;      // ファークリップ
//
//		Matrix projectionMatrix = DirectX::XMMatrixOrthographicLH(static_cast<float>(Application::GetWidth()), static_cast<float>(Application::GetHeight()), nearPlane, farPlane);
//
//		projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
//		DirectXRender::SetProjectionMatrix(&projectionMatrix);
//	}
//	// スカイボックス用
//	// カメラが原点に存在するという情報をスカイボックスに適用することで、スカイボックスが全く動いていない様に見える
//	if (mode == 2) {
//		// ビュー変換後列作成
//		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
//		m_ViewMatrix = DirectX::XMMatrixLookAtLH(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f), up); // 左手系にした　20230511 by suzuki.tomoki
//		// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
//		// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
//		// このコードは確認テストのために残す
//		// m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					
//
//		DirectXRender::SetViewMatrix(&m_ViewMatrix);
//
//		//プロジェクション行列の生成
//		constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角
//
//		float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
//		float nearPlane = 1.0f;       // ニアクリップ
//		float farPlane = 1000.0f;      // ファークリップ
//
//		//プロジェクション行列の生成
//		Matrix projectionMatrix;
//		projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki
//		// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
//		// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
//		// このコードは確認テストのために残す
//		// projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
//
//		DirectXRender::SetProjectionMatrix(&projectionMatrix);
//	}
//}
