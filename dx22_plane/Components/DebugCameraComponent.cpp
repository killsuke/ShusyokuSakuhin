#include "DebugCameraComponent.h"
#include "System/DirectXRender.h"
#include "CameraPattern.h"
#include "CameraMoveComponent.h"
#include "System/Application.h"
#include "input.h"
#include <Windows.h>

using namespace DirectX;
using namespace std;

DebugCameraComponent::DebugCameraComponent(GameObject& obj) : Camera(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("CAMERA"); // ソート番号を設定
}


//=======================================
//更新処理
//=======================================
void DebugCameraComponent::Update()
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
		XMVECTOR rightVec = XMVector3Cross(upVec, forwardVec);
		rightVec = XMVector3Normalize(rightVec);
		XMStoreFloat3(&right, rightVec);

		XMFLOAT3 move = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 初期化

#if _DEBUG

		if (Input::GetKeyPress(VK_W)) {	// 前進
			move += forward;
		}
		if (Input::GetKeyPress(VK_S)) { // 後退
			move -= forward;
		}
		if (Input::GetKeyPress(VK_D)) {
			move += right;
		}
		if (Input::GetKeyPress(VK_A)) {
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

		//CameraMoveComponent* cameraMove = m_Object->GetComponent<CameraMoveComponent>();

		//if (cameraMove)
		//{
		//	CameraPattern cameraPattern = cameraMove->GetCameraPattern();

		//	if (cameraPattern != CameraPattern::CAMERA_NONE) {
		//		m_Target.x = nowPos.x;
		//		m_Target.y = nowPos.y;
		//	}
		//}

		// このフレームのマウス位置を次回に備えて保存
		prevMouse = mouseVec2;

		Update2D();
		Update3D();
		UpdateSky();
		DirectXRender::GPU_UpdateViewAndProj();
	}
}