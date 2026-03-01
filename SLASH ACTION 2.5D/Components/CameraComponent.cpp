#include "System/DirectXRender.h"
#include "CameraComponent.h"
#include "Structs/CameraPattern.h"
#include "CameraMoveComponent.h"
#include "Render3DComponent.h"
#include "System/Application.h"
#include "Input/Input.h"
#include <Windows.h>

using namespace DirectX;
using namespace std;

CameraComponent::CameraComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("CAMERA"); // ソート番号を設定

	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr) {

		const XMFLOAT3 pos = transform->GetPosition();

		if (pos == m_Target) {
			MessageBoxA(nullptr, "カメラのポジションとターゲットが同じです。", "エラー", MB_OK | MB_ICONERROR);
			assert(false); // もしくは throw などで止める
		}
	}

	m_Target = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Render3DComponent* render3D = m_Object->AddComponent<Render3DComponent>();
	render3D->LoadModelMesh("assets/model/Camera/uploads_files_2423186_fbx+file.fbx", "assets/model/Camera/");
	render3D->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	render3D->SetActiveFlag(false); // 非表示にする
}


//=======================================
//更新処理
//=======================================
void CameraComponent::Update()
{
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();

	if (transform != nullptr) {
		const XMFLOAT3 pos = transform->GetPosition();

		if (pos == m_Target) {
			MessageBoxA(nullptr, "カメラのポジションとターゲットが同じです。", "エラー", MB_OK | MB_ICONERROR);
			assert(false); // もしくは throw などで止める
		}

		XMFLOAT3 forward = m_Target - pos;
		const XMVECTOR forwardVec = XMLoadFloat3(&forward);
		const float distance = XMVectorGetX(XMVector3Length(forwardVec));
		const XMVECTOR forwardNorm = XMVector3Normalize(forwardVec);
		XMStoreFloat3(&forward, forwardNorm);

		XMFLOAT3 move = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 初期化

		// 座標更新
		transform->AddPosition(move);

		XMFLOAT3 nowPos = transform->GetPosition();

		if (move != XMFLOAT3(0.0f, 0.0f, 0.0f)) {
			// ターゲット計算
			m_Target = nowPos + forward * distance;
		}

		CameraMoveComponent* cameraMove = m_Object->GetComponent<CameraMoveComponent>();

		if (cameraMove)
		{
			const CameraPattern cameraPattern = cameraMove->GetCameraPattern();

			if (cameraPattern != CameraPattern::CAMERA_NONE) {
				m_Target.x = nowPos.x;
				m_Target.y = nowPos.y;
			}
		}

		Update2D();
		Update3D();
		UpdateSky();
		DirectXRender::GPU_UpdateViewAndProj();
	}
}

void CameraComponent::Update2D() {
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	const XMFLOAT3 camPos = transform->GetPosition();
	// ビュー変換後列作成
	XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, -10.0f);
	const XMVECTOR posV = XMLoadFloat3(&pos);

	XMFLOAT3 tgt = XMFLOAT3(0.0f, 0.0f, 0.0f);
	const XMVECTOR tgtV = XMLoadFloat3(&tgt);

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

void CameraComponent::Update3D() {
	TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	XMFLOAT3 pos = transform->GetPosition();
	pos += m_OffsetPosition;

	// ビュー変換後列作成
	const XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX viewMatrix;

	const XMVECTOR posV = XMLoadFloat3(&pos);

	const XMFLOAT3 target = m_Target + m_OffsetTarget;
	const XMVECTOR targetV = XMLoadFloat3(&target);

	viewMatrix = XMMatrixLookAtLH(posV, targetV, up); // 左手系にした　20230511 by suzuki.tomoki
	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

	m_ViewMatrix3D = viewMatrix;

	DirectXRender::SetViewMatrix3D(&viewMatrix);

	//プロジェクション行列の生成
	float fieldOfView = 0.0f;    // 視野角

	switch (m_FieldOfView)
	{
	case FieldOfView::DEFAULT:

		fieldOfView = XMConvertToRadians(DEFAULT_FOV); // 標準
		break;
	case FieldOfView::WIDE:

		fieldOfView = XMConvertToRadians(WIDE_FOV); // 広角
		break;
	case FieldOfView::TELEPHOTO:

		fieldOfView = XMConvertToRadians(TELEPHOTO_FOV); // 望遠
		break;
	case FieldOfView::MAX:
		break;
	default:
		break;
	}

	// 後にここは調整できるようにしておく
	const float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 5000.0f;      // ファークリップ

	//プロジェクション行列の生成
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(
		fieldOfView,	// 垂直視野角
		aspectRatio,	// 画面のアスペクト比
		nearPlane,		// 前方クリップ面
		farPlane		// 後方クリップ面
	);	// 左手系にした　20230511 by suzuki.tomoki
	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	// projectionMatrix = SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	m_ProjectionMatrix3D = projectionMatrix;

	DirectXRender::SetProjectionMatrix3D(&projectionMatrix);
}

void CameraComponent::UpdateSky() {
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
