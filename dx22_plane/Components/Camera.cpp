#include "System/DirectXRender.h"
#include "Camera.h"
#include "CameraPattern.h"
#include "CameraMoveComponent.h"
#include "System/Application.h"
#include "input.h"
#include <Windows.h>

using namespace DirectX;
using namespace std;

Camera::Camera(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("CAMERA"); // ソート番号を設定

	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr) {
		XMFLOAT3 pos = transform->GetPosition();

		if (pos == m_Target) {
			MessageBoxA(nullptr, "カメラのポジションとターゲットが同じです。", "エラー", MB_OK | MB_ICONERROR);
			assert(false); // もしくは throw などで止める
		}
	}

	m_Target = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


//=======================================
//更新処理
//=======================================
void Camera::Update()
{
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr) {
		XMFLOAT3 pos = transform->GetPosition();

		if (pos == m_Target) {
			MessageBoxA(nullptr, "カメラのポジションとターゲットが同じです。", "エラー", MB_OK | MB_ICONERROR);
			assert(false); // もしくは throw などで止める
		}

		XMFLOAT3 forward = m_Target - pos;
		XMVECTOR forwardVec = XMLoadFloat3(&forward);
		const float distance = XMVectorGetX(XMVector3Length(forwardVec));
		XMVECTOR forwardNorm = XMVector3Normalize(forwardVec);
		XMStoreFloat3(&forward, forwardNorm);

		XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
		XMVECTOR upVec = XMLoadFloat3(&up);

		XMFLOAT3 right = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMVECTOR rightVec = XMVector3Cross(upVec,forwardVec);
		rightVec = XMVector3Normalize(rightVec);
		XMStoreFloat3(&right, rightVec);

		XMFLOAT3 move = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 初期化

#if _DEBUG

		if (Input::GetKeyPress(VK_I)) {	// 前進
			move += forward;
		}
		if (Input::GetKeyPress(VK_K)) { // 後退
			move -= forward;
		}
		if (Input::GetKeyPress(VK_L)) {
			move += right;
		}
		if (Input::GetKeyPress(VK_J)) {
			move -= right;
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

		XMVECTOR moveVec = XMLoadFloat3(&move);
		const float distSq = XMVectorGetX(XMVector3LengthSq(moveVec));

		if (distSq > 0.0f) {
			moveVec = XMVector3Normalize(moveVec);
			XMStoreFloat3(&move, moveVec);
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
		XMFLOAT3 nowPos = transform->GetPosition();

		if (move != XMFLOAT3(0.0f, 0.0f, 0.0f)) {
			// ターゲット計算
			m_Target = nowPos + forward * distance;
		}


		// マウスの座標を取得
		XMFLOAT2 mouseVec2 = Input::GetMousePositionNormalize();

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
			float PitchRadians = XMConvertToRadians(rot2.x); // X軸回転
			float YawRadians = XMConvertToRadians(rot2.y);     // Y軸回転
			float RollRadians = XMConvertToRadians(rot2.z);   // Z軸回転

			// クォータニオンを構成（ピッチ・ヨーを個別に回転軸に適用）
			XMVECTOR qPitch = XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), PitchRadians);
			XMVECTOR qYaw = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), YawRadians);

			// 合成（順番注意：Yawを先にかけると、カメラが左右中心に回る）
			XMVECTOR qRotation = XMQuaternionMultiply(qPitch, qYaw);

			// 回転行列に変換
			XMMATRIX rotMat = XMMatrixRotationQuaternion(qRotation);

			// 前方ベクトル回転
			XMVECTOR newForward = XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), rotMat);

			// ターゲット計算
			const XMVECTOR nowPosV = XMLoadFloat3(&nowPos);
			const XMVECTOR targetV = XMVectorAdd(nowPosV, newForward);
			XMStoreFloat3(&m_Target, targetV);
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
	XMFLOAT3 camPos = transform->GetPosition();
	// ビュー変換後列作成
	XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, -10.0f);
	XMVECTOR posV = XMLoadFloat3(&pos);

	XMFLOAT3 tgt = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMVECTOR tgtV = XMLoadFloat3(&tgt);

	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX viewMatrix = XMMatrixLookAtLH(posV, tgtV, up);

	m_ViewMatrix2D = viewMatrix;

	DirectXRender::SetViewMatrix2D(&viewMatrix);

	// プロジェクション行列の生成
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 1000.0f;      // ファークリップ

	XMMATRIX projectionMatrix = XMMatrixOrthographicLH(static_cast<float>(Application::GetWidth()), static_cast<float>(Application::GetHeight()), nearPlane, farPlane);

	m_ProjectionMatrix2D = projectionMatrix;

	DirectXRender::SetProjectionMatrix2D(&projectionMatrix);
}

void Camera::Update3D() {
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	XMFLOAT3 pos = transform->GetPosition();

	// ビュー変換後列作成
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX viewMatrix;

	XMVECTOR posV = XMLoadFloat3(&pos);
	XMVECTOR targetV = XMLoadFloat3(&m_Target);


	viewMatrix = XMMatrixLookAtLH(posV, targetV, up); // 左手系にした　20230511 by suzuki.tomoki
	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

	m_ViewMatrix3D = viewMatrix;

	DirectXRender::SetViewMatrix3D(&viewMatrix);

	//プロジェクション行列の生成
	constexpr float fieldOfView = XMConvertToRadians(45.0f);    // 視野角

	// 後にここは調整できるようにしておく
	float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 5000.0f;      // ファークリップ

	//プロジェクション行列の生成
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki
	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// projectionMatrix = SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	m_ProjectionMatrix3D = projectionMatrix;

	DirectXRender::SetProjectionMatrix3D(&projectionMatrix);
}

void Camera::UpdateSky() {
	//// ビュー変換後列作成
	XMVECTOR up = XMVectorSet(0.0f,1.0f,0.0f,0.0f);
	XMVECTOR posV = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR targetV = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	m_ViewMatrixSky = XMMatrixLookAtLH(posV, targetV, up); // 左手系にした　20230511 by suzuki.tomoki
	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

	DirectXRender::SetViewMatrixSkyDome(&m_ViewMatrixSky);

	//プロジェクション行列の生成
	constexpr float fieldOfView = XMConvertToRadians(45.0f);    // 視野角

	float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 1000.0f;      // ファークリップ

	//プロジェクション行列の生成

	m_ProjectionMatrixSky = XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);	// 左手系にした　20230511 by suzuki.tomoki

	DirectXRender::SetProjectionMatrixSkyDome(&m_ProjectionMatrixSky);

	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// projectionMatrix = SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	//projectionMtxSky = XMMatrixTranspose(projectionMatrix);
}
