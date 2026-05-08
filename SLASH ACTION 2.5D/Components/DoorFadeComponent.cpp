#include "DoorFadeComponent.h"
#include "TransformComponent.h"
#include "Render2DComponent.h"
#include "Manager/SceneManager.h"
#include "Manager/GameObjectManager.h"
#include "Manager/TimeManager.h"
#include "Input/Input.h"
#include "Mesh/SquareMesh.h"
#include <iostream>

using namespace DirectX;

namespace
{
	constexpr float DOOR_MOVE_DURATION = 0.70f; // ドアの移動にかかる時間
	constexpr XMFLOAT3 DOOR_UP_SPEED = { 0.0f, 10.0f, 0.0f }; // ドアが開くときの上ドアの移動速度
	constexpr XMFLOAT3 DOOR_DOWN_SPEED = { 0.0f, -10.0f, 0.0f }; // ドアが開くときの下ドアの移動速度
	constexpr XMFLOAT3 DOOR_UP_CLOSE_SPEED = { 0.0f, -10.0f, 0.0f }; // ドアが閉じるときの上ドアの移動速度
	constexpr XMFLOAT3 DOOR_DOWN_CLOSE_SPEED = { 0.0f, 10.0f, 0.0f }; // ドアが閉じるときの下ドアの移動速度
}

DoorFadeComponent::DoorFadeComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("DOOR_FADE"); // 更新の優先度を設定（数値が小さいほど優先して更新される）
	m_DoorUpObj = GameObjectManager::AddUI("doorUP", "FadeUI");
	TransformComponent* doorUpTransform = m_DoorUpObj->AddComponent<TransformComponent>();
	doorUpTransform->SetPosition({ 0.0f,600.0f,-1.0f });
	doorUpTransform->SetScale({ 700.0f,200.0f,1.0f });
	m_DoorUpFirstPos = { 0.0f,600.0f,-1.0f };
	Render2DComponent* doorUpRender = m_DoorUpObj->AddComponent<Render2DComponent>();
	doorUpRender->CreateMesh<SquareMesh>();
	doorUpRender->ChangeTexture("IronWall.png");
	doorUpRender->SetShader("unlitTextureVS2D.hlsl", "unlitTexturePS.hlsl");

	m_DoorDownObj = GameObjectManager::AddUI("doorDOWN", "FadeUI");
	TransformComponent* doorDownTransform = m_DoorDownObj->AddComponent<TransformComponent>();
	doorDownTransform->SetPosition({ 0.0f,-600.0f,-1.0f });
	doorDownTransform->SetScale({ 700.0f,200.0f,1.0f });
	m_DoorDownFirstPos = { 0.0f,-600.0f,-1.0f };
	Render2DComponent* doorDownRender = m_DoorDownObj->AddComponent<Render2DComponent>();
	doorDownRender->CreateMesh<SquareMesh>();
	doorDownRender->ChangeTexture("IronWall.png");
	doorDownRender->SetShader("unlitTextureVS2D.hlsl", "unlitTexturePS.hlsl");

	m_DoorUpObj->SetCarryOverFlag(true);
	m_DoorDownObj->SetCarryOverFlag(true);
}

void DoorFadeComponent::Update()
{
	if (m_IsDootDoor == true && m_IsDoorMoveEndFlag == true) {
		m_IsDoorMoveEndFlag = false;
		if (m_IsOpenCloseFlag == false) {
			m_IsOpenCloseFlag = true;
		}
		else {
			m_IsOpenCloseFlag = false;
		}
		m_IsDootDoor = false;
	}

	if (m_IsOpenCloseFlag) {
		CloseDoor();
	}
	else {
		OpenDoor();
	}
}

// ドアを開く処理
void DoorFadeComponent::OpenDoor() {
	
	TransformComponent* upTrans = m_DoorUpObj->GetComponent<TransformComponent>();
	TransformComponent* downTrans = m_DoorDownObj->GetComponent<TransformComponent>();

	if(upTrans == nullptr || downTrans == nullptr) {
		return;
	}

	if (m_IsDoorMoveEndFlag == false) {
		upTrans->AddPosition(DOOR_UP_SPEED);
		downTrans->AddPosition(DOOR_DOWN_SPEED);
	}
	else {
		return;
	}

	const float deltaTime = TimeManager::GetFixedDeltaTime();

	m_RecordTime += deltaTime;

	if (m_RecordTime > DOOR_MOVE_DURATION) {
		m_IsDoorMoveEndFlag = true;

		upTrans->SetPosition(m_DoorUpFirstPos);
		downTrans->SetPosition(m_DoorDownFirstPos);
		m_RecordTime = 0.0f;
	}
}

// ドアを閉じる処理
void DoorFadeComponent::CloseDoor() {
	
	TransformComponent* upTrans = m_DoorUpObj->GetComponent<TransformComponent>();
	TransformComponent* downTrans = m_DoorDownObj->GetComponent<TransformComponent>();

	if(upTrans == nullptr || downTrans == nullptr) {
		return;
	}

	DirectX::XMFLOAT3 upPos = upTrans->GetPosition();

	const float deltaTime = TimeManager::GetFixedDeltaTime();

	m_RecordTime += deltaTime;

	if (m_RecordTime > DOOR_MOVE_DURATION) {


		m_IsDoorMoveEndFlag = true;
		m_RecordTime = 0.0f;
		if(m_NextSceneName == "TitleScene"){
			SceneManager::SceneChange<TitleScene>();
		}
		else if (m_NextSceneName == "LoadStageScene") {
			SceneManager::SceneChange<LoadStageScene>();
		}
		else if(m_NextSceneName == "ResultScene") {
			SceneManager::SceneChange<ResultScene>();
		}
		else {
			// 何もしない
		}

		return;
	}

	if (m_IsDoorMoveEndFlag == false) {
		upTrans->AddPosition(DOOR_UP_CLOSE_SPEED);
		downTrans->AddPosition(DOOR_DOWN_CLOSE_SPEED);
	}
}