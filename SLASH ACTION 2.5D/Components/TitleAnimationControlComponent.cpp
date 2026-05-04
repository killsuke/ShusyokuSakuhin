#include "TitleAnimationControlComponent.h"
#include "Manager/GameObjectManager.h"
#include "TransformComponent.h"
#include "Render2DComponent.h"
#include "Render3DComponent.h"
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
#include "SoundComponent.h"

TitleAnimationControlComponent::TitleAnimationControlComponent(GameObject& obj) : Component(obj) {

	m_SortNum = ComponentTypeManager::GetID_FromName("TEST_MOVE"); // ソート番号を仮置き

	GameObject* titleUI = GameObjectManager::AddObject("titleUI", "TitleUI");
	TransformComponent* transTitle = titleUI->AddComponent<TransformComponent>();
	transTitle->SetPosition({ 0.0f,70.0f,0.0f });
	transTitle->SetScale({ 350.0f,200.0f,1.0f });
	Render3DComponent* rendTitle = titleUI->AddComponent<Render3DComponent>();
	rendTitle->CreateMesh<SquareMesh>();
	rendTitle->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	rendTitle->ChangeTexture("Slash_Action_Title.png");
	m_TitleRogo = titleUI;

	GameObject* sword3D = GameObjectManager::AddUI("sword3D", "TitleUI");
	TransformComponent* transSword = sword3D->AddComponent<TransformComponent>();
	transSword->SetPosition({ -1000.0f,-200.0f,-8.0f });
	transSword->SetScale({ 10.0f,500.0f,500.0f });
	transSword->SetRotation({ 0.0f,90.0f,0.0f });
	SoundComponent* soundSword = sword3D->AddComponent<SoundComponent>();
	soundSword->AddSoundLabel("sword_move");

	Render3DComponent* rendSword = sword3D->AddComponent<Render3DComponent>();
	rendSword->LoadModelMesh("assets/model/Copilot3D/sword3D.glb",
		"assets/model/Copilot3D");
	rendSword->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	m_Sword3D = sword3D;

	GameObject* startUI = GameObjectManager::AddObject("start", "TitleUI");
	TransformComponent* startTrans = startUI->AddComponent<TransformComponent>();
	startTrans->SetPosition({ 0.0f,-200.0f,-8.0f });
	startTrans->SetScale({ 130.0f,30.0f,1.0f });
	SoundComponent* soundStart = startUI->AddComponent<SoundComponent>();
	soundStart->AddSoundLabel("Sword_Title");

	Render3DComponent* startRend = startUI->AddComponent<Render3DComponent>();
	startRend->CreateMesh<SquareMesh>();
	startRend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	startRend->ChangeTexture("start.png");
	m_StartUI = startUI;
	m_StartUI->SetActiveState(ActiveState::ALL_STOP);

	GameObject* endUI = GameObjectManager::AddObject("end", "TitleUI");
	TransformComponent* endTrans = endUI->AddComponent<TransformComponent>();
	endTrans->SetPosition({ 0.0f,-290.0f,-8.0f });
	endTrans->SetScale({ 80.0f,30.0f,1.0f });
	Render3DComponent* endRend = endUI->AddComponent<Render3DComponent>();
	endRend->CreateMesh<SquareMesh>();
	endRend->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	endRend->ChangeTexture("end.png");
	m_EndUI = endUI;
	m_EndUI->SetActiveState(ActiveState::ALL_STOP);

	GameObject* miniSword = GameObjectManager::AddObject("miniSword", "TitleUI");
	TransformComponent* transMiniSword = miniSword->AddComponent<TransformComponent>();
	transMiniSword->SetPosition({ -200.0f,-200.0f,-8.0f });
	transMiniSword->SetScale({ 150.0f,150.0f,150.0f });
	transMiniSword->SetRotation({ 0.0f,90.0f,0.0f });
	SoundComponent* soundMiniSword = miniSword->AddComponent<SoundComponent>();
	soundMiniSword->AddSoundLabel("sword_sheath");

	Render3DComponent* rendMiniSword = miniSword->AddComponent<Render3DComponent>();
	rendMiniSword->LoadModelMesh("assets/model/Copilot3D/sword3D.glb",
		"assets/model/Copilot3D");
	rendMiniSword->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
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
		SoundComponent* sound1 = triangle1->AddComponent<SoundComponent>();
		sound1->AddSoundLabel("glass_breaking1");

		Render2DComponent* rendTri1 = triangle1->AddComponent<Render2DComponent>();
		rendTri1->CreateMesh<TriangleMesh>();
		rendTri1->SetShader("litTextureVS.hlsl", "unlitTexturePS.hlsl");
		m_Triangles[0] = triangle1;

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
		SoundComponent* sound2 = triangle2->AddComponent<SoundComponent>();
		sound2->AddSoundLabel("glass_breaking1");

		Render2DComponent* rendTri2 = triangle2->AddComponent<Render2DComponent>();
		rendTri2->CreateMesh<TriangleMesh>();
		rendTri2->SetShader("litTextureVS.hlsl", "unlitTexturePS.hlsl");
		m_Triangles[1] = triangle2;

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
		SoundComponent* sound3 = triangle3->AddComponent<SoundComponent>();
		sound3->AddSoundLabel("glass_breaking2");

		Render2DComponent* rendTri3 = triangle3->AddComponent<Render2DComponent>();
		rendTri3->CreateMesh<TriangleMesh>();
		rendTri3->SetShader("litTextureVS.hlsl", "unlitTexturePS.hlsl");
		m_Triangles[2] = triangle3;

		GameObject* triangle4 = GameObjectManager::AddObject("triangle4", "TitleUI");
		TransformComponent* transTri4 = triangle4->AddComponent<TransformComponent>();
		transTri4->SetPosition({ -320.0f,15.0f,-9.6f });
		transTri4->SetScale({ 400.0f,360.0f,1.0f });
		transTri4->SetRotation({ 0.0f,0.0f,-90.0f });
		ProjectileMotionComponent* projComp4 = triangle4->AddComponent<ProjectileMotionComponent>();
		projComp4->InitProjectile({ 0.0f, 5.0f, -20.0f }, { -0.5f,0.0f,0.0f }, 50.0f);
		projComp4->SetActiveFlag(false);
		RigidBodyComponent* rigidComp4 = triangle4->AddComponent<RigidBodyComponent>();
		rigidComp4->SetGravityFlag(true);
		rigidComp4->SetFallMagnification(36.0f);
		rigidComp4->SetMass(2.0f);
		rigidComp4->SetActiveFlag(false);
		rigidComp4->SetStopGravity(400.0f);
		SoundComponent* sound4 = triangle4->AddComponent<SoundComponent>();
		sound4->AddSoundLabel("glass_breaking2");

		Render2DComponent* rendTri4 = triangle4->AddComponent<Render2DComponent>();
		rendTri4->CreateMesh<TriangleMesh>();
		rendTri4->SetShader("litTextureVS.hlsl", "unlitTexturePS.hlsl");
		m_Triangles[3] = triangle4;
	}

	GameObject* slashLineUI = GameObjectManager::AddObject("slashLineUI", "TitleUI");
	TransformComponent* transSlashLine = slashLineUI->AddComponent<TransformComponent>();
	transSlashLine->SetPosition({ -2280.0f,1280.0f,-10.6f });
	transSlashLine->SetScale({ 1080.0f,100.0f,1.0f });
	transSlashLine->SetRotation({ 0.0f,0.0f,-29.0f });
	VectorMoveComponent* lineMove = slashLineUI->AddComponent<VectorMoveComponent>();
	lineMove->SetMovePower(90.0f);
	lineMove->SetMoveDirection({ 1.8f,-1.0f,0.0f });
	TimeSpawnParticlesComponent* timeSp1 = slashLineUI->AddComponent<TimeSpawnParticlesComponent>();
	timeSp1->SetSpawnInterval(0.03f);
	SoundComponent* sound1 = slashLineUI->AddComponent<SoundComponent>();
	sound1->AddSoundLabel("slash");

	Render2DComponent* rendSlashLine = slashLineUI->AddComponent<Render2DComponent>();
	rendSlashLine->CreateMesh<SquareMesh>();
	rendSlashLine->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	rendSlashLine->ChangeTexture("LightLine.png");
	m_SlashUI1 = slashLineUI;
	m_SlashUI1->SetActiveState(ActiveState::UPDATE_STOP);

	GameObject* slashLineUI2 = GameObjectManager::AddObject("slashLineUI2", "TitleUI");
	TransformComponent* transSlashLine2 = slashLineUI2->AddComponent<TransformComponent>();
	transSlashLine2->SetPosition({ 2280.0f,1280.0f,-10.6f });
	transSlashLine2->SetScale({ 1080.0f,100.0f,1.0f });
	transSlashLine2->SetRotation({ 0.0f,0.0f,209.0f });
	VectorMoveComponent* lineMove2 = slashLineUI2->AddComponent<VectorMoveComponent>();
	lineMove2->SetMovePower(90.0f);
	lineMove2->SetMoveDirection({ -1.8f,-1.0f,0.0f });
	TimeSpawnParticlesComponent* timeSp2 = slashLineUI2->AddComponent<TimeSpawnParticlesComponent>();
	timeSp2->SetSpawnInterval(0.03f);
	SoundComponent* sound2 = slashLineUI2->AddComponent<SoundComponent>();
	sound2->AddSoundLabel("slash");

	Render2DComponent* rendSlashLine2 = slashLineUI2->AddComponent<Render2DComponent>();
	rendSlashLine2->CreateMesh<SquareMesh>();
	rendSlashLine2->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
	rendSlashLine2->ChangeTexture("LightLine.png");
	m_SlashUI2 = slashLineUI2;
	m_SlashUI2->SetActiveState(ActiveState::UPDATE_STOP);

	TimeLineComponent* timeLine = m_Object->GetComponent<TimeLineComponent>();

	if (timeLine == nullptr) {
		timeLine = m_Object->AddComponent<TimeLineComponent>();
	}

	m_Events[0] = timeLine->AddPointDelayEvent(1.0f, this, [this]() {Slash1ActiveOn(); });				// 1.0秒後にスラッシュエフェクト1出現
	m_Events[1] = timeLine->AddPointDelayEvent(1.2f, this, [this]() {Slash2ActiveOn(); });				// 1.2秒後にスラッシュエフェクト2出現
	m_Events[2] = timeLine->AddPointDelayEvent(2.3f, this, [this]() {SlashesActiveOff(); });				// 2.3秒後にスラッシュエフェクト消去
	m_Events[3] = timeLine->AddPointDelayEvent(2.8f, this, [this]() {TrianglesActiveOn(); });				// 2.8秒後に三角形割れ開始
	m_Events[4] = timeLine->AddRangeDelayEvent(4.0f, 5.0f, 0.0f, this, [this](float) {SwordMove(); }, [this]() {SwordMoveStart(); }, [this]() {SwordSpecifiedPosition(); });	// 4.0秒から5.0秒まで剣移動
	m_Events[5] = timeLine->AddRangeDelayEvent(5.0f, 5.3f, 0.0f, this, [this](float) {LogoRotating(); }); // 5.0秒から5.3秒までロゴの回転
	m_Events[6] = timeLine->AddRangeDelayEvent(5.4f, 6.3f, 0.0f, this, [this](float) {SwordDown(); });	// 5.4秒から6.3秒まで剣を下に移動
	m_Events[7] = timeLine->AddPointDelayEvent(6.3f, this, [this]() {SwordSheath(); });
	m_Events[8] = timeLine->AddContinuousDelayEvent(6.3f, this, [this]() {GameStartWait(); });	// 5.4秒から6.3秒まで剣を下に移動
}

void TitleAnimationControlComponent::Update() {

	SkipAnimation();
}

void TitleAnimationControlComponent::Slash1ActiveOn() {

	if (m_SlashUI1 == nullptr) {
		return;
	}

	m_SlashUI1->SetActiveState(ActiveState::ACTIVE);
	SoundComponent* sound = m_SlashUI1->GetComponent<SoundComponent>();
	if (sound != nullptr) {
		sound->Play();
	}
}

void TitleAnimationControlComponent::Slash2ActiveOn() {

	if (m_SlashUI2 == nullptr) {
		return;
	}

	m_SlashUI2->SetActiveState(ActiveState::ACTIVE);
	SoundComponent* sound = m_SlashUI2->GetComponent<SoundComponent>();
	if (sound != nullptr) {
		sound->Play();
	}
}

void TitleAnimationControlComponent::SlashesActiveOff() {

	if (m_SlashUI1 == nullptr || m_SlashUI2 == nullptr) {
		return;
	}

	m_SlashUI1->SetActiveState(ActiveState::ALL_STOP);
	m_SlashUI2->SetActiveState(ActiveState::ALL_STOP);
}

// タイトルの画面を割るアニメーション開始
void TitleAnimationControlComponent::TrianglesActiveOn() {

	for (GameObject* const triangle : m_Triangles) {
		if (triangle == nullptr) {
			return;
		}
	}

	RigidBodyComponent* rigid1 = m_Triangles[0]->GetComponent<RigidBodyComponent>();
	RigidBodyComponent* rigid2 = m_Triangles[1]->GetComponent<RigidBodyComponent>();
	RigidBodyComponent* rigid3 = m_Triangles[2]->GetComponent<RigidBodyComponent>();
	RigidBodyComponent* rigid4 = m_Triangles[3]->GetComponent<RigidBodyComponent>();

	ProjectileMotionComponent* proj1 = m_Triangles[0]->GetComponent<ProjectileMotionComponent>();
	ProjectileMotionComponent* proj2 = m_Triangles[1]->GetComponent<ProjectileMotionComponent>();
	ProjectileMotionComponent* proj3 = m_Triangles[2]->GetComponent<ProjectileMotionComponent>();
	ProjectileMotionComponent* proj4 = m_Triangles[3]->GetComponent<ProjectileMotionComponent>();

	SoundComponent* sound1 = m_Triangles[0]->GetComponent<SoundComponent>();
	SoundComponent* sound2 = m_Triangles[1]->GetComponent<SoundComponent>();
	SoundComponent* sound3 = m_Triangles[2]->GetComponent<SoundComponent>();
	SoundComponent* sound4 = m_Triangles[3]->GetComponent<SoundComponent>();

	if (rigid1 == nullptr || rigid2 == nullptr || rigid3 == nullptr || rigid4 == nullptr ||
		proj1 == nullptr || proj2 == nullptr || proj3 == nullptr || proj4 == nullptr ||
		sound1 == nullptr || sound2 == nullptr || sound3 == nullptr || sound4 == nullptr) {
		return;
	}

	rigid1->SetActiveFlag(true);
	rigid2->SetActiveFlag(true);
	rigid3->SetActiveFlag(true);
	rigid4->SetActiveFlag(true);

	proj1->SetActiveFlag(true);
	proj2->SetActiveFlag(true);
	proj3->SetActiveFlag(true);
	proj4->SetActiveFlag(true);

	sound1->Play();
	sound2->Play();
	sound3->Play();
	sound4->Play();

	sound1->SetVolume(0.3f);
	sound2->SetVolume(0.3f);
	sound3->SetVolume(0.3f);
	sound4->SetVolume(0.3f);
}

void TitleAnimationControlComponent::SwordMoveStart() {

	if (m_Sword3D == nullptr) {
		return;
	}

	SoundComponent* sound = m_Sword3D->GetComponent<SoundComponent>();
	if (sound != nullptr) {
		sound->Play();
	}
}

void TitleAnimationControlComponent::SwordMove() {

	if (m_Sword3D == nullptr) {
		return;
	}

	TransformComponent* trans = m_Sword3D->GetComponent<TransformComponent>();
	if (trans != nullptr) {
		trans->AddPosition({ 40.0f,0.0f,0.0f });
	}
}

void TitleAnimationControlComponent::LogoRotating() {

	if (m_TitleRogo == nullptr) {
		return;
	}

	TransformComponent* trans = m_TitleRogo->GetComponent<TransformComponent>();
	if (trans != nullptr) {
		trans->AddRotation({ 0.0f,2.0f,0.0f });
	}
}

void TitleAnimationControlComponent::SwordSpecifiedPosition() {

	if (m_Sword3D == nullptr) {
		return;
	}

	TransformComponent* transSword = m_Sword3D->GetComponent<TransformComponent>();
	if (transSword != nullptr) {
		transSword->SetPosition({ 1200.0f,1200.0f,-30.0f });
		transSword->SetRotation({ 45.0f,-50.0f,0.0f });
		transSword->SetScale({ 10.0f,500.0f,800.0f });
	}
}

void TitleAnimationControlComponent::SwordDown() {

	if (m_Sword3D == nullptr) {
		return;
	}

	TransformComponent* trans = m_Sword3D->GetComponent<TransformComponent>();
	if (trans != nullptr) {
		trans->AddPosition({ -20.0f,-20.0f,0.0f });
	}
}

void TitleAnimationControlComponent::SwordSheath() {

	if (m_MiniSword3D == nullptr) {
		return;
	}

	SoundComponent* sound = m_MiniSword3D->GetComponent<SoundComponent>();
	if (sound != nullptr) {
		sound->Play();
		sound->SetVolume(0.8f);
	}
}

void TitleAnimationControlComponent::GameStartWait() {

	const bool IsEnter = (Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_A));
	const bool IsUporDown = (Input::GetKeyTrigger(VK_W) || Input::GetKeyTrigger(VK_S) || Input::GetKeyTrigger(VK_UP) || Input::GetKeyTrigger(VK_DOWN)
		|| Input::GetButtonTrigger(XINPUT_UP) || Input::GetButtonTrigger(XINPUT_DOWN));

	if (m_StartUI == nullptr || m_EndUI == nullptr || m_MiniSword3D == nullptr) {
		return;
	}

	SoundComponent* sound = m_StartUI->GetComponent<SoundComponent>();

	if (sound != nullptr) {
		sound->PlayOnce(0.0f);
		sound->SetMaxVolume(0.7f);
		sound->AddVolume(0.005f);
	}

	// ここで一連のアニメーション終了後の処理
	m_StartUI->SetActiveState(ActiveState::ACTIVE);
	m_EndUI->SetActiveState(ActiveState::ACTIVE);
	m_MiniSword3D->SetActiveState(ActiveState::ACTIVE);

	TransformComponent* trans = m_MiniSword3D->GetComponent<TransformComponent>();
	trans->AddRotation({ 0.0f,0.0f,2.0f });

	// ここで十字キー上下とかで選択
	if (IsUporDown == true) {
		m_IsMiniSwordUpDown = !m_IsMiniSwordUpDown;
	}

	if (m_IsMiniSwordUpDown == true) {
		trans->SetPosition({ -200.0f,-200.0f,-8.0f });
		// エンターキーを押してステージ1へ
		if (IsEnter == true)
		{
			if (sound != nullptr) {
				sound->Stop();
			}

			GameObject* fade = GameObjectManager::GameObjectFindNameUI("fade");
			DoorFadeComponent* door = fade->GetComponent<DoorFadeComponent>();
			door->SetBootDoor(true);

			m_IsloopAnim = false;
		}
	}
	else {
		trans->SetPosition({ -150.0f,-290.0f,-8.0f });
		if (IsEnter == true) {
			Application::GameEnd();
		}
	}

	m_IsUseSkipAnim = false;
}

void TitleAnimationControlComponent::SkipAnimation() {

	if (m_EndUI == nullptr || m_StartUI == nullptr || m_MiniSword3D == nullptr) {
		return;
	}

	if (m_IsloopAnim == true) {

		GameStartWait();
	}

	if (m_IsUseSkipAnim == false) {
		return;
	}

	const bool IsEnter = (Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_A));

	if (IsEnter == false) {

		return;
	}

	TimeLineComponent* line = m_Object->GetComponent<TimeLineComponent>();

	if(line == nullptr){
		return;
	}

	for (const uint32_t event : m_Events) {

		line->StopEvent(event);
	}

	// スラッシュUI
	SlashesActiveOff();

	// 割れた破片たち
	for (GameObject* triangle : m_Triangles) {

		if(triangle == nullptr){
			return;
		}

		triangle->SetActiveState(ActiveState::ALL_STOP);
	}

	// タイトルロゴを止める
	TransformComponent* trans = m_TitleRogo->GetComponent<TransformComponent>();
	
	if (trans == nullptr) {
		
		return;
	}

	trans->SetRotation({ 0.0f,42.0f,0.0f });

	// パーティクルたちをストップ
	std::vector<GameObject*> particles = GameObjectManager::GameObjectFindAllTag("Particle");
	for (GameObject* particle : particles) {

		if(particle == nullptr){
			return;
		}

		particle->SetActiveState(ActiveState::ALL_STOP);
	}

	// デカい剣を止める
	TransformComponent* transSword = m_Sword3D->GetComponent<TransformComponent>();

	if (transSword == nullptr) {
		
		return;
	}

	transSword->SetPosition({ 90.0f,100.0f,2.0f });
	transSword->SetRotation({ 45.0f,-50.0f,0.0f });
	transSword->SetScale({ 10.0f,500.0f,800.0f });

	m_IsloopAnim = true;

}