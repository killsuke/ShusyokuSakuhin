#include "DirectXRender.h"
#include "Camera.h"
#include "CameraPattern.h"
#include "CameraMoveComponent.h"
#include "Application.h"
#include "input.h"
#include "Transform.h"
#include <Windows.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;
using namespace std;

Camera::Camera(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("CAMERA"); // ソート番号を設定

	auto transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr) {
		auto pos = transform->GetPosition();

		if (pos == m_Target) {
			MessageBoxA(nullptr, "カメラのポジションとターゲットが同じです。", "エラー", MB_OK | MB_ICONERROR);
			assert(false); // もしくは throw などで止める
		}
	}

	m_Target = Vector3::Zero;
}


//=======================================
//更新処理
//=======================================
void Camera::Update()
{
	auto transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr) {
		Vector3 pos = transform->GetPosition();

		if (pos == m_Target) {
			MessageBoxA(nullptr, "カメラのポジションとターゲットが同じです。", "エラー", MB_OK | MB_ICONERROR);
			assert(false); // もしくは throw などで止める
		}

		Vector3 forward = m_Target - pos;
		const float distance = forward.Length();
		forward.Normalize();

		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		Vector3 right = forward.Cross(up);
		right.Normalize();


		Vector3 move = {};	// 初期化

#if _DEBUG

		if (Input::GetKeyPress(VK_I)) {	// 前進
			move += forward;
		}
		if (Input::GetKeyPress(VK_K)) { // 後退
			move -= forward;
		}
		if (Input::GetKeyPress(VK_L)) {
			move -= right;
		}
		if (Input::GetKeyPress(VK_J)) {
			move += right;
		}
		if (Input::GetKeyPress(VK_UP)) {
			move += up;
		}
		if (Input::GetKeyPress(VK_DOWN)) {
			move -= up;
		}
		if (Input::GetKeyPress(VK_SHIFT)) {
			m_Target.x = pos.x;
			m_Target.y = pos.y;
		}

		if (move.LengthSquared() > 0.0f) {
			move.Normalize();
			if (Input::GetKeyPress(VK_DELETE)) {
				move *= 0.5f; // 移動速度
			}
			else {
				move *= 5.0f; // 移動速度
			}
		}

#endif

		// 座標更新
		transform->AddPosition(move);

		XMFLOAT3 rot = transform->GetRotation();
		Vector3 nowPos = transform->GetPosition();

		if (move != Vector3::Zero) {
			// ターゲット計算
			m_Target = nowPos + forward * distance;
		}


		// マウスの座標を取得
		Vector2 mouseVec2 = Input::GetMousePositionNormalize();

#if _DEBUG

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

			// 前方ベクトル回転
			DirectX::XMVECTOR forward = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), rotMat);

			// ターゲット計算
			m_Target = DirectX::XMVectorAdd(nowPos, forward);
		}

#endif

		CameraMoveComponent* cameraMove = m_Object->GetComponent<CameraMoveComponent>();

		if (cameraMove)
		{
			CameraPattern cameraPattern = cameraMove->GetCameraPattern();

			if (cameraPattern != CameraPattern::CAMERA_NONE) {
				m_Target.x = nowPos.x;
				m_Target.y = nowPos.y;
			}
		}

		// このフレームのマウス位置を次回に備えて保存
		prevMouse = mouseVec2;

		Update2D();
		Update3D();
		UpdateSky();
		DirectXRender::GPU_UpdateViewAndProj();
	}
}

void Camera::Update2D() {
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	Vector3 camPos = transform->GetPosition();
	// ビュー変換後列作成
	Vector3 pos = { 0.0f,0.0f,camPos.z };
	Vector3 tgt = { 0.0f,0.0f,m_Target.z };
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(pos, tgt, up);

	m_ViewMatrix2D = viewMatrix;

	DirectXRender::SetViewMatrix2D(&viewMatrix);

	// プロジェクション行列の生成
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 1000.0f;      // ファークリップ

	XMMATRIX projectionMatrix = DirectX::XMMatrixOrthographicLH(static_cast<float>(Application::GetWidth()), static_cast<float>(Application::GetHeight()), nearPlane, farPlane);

	m_ProjectionMatrix2D = projectionMatrix;

	DirectXRender::SetProjectionMatrix2D(&projectionMatrix);
}

void Camera::Update3D() {
	auto transform = m_Object->GetComponent<TransformComponent>();
	auto pos = transform->GetPosition();

	// ビュー変換後列作成
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	XMMATRIX viewMatrix;
	viewMatrix = DirectX::XMMatrixLookAtLH(pos, m_Target, up); // 左手系にした　20230511 by suzuki.tomoki
	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

	m_ViewMatrix3D = viewMatrix;

	DirectXRender::SetViewMatrix3D(&viewMatrix);

	//プロジェクション行列の生成
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角

	float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 5000.0f;      // ファークリップ

	//プロジェクション行列の生成
	XMMATRIX projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki
	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	m_ProjectionMatrix3D = projectionMatrix;

	DirectXRender::SetProjectionMatrix3D(&projectionMatrix);
}

void Camera::UpdateSky() {
	//// ビュー変換後列作成
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	m_ViewMatrixSky = DirectX::XMMatrixLookAtLH(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f), up); // 左手系にした　20230511 by suzuki.tomoki
	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

	DirectXRender::SetViewMatrixSkyDome(&m_ViewMatrixSky);

	//プロジェクション行列の生成
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角

	float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 1000.0f;      // ファークリップ

	//プロジェクション行列の生成

	m_ProjectionMatrixSky = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki

	DirectXRender::SetProjectionMatrixSkyDome(&m_ProjectionMatrixSky);

	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	//projectionMtxSky = DirectX::XMMatrixTranspose(projectionMatrix);
}
