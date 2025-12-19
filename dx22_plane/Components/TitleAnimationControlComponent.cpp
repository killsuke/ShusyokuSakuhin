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
#include "Mesh/SquareMesh.h"
#include "Mesh/TriangleMesh.h"

using namespace DirectX::SimpleMath;

namespace {
	constexpr float DeltaTime = 0.016f;
}

TitleAnimationControlComponent::TitleAnimationControlComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を仮置き

	/*GameObject* sword3D = GameObjectManager::AddUI("sword3D", "TitleUI");
	TransformComponent* transSword = sword3D->AddComponent<TransformComponent>();
	transSword->SetPosition({ 0.0f,0.0f,1.0f });
	transSword->SetScale({ 40.0f,400.0f,400.0f });
	transSword->SetRotation({ 0.0f,-90.0f,0.0f });
	Render3DComponent* rendSword = sword3D->AddComponent<Render3DComponent>();
	rendSword->LoadModelMesh("assets/model/Copilot3D/sword3D.glb",
		"assets/model/Copilot3D");
	rendSword->SetShader("shader/unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");*/

	GameObject* titleUI = GameObjectManager::AddObject("titleUI", "TitleUI");
	TransformComponent* transTitle = titleUI->AddComponent<TransformComponent>();
	transTitle->SetPosition({ 0.0f,70.0f,0.0f });
	transTitle->SetScale({ 400.0f,280.0f,1.0f });
	Render3DComponent* rendTitle = titleUI->AddComponent<Render3DComponent>();
	rendTitle->CreateMesh<SquareMesh>();
	rendTitle->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	rendTitle->ChangeTexture("assets/texture/Slash_Action_Title.png");

	/*{
		auto titleUI = GameObjectManager::AddUI("tenmetuUI", "TitleUI");
		auto transTitle = titleUI->AddComponent<TransformComponent>();
		transTitle->SetPosition({ 0.0f,-200.0f,-0.5f });
		transTitle->SetScale({ 200.0f,100.0f,1.0f });
		auto blink = titleUI->AddComponent<BlinkingUIComponent>();
		blink->SetBlinkingSpeed(0.7f);

		auto rendTitle = titleUI->AddComponent<Render2DComponent>();
		rendTitle->CreateMesh<SquareMesh>();
		rendTitle->SetShader("shader/unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
		rendTitle->ChangeTexture("assets/texture/please.png");
		rendTitle->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}*/


	// これを4枚分作って前に飛び出しながら回転するとか？
	// 位置・サイズを調整
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
		rendTri1->SetShader("shader/litTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
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
		rendTri2->SetShader("shader/litTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
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
		rendTri3->SetShader("shader/litTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
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
		rendTri4->SetShader("shader/litTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
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
	rendSlashLine->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
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
	rendSlashLine2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	rendSlashLine2->ChangeTexture("assets/texture/LightLine.png");
	m_SlashUI2 = slashLineUI2;
	m_SlashUI2->SetActiveState(ActiveState::UPDATE_STOP);

	

}

void TitleAnimationControlComponent::Update() {

	// 仮おき
	// 終了時間
	if (m_RecordTime > 2.2f) {
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