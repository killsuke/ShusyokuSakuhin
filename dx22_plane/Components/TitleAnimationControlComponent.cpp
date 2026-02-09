#include "TitleAnimationControlComponent.h"
#include "Manager/GameObjectManager.h"
#include "Transform.h"
#include "Render2D.h"
#include "Render3D.h"
#include "VectorMoveComponent.h"
#include "TimeSpawnParticlesComponent.h"
#include "ProjectileMotionComponent.h"
#include "RigidBodyComponent.h"
#include "BlinkingUIComponent.h"
#include "DoorFadeComponent.h"
#include "Mesh/SquareMesh.h"
#include "Mesh/TriangleMesh.h"
#include "System/Application.h"
#include "Input/Input.h"

using namespace DirectX::SimpleMath;

namespace {
	constexpr float DeltaTime = 0.016f;
}

TitleAnimationControlComponent::TitleAnimationControlComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を仮置き

	

	GameObject* titleUI = GameObjectManager::AddObject("titleUI", "TitleUI");
	TransformComponent* transTitle = titleUI->AddComponent<TransformComponent>();
	transTitle->SetPosition({ 0.0f,70.0f,0.0f });
	transTitle->SetScale({ 350.0f,200.0f,1.0f });
//	transTitle->SetRotation({ 0.0f,30.0f,0.0f });
	Render3DComponent* rendTitle = titleUI->AddComponent<Render3DComponent>();
	rendTitle->CreateMesh<SquareMesh>();
	rendTitle->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	rendTitle->ChangeTexture("assets/texture/Slash_Action_Title.png");
	m_TitleRogo = titleUI;

	GameObject* sword3D = GameObjectManager::AddUI("sword3D", "TitleUI");
	TransformComponent* transSword = sword3D->AddComponent<TransformComponent>();
	transSword->SetPosition({ -1000.0f,-200.0f,-8.0f });
	transSword->SetScale({ 10.0f,500.0f,500.0f });
	transSword->SetRotation({ 0.0f,90.0f,0.0f });
	Render3DComponent* rendSword = sword3D->AddComponent<Render3DComponent>();
	rendSword->LoadModelMesh("assets/model/Copilot3D/sword3D.glb",
		"assets/model/Copilot3D");
	rendSword->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	m_Sword3D = sword3D;

	GameObject* startUI = GameObjectManager::AddObject("start", "TitleUI");
	TransformComponent* startTrans = startUI->AddComponent<TransformComponent>();
	startTrans->SetPosition({ 0.0f,-200.0f,-8.0f });
	startTrans->SetScale({ 130.0f,30.0f,1.0f });
	Render3DComponent* startRend = startUI->AddComponent<Render3DComponent>();
	startRend->CreateMesh<SquareMesh>();
	startRend->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	startRend->ChangeTexture("assets/texture/start.png");
	m_StartUI = startUI;
	m_StartUI->SetActiveState(ActiveState::ALL_STOP);

	GameObject* endUI = GameObjectManager::AddObject("end", "TitleUI");
	TransformComponent* endTrans = endUI->AddComponent<TransformComponent>();
	endTrans->SetPosition({ 0.0f,-290.0f,-8.0f });
	endTrans->SetScale({ 80.0f,30.0f,1.0f });
	Render3DComponent* endRend = endUI->AddComponent<Render3DComponent>();
	endRend->CreateMesh<SquareMesh>();
	endRend->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	endRend->ChangeTexture("assets/texture/end.png");
	m_EndUI = endUI;
	m_EndUI->SetActiveState(ActiveState::ALL_STOP);

	GameObject* miniSword = GameObjectManager::AddObject("miniSword", "TitleUI");
	TransformComponent* transMiniSword = miniSword->AddComponent<TransformComponent>();
	transMiniSword->SetPosition({ -200.0f,-200.0f,-8.0f });
	transMiniSword->SetScale({ 150.0f,150.0f,150.0f });
	transMiniSword->SetRotation({ 0.0f,90.0f,0.0f });
	Render3DComponent* rendMiniSword = miniSword->AddComponent<Render3DComponent>();
	rendMiniSword->LoadModelMesh("assets/model/Copilot3D/sword3D.glb",
		"assets/model/Copilot3D");
	rendMiniSword->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	m_MiniSword3D = miniSword;
	m_MiniSword3D->SetActiveState(ActiveState::ALL_STOP);

	// 割れる三角形４つ
	{
		GameObject* triangle1 = GameObjectManager::AddObject("triangle1", "TitleUI");
		TransformComponent* transTri1 = triangle1->AddComponent<TransformComponent>();
		transTri1->SetPosition({ 0.0f,-180.0f,-9.6f });
		transTri1->SetScale({ 645.0f,180.0f,1.0f });
		ProjectileMotionComponent* projComp1 = triangle1->AddComponent<ProjectileMotionComponent>();
		projComp1->InitProjectile({ 0.0f, 5.0f, -20.0f }, { 0.8f,0.0f,1.0f }, 50.0f);
		projComp1->SetActiveFlag(false);
		RigidBodyComponent* rigidComp1 = triangle1->AddComponent<RigidBodyComponent>();
		rigidComp1->SetGravityFlag(true);
		rigidComp1->SetFallMagnification(36.0f);
		rigidComp1->SetMass(2.0f);
		rigidComp1->SetActiveFlag(false);
		rigidComp1->SetStopGravity(400.0f);
		Render2DComponent* rendTri1 = triangle1->AddComponent<Render2DComponent>();
		rendTri1->CreateMesh<TriangleMesh>();
		rendTri1->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	//	rendTri1->ChangeTexture("assets/texture/testTerrain.png");
		m_Triangle1 = triangle1;

		GameObject* triangle2 = GameObjectManager::AddObject("triangle2", "TitleUI");
		TransformComponent* transTri2 = triangle2->AddComponent<TransformComponent>();
		transTri2->SetPosition({ 0.0f,200.0f,-9.6f });
		transTri2->SetScale({ 660.0f,200.0f,1.0f });
		transTri2->SetRotation({ 0.0f,0.0f,180.0f });
		ProjectileMotionComponent* projComp2 = triangle2->AddComponent<ProjectileMotionComponent>();
		projComp2->InitProjectile({ 0.0f, 5.0f, -20.0f }, { 1.0f,0.0f,1.0f }, 50.0f);
		projComp2->SetActiveFlag(false);
		RigidBodyComponent* rigidComp2 = triangle2->AddComponent<RigidBodyComponent>();
		rigidComp2->SetGravityFlag(true);
		rigidComp2->SetFallMagnification(36.0f);
		rigidComp2->SetMass(2.0f);
		rigidComp2->SetActiveFlag(false);
		rigidComp2->SetStopGravity(400.0f);
		Render2DComponent* rendTri2 = triangle2->AddComponent<Render2DComponent>();
		rendTri2->CreateMesh<TriangleMesh>();
		rendTri2->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		//rendTri2->ChangeTexture("assets/texture/testTerrain.png");
		m_Triangle2 = triangle2;

		GameObject* triangle3 = GameObjectManager::AddObject("triangle3", "TitleUI");
		TransformComponent* transTri3 = triangle3->AddComponent<TransformComponent>();
		transTri3->SetPosition({ 320.0f,15.0f,-9.6f });
		transTri3->SetScale({ 400.0f,360.0f,1.0f });
		transTri3->SetRotation({ 0.0f,0.0f,90.0f });
		ProjectileMotionComponent* projComp3 = triangle3->AddComponent<ProjectileMotionComponent>();
		projComp3->InitProjectile({ 0.0f, 5.0f, -20.0f }, { -0.5f,0.0f,1.0f }, 50.0f);
		projComp3->SetActiveFlag(false);
		RigidBodyComponent* rigidComp3 = triangle3->AddComponent<RigidBodyComponent>();
		rigidComp3->SetGravityFlag(true);
		rigidComp3->SetFallMagnification(36.0f);
		rigidComp3->SetMass(2.0f);
		rigidComp3->SetActiveFlag(false);
		rigidComp3->SetStopGravity(400.0f);
		Render2DComponent* rendTri3 = triangle3->AddComponent<Render2DComponent>();
		rendTri3->CreateMesh<TriangleMesh>();
		rendTri3->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		//rendTri3->ChangeTexture("assets/texture/testTerrain.png");
		m_Triangle3 = triangle3;

		GameObject* triangle4 = GameObjectManager::AddObject("triangle4", "TitleUI");
		TransformComponent* transTri4 = triangle4->AddComponent<TransformComponent>();
		transTri4->SetPosition({ -320.0f,15.0f,-9.6f });
		transTri4->SetScale({ 400.0f,360.0f,1.0f });
		transTri4->SetRotation({ 0.0f,0.0f,-90.0f });
		ProjectileMotionComponent* projComp4 = triangle4->AddComponent<ProjectileMotionComponent>();
		projComp4->InitProjectile({ 0.0f, 5.0f, -20.0f }, { -0.5f,0.0f,0.0f },50.0f);
		projComp4->SetActiveFlag(false);
		RigidBodyComponent* rigidComp4 = triangle4->AddComponent<RigidBodyComponent>();
		rigidComp4->SetGravityFlag(true);
		rigidComp4->SetFallMagnification(36.0f);
		rigidComp4->SetMass(2.0f);
		rigidComp4->SetActiveFlag(false);
		rigidComp4->SetStopGravity(400.0f);
		Render2DComponent* rendTri4 = triangle4->AddComponent<Render2DComponent>();
		rendTri4->CreateMesh<TriangleMesh>();
		rendTri4->SetShader("ShaderResource/litTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		//rendTri4->ChangeTexture("assets/texture/testTerrain.png");
		m_Triangle4 = triangle4;
	}

	GameObject* slashLineUI = GameObjectManager::AddObject("slashLineUI", "TitleUI");
	TransformComponent* transSlashLine = slashLineUI->AddComponent<TransformComponent>();
	transSlashLine->SetPosition({ -2280.0f,1280.0f,-10.6f });
	transSlashLine->SetScale({ 1080.0f,100.0f,1.0f });
	transSlashLine->SetRotation({ 0.0f,0.0f,-29.0f });
	VectorMoveComponent* lineMove = slashLineUI->AddComponent<VectorMoveComponent>();
	lineMove->SetMovePower(80.0f);
	lineMove->SetMoveDirection({ 1.8f,-1.0f,0.0f });
	TimeSpawnParticlesComponent* timeSp1 = slashLineUI->AddComponent<TimeSpawnParticlesComponent>();
	timeSp1->SetSpawnInterval(0.03f);
	Render2DComponent* rendSlashLine = slashLineUI->AddComponent<Render2DComponent>();
	rendSlashLine->CreateMesh<SquareMesh>();
	rendSlashLine->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	rendSlashLine->ChangeTexture("assets/texture/LightLine.png");
	m_SlashUI1 = slashLineUI;
	m_SlashUI1->SetActiveState(ActiveState::UPDATE_STOP);

	GameObject* slashLineUI2 = GameObjectManager::AddObject("slashLineUI2", "TitleUI");
	TransformComponent* transSlashLine2 = slashLineUI2->AddComponent<TransformComponent>();
	transSlashLine2->SetPosition({ 2280.0f,1280.0f,-10.6f });
	transSlashLine2->SetScale({ 1080.0f,100.0f,1.0f });
	transSlashLine2->SetRotation({ 0.0f,0.0f,209.0f });
	VectorMoveComponent* lineMove2 = slashLineUI2->AddComponent<VectorMoveComponent>();
	lineMove2->SetMovePower(80.0f);
	lineMove2->SetMoveDirection({ -1.8f,-1.0f,0.0f });
	TimeSpawnParticlesComponent* timeSp2 = slashLineUI2->AddComponent<TimeSpawnParticlesComponent>();
	timeSp2->SetSpawnInterval(0.03f);
	Render2DComponent* rendSlashLine2 = slashLineUI2->AddComponent<Render2DComponent>();
	rendSlashLine2->CreateMesh<SquareMesh>();
	rendSlashLine2->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
	rendSlashLine2->ChangeTexture("assets/texture/LightLine.png");
	m_SlashUI2 = slashLineUI2;
	m_SlashUI2->SetActiveState(ActiveState::UPDATE_STOP);
}

void TitleAnimationControlComponent::Update() {

	const bool IsEnter = (Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_A));
	const bool IsUporDown = (Input::GetKeyTrigger(VK_W) || Input::GetKeyTrigger(VK_S) || Input::GetKeyTrigger(VK_UP) || Input::GetKeyTrigger(VK_DOWN)
		|| Input::GetButtonTrigger(XINPUT_UP) || Input::GetButtonTrigger(XINPUT_DOWN));

	// ここで一連のアニメーション終了後の処理
	if (m_RecordTime > 5.9f) {
		m_StartUI->SetActiveState(ActiveState::ACTIVE);
		m_EndUI->SetActiveState(ActiveState::ACTIVE);
		m_MiniSword3D->SetActiveState(ActiveState::ACTIVE);

		TransformComponent* trans = m_MiniSword3D->GetComponent<TransformComponent>();
		trans->AddRotation({0.0f,0.0f,2.0f});

		// ここで十字キー上下とかで選択
		if (IsUporDown == true) {
			m_IsMiniSwordUpDown = !m_IsMiniSwordUpDown;
		}

		if (m_IsMiniSwordUpDown == true) {
			trans->SetPosition({ -200.0f,-200.0f,-8.0f });
			// エンターキーを押してステージ1へ
			if (IsEnter == true)
			{
				auto fade = GameObjectManager::GameObjectFindNameUI("fade");
				auto door = fade->GetComponent<DoorFadeComponent>();
				door->SetBootDoor(true);
			}
		}
		else {
			trans->SetPosition({ -150.0f,-290.0f,-8.0f });
			if (IsEnter == true) {
				Application::GameEnd();
			}
		}
	}
	// 出現させた剣をロゴの後ろに移動させる
	else if (m_RecordTime > 5.0f) {
		TransformComponent* trans = m_Sword3D->GetComponent<TransformComponent>();
		trans->AddPosition({-20.0f,-20.0f,0.0f});
	}
	// ロゴ回転
	else if (m_RecordTime > 4.7f) {
		TransformComponent* trans = m_TitleRogo->GetComponent<TransformComponent>();
		trans->AddRotation({0.0f,2.0f,0.0f});

		TransformComponent* transSword = m_Sword3D->GetComponent<TransformComponent>();
		transSword->SetPosition({ 1200.0f,1200.0f,-30.0f });
		transSword->SetRotation({45.0f,-50.0f,0.0f});
		transSword->SetScale({ 10.0f,500.0f,800.0f });
	}
	// 剣出現
	else if (m_RecordTime > 3.8) {
		TransformComponent* trans = m_Sword3D->GetComponent<TransformComponent>();
		trans->AddPosition({ 40.0f,0.0f,0.0f });
	}
	// 三角形飛ばす、画面割れ
	else if (m_RecordTime > 2.2f) {
		RigidBodyComponent* rigid1 = m_Triangle1->GetComponent<RigidBodyComponent>();
		rigid1->SetActiveFlag(true);
		RigidBodyComponent* rigid2 = m_Triangle2->GetComponent<RigidBodyComponent>();
		rigid2->SetActiveFlag(true);
		RigidBodyComponent* rigid3 = m_Triangle3->GetComponent<RigidBodyComponent>();
		rigid3->SetActiveFlag(true);
		RigidBodyComponent* rigid4 = m_Triangle4->GetComponent<RigidBodyComponent>();
		rigid4->SetActiveFlag(true);
		ProjectileMotionComponent* proj1 = m_Triangle1->GetComponent<ProjectileMotionComponent>();
		proj1->SetActiveFlag(true);
		ProjectileMotionComponent* proj2 = m_Triangle2->GetComponent<ProjectileMotionComponent>();
		proj2->SetActiveFlag(true);
		ProjectileMotionComponent* proj3 = m_Triangle3->GetComponent<ProjectileMotionComponent>();
		proj3->SetActiveFlag(true);
		ProjectileMotionComponent* proj4 = m_Triangle4->GetComponent<ProjectileMotionComponent>();
		proj4->SetActiveFlag(true);


	}
	// 斬撃の役目終了
	else if (m_RecordTime > 1.8f) {
		m_SlashUI1->SetActiveState(ActiveState::ALL_STOP);
		m_SlashUI2->SetActiveState(ActiveState::ALL_STOP);
	}
	// ２本目の斬撃
	else if (m_RecordTime > 0.7f) {
		m_SlashUI2->SetActiveState(ActiveState::ACTIVE);
	}
	// １本目の斬撃
	else if (m_RecordTime > 0.5f) {
		m_SlashUI1->SetActiveState(ActiveState::ACTIVE);
	}
	

	m_RecordTime += DeltaTime;
}