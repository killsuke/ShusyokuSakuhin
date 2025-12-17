#include "TitleAnimationControlComponent.h"
#include "Manager/GameObjectManager.h"
#include "Transform.h"
#include "Render2D.h"
#include "VectorMoveComponent.h"
#include "TimeSpawnParticlesComponent.h"
#include "Mesh/SquareMesh.h"
#include "Mesh/TriangleMesh.h"

using namespace DirectX::SimpleMath;

namespace {
	constexpr float DeltaTime = 0.016f;
}

TitleAnimationControlComponent::TitleAnimationControlComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を仮置き

	// これを4枚分作って前に飛び出しながら回転するとか？
	// 位置・サイズを調整
	{
		GameObject* triangle1 = GameObjectManager::AddUI("triangle1", "TitleUI");
		TransformComponent* transTri1 = triangle1->AddComponent<TransformComponent>();
		transTri1->SetPosition({ 0.0f,-180.0f,-0.6f });
		transTri1->SetScale({ 645.0f,180.0f,1.0f });
		Render2DComponent* rendTri1 = triangle1->AddComponent<Render2DComponent>();
		rendTri1->CreateMesh<TriangleMesh>();
		rendTri1->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");

		GameObject* triangle2 = GameObjectManager::AddUI("triangle2", "TitleUI");
		TransformComponent* transTri2 = triangle2->AddComponent<TransformComponent>();
		transTri2->SetPosition({ 0.0f,180.0f,-0.6f });
		transTri2->SetScale({ 645.0f,180.0f,1.0f });
		transTri2->SetRotation({ 0.0f,0.0f,180.0f });
		Render2DComponent* rendTri2 = triangle2->AddComponent<Render2DComponent>();
		rendTri2->CreateMesh<TriangleMesh>();
		rendTri2->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");

		GameObject* triangle3 = GameObjectManager::AddUI("triangle3", "TitleUI");
		TransformComponent* transTri3 = triangle3->AddComponent<TransformComponent>();
		transTri3->SetPosition({ 320.0f,0.0f,-0.6f });
		transTri3->SetScale({ 360.0f,322.5f,1.0f });
		transTri3->SetRotation({ 0.0f,0.0f,90.0f });
		Render2DComponent* rendTri3 = triangle3->AddComponent<Render2DComponent>();
		rendTri3->CreateMesh<TriangleMesh>();
		rendTri3->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");

		GameObject* triangle4 = GameObjectManager::AddUI("triangle4", "TitleUI");
		TransformComponent* transTri4 = triangle4->AddComponent<TransformComponent>();
		transTri4->SetPosition({ -320.0f,0.0f,-0.6f });
		transTri4->SetScale({ 360.0f,322.5f,1.0f });
		transTri4->SetRotation({ 0.0f,0.0f,-90.0f });
		Render2DComponent* rendTri4 = triangle4->AddComponent<Render2DComponent>();
		rendTri4->CreateMesh<TriangleMesh>();
		rendTri4->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
	}

	GameObject* slashLineUI = GameObjectManager::AddUI("slashLineUI", "TitleUI");
	TransformComponent* transSlashLine = slashLineUI->AddComponent<TransformComponent>();
	transSlashLine->SetPosition({ -2280.0f,1280.0f,-0.6f });
	transSlashLine->SetScale({ 1080.0f,100.0f,1.0f });
	transSlashLine->SetRotation({ 0.0f,0.0f,-29.0f });
	VectorMoveComponent* lineMove = slashLineUI->AddComponent<VectorMoveComponent>();
	lineMove->SetMovePower(80.0f);
	lineMove->SetMoveDirection({ 1.8f,-1.0f,0.0f });
	TimeSpawnParticlesComponent* timeSp1 = slashLineUI->AddComponent<TimeSpawnParticlesComponent>();
	timeSp1->SetSpawnInterval(0.03f);
	Render2DComponent* rendSlashLine = slashLineUI->AddComponent<Render2DComponent>();
	rendSlashLine->CreateMesh<SquareMesh>();
	rendSlashLine->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
	rendSlashLine->ChangeTexture("assets/texture/LightLine.png");
	m_SlashUI1 = slashLineUI;

	GameObject* slashLineUI2 = GameObjectManager::AddUI("slashLineUI2", "TitleUI");
	TransformComponent* transSlashLine2 = slashLineUI2->AddComponent<TransformComponent>();
	transSlashLine2->SetPosition({ 2280.0f,1280.0f,-0.6f });
	transSlashLine2->SetScale({ 1080.0f,100.0f,1.0f });
	transSlashLine2->SetRotation({ 0.0f,0.0f,209.0f });
	VectorMoveComponent* lineMove2 = slashLineUI2->AddComponent<VectorMoveComponent>();
	lineMove2->SetMovePower(80.0f);
	lineMove2->SetMoveDirection({ -1.8f,-1.0f,0.0f });
	TimeSpawnParticlesComponent* timeSp2 = slashLineUI2->AddComponent<TimeSpawnParticlesComponent>();
	timeSp2->SetSpawnInterval(0.03f);
	Render2DComponent* rendSlashLine2 = slashLineUI2->AddComponent<Render2DComponent>();
	rendSlashLine2->CreateMesh<SquareMesh>();
	rendSlashLine2->SetShader("unlitTextureVS2D.hlsl", "shader/unlitTexturePS.hlsl");
	rendSlashLine2->ChangeTexture("assets/texture/LightLine.png");
	m_SlashUI2 = slashLineUI2;
}

void TitleAnimationControlComponent::Update() {



	// 仮おき
	// 終了時間
	if (m_RecordTime > 1.0f) {
		m_SlashUI1->SetActiveState(ActiveState::UPDATE_STOP);
		m_SlashUI2->SetActiveState(ActiveState::UPDATE_STOP);
	}
	m_RecordTime += DeltaTime;
}