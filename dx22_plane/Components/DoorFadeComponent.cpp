#include "DoorFadeComponent.h"
#include "TransformComponent.h"
#include "Render2D.h"
#include "Manager/SceneManager.h"
#include "Manager/GameObjectManager.h"
#include "Manager/TimeManager.h"
#include "Input/Input.h"
#include "Mesh/SquareMesh.h"
#include <iostream>

DoorFadeComponent::DoorFadeComponent(GameObject& obj) : Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("DOOR_FADE"); // 更新の優先度を設定（数値が小さいほど優先して更新される）
	doorUp = GameObjectManager::AddUI("doorUP", "FadeUI");
	auto doorUpTransform = doorUp->AddComponent<TransformComponent>();
	doorUpTransform->SetPosition({ 0.0f,600.0f,-1.0f });
	doorUpTransform->SetScale({ 700.0f,200.0f,1.0f });
	doorUpFirstPos = doorUpTransform->GetPosition();
	auto doorUpRender = doorUp->AddComponent<Render2DComponent>();
	doorUpRender->CreateMesh<SquareMesh>();
	doorUpRender->ChangeTexture("assets/texture/IronWall.png");
	doorUpRender->SetShader("ShaderResource/unlitTextureVS2D.hlsl", "ShaderResource/unlitTexturePS.hlsl");

	doorDown = GameObjectManager::AddUI("doorDOWN", "FadeUI");
	auto doorDownTransform = doorDown->AddComponent<TransformComponent>();
	doorDownTransform->SetPosition({ 0.0f,-600.0f,-1.0f });
	doorDownTransform->SetScale({ 700.0f,200.0f,1.0f });
	doorDownFirstPos = doorDownTransform->GetPosition();
	auto doorDownRender = doorDown->AddComponent<Render2DComponent>();
	doorDownRender->CreateMesh<SquareMesh>();
	doorDownRender->ChangeTexture("assets/texture/IronWall.png");
	doorDownRender->SetShader("ShaderResource/unlitTextureVS2D.hlsl", "ShaderResource/unlitTexturePS.hlsl");

	doorUp->SetCarryOverFlag(true);
	doorDown->SetCarryOverFlag(true);
}

void DoorFadeComponent::Update()
{
	if (m_bootDoor == true && m_doorMoveEndFlag == true) {
		m_doorMoveEndFlag = false;
		if (m_openCloseFlag == false) {
			m_openCloseFlag = true;
		}
		else {
			m_openCloseFlag = false;
		}
		m_bootDoor = false;
		//SceneManager::WaitSceneChange<LoadStageScene>(2.0f);
	}

	if (m_openCloseFlag) {
		CloseDoor();
	}
	else {
		OpenDoor();
	}
}

void DoorFadeComponent::OpenDoor() {
	auto upTrans = doorUp->GetComponent<TransformComponent>();
	auto downTrans = doorDown->GetComponent<TransformComponent>();

	if (m_doorMoveEndFlag == false) {
		upTrans->AddPosition({ 0.0f,10.0f,0.0f });
		downTrans->AddPosition({ 0.0f,-10.0f,0.0f });
	}
	else {
		return;
	}

	timer += deltaTime;

	if (timer > 0.65f) {
		m_doorMoveEndFlag = true;
		timer = 0.0f;
	}
}

void DoorFadeComponent::CloseDoor() {
	auto upTrans = doorUp->GetComponent<TransformComponent>();
	auto downTrans = doorDown->GetComponent<TransformComponent>();

	auto upPos = upTrans->GetPosition();

	if (timer > 0.65f) {
		m_doorMoveEndFlag = true;
		timer = 0.0f;
		if(m_nextSceneName == "TitleScene"){
			SceneManager::SceneChange<TitleScene>();
		}
		else if (m_nextSceneName == "LoadStageScene") {
			SceneManager::SceneChange<LoadStageScene>();
		}
		else if(m_nextSceneName == "ResultScene") {
			SceneManager::SceneChange<ResultScene>();
		}
		else {
			// 何もしない
		}

		return;
	}

	if (m_doorMoveEndFlag == false) {
		upTrans->AddPosition({ 0.0f,-10.0f,0.0f });
		downTrans->AddPosition({ 0.0f,10.0f,0.0f });
	}
	else {
		return;
	}

	timer += deltaTime;
}