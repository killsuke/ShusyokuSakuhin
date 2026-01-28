#include "CutObjectActionComponent.h"
#include "Camera.h"
#include "RigidBodyComponent.h"
#include "ProjectileMotionComponent.h"
#include "Render2D.h"
#include "Render3D.h"
#include "CameraShakeComponent.h"
#include "Manager/GameObjectManager.h"
#include "Mesh/SquareMesh.h"
#include <random>

using namespace DirectX;

namespace {
	constexpr float DeltaTime = 0.016f;
	constexpr float FrontCameraZ = 20.0f;
	constexpr float CameraUpY = 25.0f;
	constexpr float ShakePower = 15.0f;
	constexpr float ShakeSpeed = 2.5f;
	constexpr float ShakeTime = 0.2f;
}

CutObjectActionComponent::CutObjectActionComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("DOOR_FADE"); // ソート番号を設定
}

void CutObjectActionComponent::Update() {

	GameObject* meshCutObj1 = GameObjectManager::GameObjectFindInstanceID(m_CutObj1ID);
	GameObject* meshCutObj2 = GameObjectManager::GameObjectFindInstanceID(m_CutObj2ID);

	if (meshCutObj1 == nullptr || meshCutObj2 == nullptr) {
		return; // 切断オブジェクトが存在しないなら何もしない
	}

	switch (m_DeathState)
	{
		// 切れて目の前に飛んで来る
	case EnemyDeathEventState::IMMEDIATE:

		ImmediateProcess(meshCutObj1, meshCutObj2);
		break;

		// 吹っ飛んでから画面に貼りつく
	case EnemyDeathEventState::STICKY:

		StickyProcess2(meshCutObj1, meshCutObj2);
		break;
	default:
		break;
	}

	m_RecordTime += DeltaTime;

}

void CutObjectActionComponent::ImmediateProcess(GameObject* obj1, GameObject* obj2) {


	// 一定時間経過したらオブジェクトを消去する
	if (m_RecordTime > 3.0f) {
		obj1->Destroy();
		obj2->Destroy();
		m_Object->Destroy();
	}
	// 最初の一回だけ動かす
	// 切れてズレる
	else if (m_RecordTime == 0.0f) {	// 切れたら即飛ばす！

		RigidBodyComponent* rigid1 = obj1->AddComponent<RigidBodyComponent>();
		RigidBodyComponent* rigid2 = obj2->AddComponent<RigidBodyComponent>();

		rigid1->SetGravityFlag(true);
		rigid2->SetGravityFlag(true);

		rigid1->SetMass(2.0f);
		rigid2->SetMass(2.0f);

		ProjectileMotionComponent* proj1 = obj1->AddComponent<ProjectileMotionComponent>();
		ProjectileMotionComponent* proj2 = obj2->AddComponent<ProjectileMotionComponent>();

		proj1->SetProjectileDirection({ -0.5f,1.0f,-1.8f });
		proj2->SetProjectileDirection({ 0.5f,1.0f,1.8f });

		proj1->SetProjectilePower(28.0f);
		proj2->SetProjectilePower(28.0f);

		proj1->SetProjectileRotation({ 0.0f,0.0f,15.0f });
		proj2->SetProjectileRotation({ 0.0f,0.0f,-15.0f });
	}
}

void CutObjectActionComponent::StickyProcess(GameObject* obj1, GameObject* obj2) {

	GameObject* camera = GameObjectManager::GameObjectFindName("camera");
	if (camera == nullptr) {
		return;
	}

	TransformComponent* camTrans = camera->GetComponent<TransformComponent>();
	TransformComponent* obj1Trans = obj1->GetComponent<TransformComponent>();
	TransformComponent* obj2Trans = obj2->GetComponent<TransformComponent>();

	if (camTrans == nullptr || obj1Trans == nullptr || obj2Trans == nullptr) {
		return;
	}


	VectorMoveComponent* move1 = obj1->GetComponent<VectorMoveComponent>();
	VectorMoveComponent* move2 = obj2->GetComponent<VectorMoveComponent>();

	if (move1 == nullptr || move2 == nullptr) {
		obj1->Destroy();
		obj2->Destroy();
		return;
	}

	if (m_RecordTime > 3.0f) {
		obj1->Destroy();
		obj2->Destroy();
		m_Object->Destroy();
	}
	else if (m_RecordTime > 1.5f) {	// 落ちる
		RigidBodyComponent* rigid1 = obj1->GetComponent<RigidBodyComponent>();
		RigidBodyComponent* rigid2 = obj2->GetComponent<RigidBodyComponent>();

		rigid1->SetGravityFlag(true);
		rigid2->SetGravityFlag(true);

		rigid1->SetActiveFlag(true);
		rigid2->SetActiveFlag(true);

		obj1Trans->AddRotation({ 0.0f,0.0f,5.0f });
		obj2Trans->AddRotation({ 0.0f,0.0f,-5.0f });
	}
	else if (m_RecordTime > 1.3f) {	// 震えを止める
	}
	else if (m_RecordTime > 1.0f) {	// 貼りついて揺らす

		const XMFLOAT3 camPos = camTrans->GetPosition();

		obj1Trans->SetPosition({ camPos.x + m_CutObj1Pos.x,camPos.y + m_CutObj1Pos.y,camPos.z + FrontCameraZ });
		obj2Trans->SetPosition({ camPos.x + m_CutObj2Pos.x,camPos.y + m_CutObj2Pos.y,camPos.z + FrontCameraZ });

		if (m_IsFirstCamPos == true) {
			m_IsFirstCamPos = false;

			Render2DComponent* render1 = obj1->GetComponent<Render2DComponent>();
			Render2DComponent* render2 = obj2->GetComponent<Render2DComponent>();

			if (render1 == nullptr || render2 == nullptr) {
				obj1->Destroy();
				obj2->Destroy();
				return;
			}
			render1->SetColor({ 0.3f,0.3f,0.3f,1.0f });
			render2->SetColor({ 0.3f,0.3f,0.3f,1.0f });

			// 画面揺れ開始
			CameraShakeComponent* camShake = camera->GetComponent<CameraShakeComponent>();
			if (camShake != nullptr) {
				camShake->ShakingPreparation(150.0f, 4.0f, 0.3f);
				camShake->SetShakeType(ShakeType::RANDOM_DEPTH);
			}
			return;
		}

		ShakeCutObjects(obj1Trans, obj2Trans);	// 揺らす
	}
	else if (m_RecordTime > 0.8f) {	// ここで出てくる

		const XMFLOAT3 camPos = camTrans->GetPosition();

		// 最初の一回だけカメラ上部に張り付ける
		if (m_IsFirstCamPos == false) {
			move1->SetActiveFlag(false);
			move2->SetActiveFlag(false);
			obj1Trans->SetPosition({ camPos.x, camPos.y + CameraUpY, camPos.z + FrontCameraZ });
			obj2Trans->SetPosition({ camPos.x, camPos.y + CameraUpY, camPos.z + FrontCameraZ });
			m_IsFirstCamPos = true;

			m_CutObj1Pos.y = CameraUpY;
			m_CutObj2Pos.y = CameraUpY;
			return;
		}

		std::random_device rd;  // 非決定的な乱数の種
		std::mt19937 gen(rd()); // メルセンヌ・ツイスタ
		std::uniform_real_distribution<float> dist(0.0f, 5.0f); // 範囲指定

		const float value = dist(gen);

		m_CutObj1Pos.x -= value;
		m_CutObj2Pos.x += value;

		m_CutObj1Pos.y -= value;
		m_CutObj2Pos.y -= value;

		obj1Trans->SetPosition({ camPos.x + m_CutObj1Pos.x,camPos.y + m_CutObj1Pos.y,camPos.z + FrontCameraZ });
		obj2Trans->SetPosition({ camPos.x + m_CutObj2Pos.x,camPos.y + m_CutObj2Pos.y,camPos.z + FrontCameraZ });

	}
	else if (m_RecordTime == 0.0f) { // 天高く飛ばす

		std::random_device rd;  // 非決定的な乱数の種
		std::mt19937 gen(rd()); // メルセンヌ・ツイスタ
		std::uniform_real_distribution<float> dist(8.0f, 10.0f); // 範囲指定

		const float value = dist(gen);

		move1->SetMoveDirection({ 0.2f,1.0f,0.0f });
		move1->SetMovePower(3.5f);
		move1->SetRotationValue({ 0.0f,0.0f,value });

		move2->SetMoveDirection({ -0.2f,1.0f,0.0f });
		move2->SetMovePower(3.5f);
		move2->SetRotationValue({ 0.0f,0.0f,-value });

		RigidBodyComponent* rigid1 = obj1->AddComponent<RigidBodyComponent>();
		RigidBodyComponent* rigid2 = obj2->AddComponent<RigidBodyComponent>();

		rigid1->SetMass(2.0f);
		rigid2->SetMass(2.0f);

		rigid1->SetGravityFlag(false);
		rigid2->SetGravityFlag(false);

		rigid1->SetActiveFlag(false);
		rigid2->SetActiveFlag(false);

		rigid1->SetFallMagnification(6.0f);
		rigid2->SetFallMagnification(6.0f);

		rigid1->SetFirstFallMagnification(60.0f);
		rigid2->SetFirstFallMagnification(60.0f);
	}
}

void CutObjectActionComponent::ShakeCutObjects(TransformComponent* obj1, TransformComponent* obj2) {

	std::random_device rd;  // 非決定的な乱数の種
	std::mt19937 gen(rd()); // メルセンヌ・ツイスタ
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f); // 範囲指定

	const float rX = dist(gen); // 0.0 ～ 1.0 の乱数
	const float rY = dist(gen); // 0.0 ～ 1.0 の乱数

	m_ShakeVector = XMVectorSet(rX, rY, 0.0f, 0.0f);
	m_ShakeVector = XMVector3Normalize(m_ShakeVector);


	// ランダム揺れ用オフセット
	const float offsetX = sinf(m_RecordTime * 50.0f) * 1.5f;

	// 正規化したRightベクトルをオフセットに適用
	const XMVECTOR offset = XMVectorScale(m_ShakeVector, offsetX);

	// 前フレームとの差分を取る
	// サイン波がゼロに戻るときに差分もゼロになるので、
	// 最終的に元の位置に戻る
	const XMVECTOR frameOffset = XMVectorSubtract(offset, m_PrevShakeOffset);

	m_PrevShakeOffset = offset;

	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, frameOffset);

	obj1->AddPosition({ newPos.x,newPos.y,0.0f });
	obj2->AddPosition({ newPos.x,newPos.y,0.0f });
}

void CutObjectActionComponent::StickyProcess2(GameObject* obj1, GameObject* obj2) {

	GameObject* camera = GameObjectManager::GameObjectFindName("camera");
	if (camera == nullptr) {
		return;
	}

	TransformComponent* camTrans = camera->GetComponent<TransformComponent>();
	TransformComponent* obj1Trans = obj1->GetComponent<TransformComponent>();
	TransformComponent* obj2Trans = obj2->GetComponent<TransformComponent>();
	Camera* cameraComp = camera->GetComponent<Camera>();

	if (camTrans == nullptr || obj1Trans == nullptr || obj2Trans == nullptr || cameraComp == nullptr) {
		return;
	}


	if (m_RecordTime > 3.0f) {
		obj1->Destroy();
		obj2->Destroy();
		m_Crack1->Destroy();
		m_Crack2->Destroy();

		for (GameObject* debris : m_Debris) {
			debris->Destroy();
		}
		m_Debris.clear();

		m_Object->Destroy();
	}
	else if (m_RecordTime > 1.9f) {	// 落ちる
		RigidBodyComponent* rigid1 = obj1->GetComponent<RigidBodyComponent>();
		RigidBodyComponent* rigid2 = obj2->GetComponent<RigidBodyComponent>();

		rigid1->SetGravityFlag(true);
		rigid2->SetGravityFlag(true);

		rigid1->SetActiveFlag(true);
		rigid2->SetActiveFlag(true);

		obj1Trans->AddRotation({ 0.0f,0.0f,5.0f });
		obj2Trans->AddRotation({ 0.0f,0.0f,-5.0f });

		Render2DComponent* rend1 = m_Crack1->GetComponent<Render2DComponent>();
		Render2DComponent* rend2 = m_Crack2->GetComponent<Render2DComponent>();

		rend1->AddColor_A(-0.05f);
		rend2->AddColor_A(-0.05f);

	}
	else if (m_RecordTime > 1.5f) {	// 震えを止める
	}
	else if (m_RecordTime > 1.3f) {	// 貼りついて揺らす

		const XMFLOAT3 camPos = camTrans->GetPosition();
		const XMFLOAT3 camOffset = cameraComp->GetOffsetPosition();

		obj1Trans->SetPosition({ camPos.x + m_CutObj1Pos.x,camPos.y + m_CutObj1Pos.y,camPos.z + camOffset.z + FrontCameraZ });
		obj2Trans->SetPosition({ camPos.x + m_CutObj2Pos.x,camPos.y + m_CutObj2Pos.y,camPos.z + camOffset.z + FrontCameraZ });

		XMFLOAT3 pos1 = obj1Trans->GetPosition();
		XMFLOAT3 pos2 = obj2Trans->GetPosition();

		if (m_IsFirstCamPos == true) {
			m_IsFirstCamPos = false;

			Render2DComponent* render1 = obj1->GetComponent<Render2DComponent>();
			Render2DComponent* render2 = obj2->GetComponent<Render2DComponent>();

			if (render1 == nullptr || render2 == nullptr) {

				obj1->Destroy();
				obj2->Destroy();
				return;
			}
			render1->SetColor({ 0.3f,0.3f,0.3f,1.0f });
			render2->SetColor({ 0.3f,0.3f,0.3f,1.0f });

			// 画面揺れ開始
			CameraShakeComponent* camShake = camera->GetComponent<CameraShakeComponent>();
			if (camShake != nullptr) {
				camShake->ShakingPreparation(ShakePower, ShakeSpeed, ShakeTime);
				camShake->SetShakeType(ShakeType::RANDOM_DEPTH_ATTENUATION);
			}

			XMFLOAT3 size1 = obj1Trans->GetScale();
			XMFLOAT3 size2 = obj2Trans->GetScale();

			pos1.z -= 1.0f;
			pos2.z -= 1.0f;

			// ここにヒビが入ったテクスチャを貼ってみる
			m_Crack1 = GameObjectManager::AddObject("crack1", "Effect");
			TransformComponent* crackTrans1 = m_Crack1->AddComponent<TransformComponent>();
			crackTrans1->SetPosition(pos1);
			crackTrans1->SetScale(size1);
			Render2DComponent* rend1 = m_Crack1->AddComponent<Render2DComponent>();
			rend1->CreateMesh<SquareMesh>();
			rend1->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
			rend1->SetColor(XMFLOAT4(3.0f, 3.0f, 3.0f, 1.0f));
			rend1->ChangeTexture("assets/texture/crack.png");

			m_Crack2 = GameObjectManager::AddObject("crack2", "Effect");
			TransformComponent* crackTrans2 = m_Crack2->AddComponent<TransformComponent>();
			crackTrans2->SetPosition(pos2);
			crackTrans2->SetScale(size2);
			Render2DComponent* rend2 = m_Crack2->AddComponent<Render2DComponent>();
			rend2->CreateMesh<SquareMesh>();
			rend2->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
			rend2->SetColor(XMFLOAT4(3.0f, 3.0f, 3.0f, 1.0f));
			rend2->ChangeTexture("assets/texture/crack.png");


			std::random_device rd;  // 非決定的な乱数の種
			std::mt19937 gen(rd()); // メルセンヌ・ツイスタ
			std::uniform_int_distribution<int> dist(2, 6); // 破片の数

			std::uniform_real_distribution<float> distFloat(1.0f, 1.5f);	// Ⅹ方向に対しての破片の飛ぶ向き
			std::uniform_real_distribution<float> distFloat2(5.0f, 8.0f);	// どのぐらいの力で進むか
			std::uniform_real_distribution<float> distFloat3(5.0f, 8.0f);	// 回転

			const int debrisNum = dist(gen);

			XMFLOAT3 testScale = { 1.0f,1.0f,1.0f };

			// 左側に飛んで行った方
			for (int i = 0; i < debrisNum; ++i) {

				GameObject* debris = GameObjectManager::AddObject("debrisLeft", "Debris");
				TransformComponent* debrisTrans = debris->AddComponent<TransformComponent>();
				debrisTrans->SetPosition(pos1);
				debrisTrans->SetScale(testScale);

				RigidBodyComponent* rigid = debris->AddComponent<RigidBodyComponent>();
				rigid->SetGravityFlag(true);
				rigid->SetMass(2.0f);
				rigid->SetFirstFallMagnification(45.0f);
				rigid->SetFallMagnification(4.5f);

				const float dirX = distFloat(gen);
				const float powerR = distFloat2(gen);
				const float angle = distFloat3(gen);

				ProjectileMotionComponent* proj = debris->AddComponent<ProjectileMotionComponent>();
				proj->SetProjectileDirection({ dirX,1.0f,-1.0f });
				proj->SetProjectilePower(powerR);
				proj->SetProjectileRotation({ 0.0f,-1.5f,angle });

				Render3DComponent* rend = debris->AddComponent<Render3DComponent>();
				rend->CreateMesh<SquareMesh>();
				rend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
				rend->ChangeTexture("assets/texture/glass_shards.png");

				m_Debris.push_back(debris);
			}

			// 右側に飛んで行った方
			for (int i = 0; i < debrisNum; ++i) {

				GameObject* debris = GameObjectManager::AddObject("debrisRight", "Debris");
				TransformComponent* debrisTrans = debris->AddComponent<TransformComponent>();
				debrisTrans->SetPosition(pos2);
				debrisTrans->SetScale(testScale);

				RigidBodyComponent* rigid = debris->AddComponent<RigidBodyComponent>();
				rigid->SetGravityFlag(true);
				rigid->SetMass(2.0f);
				rigid->SetFirstFallMagnification(45.0f);
				rigid->SetFallMagnification(4.5f);

				const float dirX = distFloat(gen);
				const float powerR = distFloat2(gen);
				const float angle = distFloat3(gen);

				ProjectileMotionComponent* proj = debris->AddComponent<ProjectileMotionComponent>();
				proj->SetProjectileDirection({ -dirX,1.0f,-1.0f });
				proj->SetProjectilePower(powerR);
				proj->SetProjectileRotation({ 0.0f,1.5f,-angle });

				Render3DComponent* rend = debris->AddComponent<Render3DComponent>();
				rend->CreateMesh<SquareMesh>();
				rend->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
				rend->ChangeTexture("assets/texture/glass_shards.png");

				m_Debris.push_back(debris);
			}

			return;
		}

		TransformComponent* crack1 = m_Crack1->GetComponent<TransformComponent>();
		TransformComponent* crack2 = m_Crack2->GetComponent<TransformComponent>();

		pos1.z -= 1.0f;
		pos2.z -= 1.0f;

		crack1->SetPosition(pos1);
		crack2->SetPosition(pos2);

		//ShakeCutObjects(obj1Trans, obj2Trans);	// 揺らす
	}
	else if (m_RecordTime >= 0.0f) { // 前に飛ばす

		if (m_IsFirstCamPos == false) {
			m_IsFirstCamPos = true;

			std::random_device rd;  // 非決定的な乱数の種
			std::mt19937 gen(rd()); // メルセンヌ・ツイスタ
			std::uniform_real_distribution<float> dist1(15.0f, 25.0f); // 範囲指定
			std::uniform_real_distribution<float> dist2(-10.0f, 10.0f); // 範囲指定

			const float value1 = dist1(gen);

			const float value2 = dist2(gen);
			const float value3 = dist2(gen);

			m_CutObj1Pos = XMFLOAT3(-value1, value2, FrontCameraZ);
			m_CutObj2Pos = XMFLOAT3(value1, value3, FrontCameraZ);

			RigidBodyComponent* rigid1 = obj1->AddComponent<RigidBodyComponent>();
			RigidBodyComponent* rigid2 = obj2->AddComponent<RigidBodyComponent>();

			rigid1->SetMass(2.0f);
			rigid2->SetMass(2.0f);

			rigid1->SetGravityFlag(false);
			rigid2->SetGravityFlag(false);

			rigid1->SetActiveFlag(false);
			rigid2->SetActiveFlag(false);

			rigid1->SetFallMagnification(6.0f);
			rigid2->SetFallMagnification(6.0f);

			rigid1->SetFirstFallMagnification(60.0f);
			rigid2->SetFirstFallMagnification(60.0f);
		}

		const XMFLOAT3 camPos = camTrans->GetPosition();

		// 最初の一回だけカメラ上部に張り付ける

		const XMFLOAT3 cut1TargetPos = camPos + m_CutObj1Pos;
		const XMFLOAT3 cut2TargetPos = camPos + m_CutObj2Pos;

		const XMFLOAT3 obj1Pos = obj1Trans->GetPosition();
		const XMFLOAT3 obj2Pos = obj2Trans->GetPosition();

		const XMVECTOR obj1CurrentPos = XMLoadFloat3(&obj1Pos);
		const XMVECTOR obj2CurrentPos = XMLoadFloat3(&obj2Pos);
		const XMVECTOR obj1TargetPos = XMLoadFloat3(&cut1TargetPos);
		const XMVECTOR obj2TargetPos = XMLoadFloat3(&cut2TargetPos);

		// 方向ベクトル
		XMVECTOR dir1 = XMVectorSubtract(obj1TargetPos, obj1CurrentPos);
		XMVECTOR dir2 = XMVectorSubtract(obj2TargetPos, obj2CurrentPos);

		// 距離チェック
		const float distance1 = XMVectorGetX(XMVector3Length(dir1));
		const float distance2 = XMVectorGetX(XMVector3Length(dir2));

		if (distance1 < 10.0f || distance2 < 10.0f) {
			m_RecordTime = 1.3f; // 次のステップへ
			return;
		}

		// 正規化
		dir1 = XMVector3Normalize(dir1);
		dir2 = XMVector3Normalize(dir2);

		XMVECTOR moveVec1 = XMVectorScale(dir1, distance1 * 10.0f * DeltaTime);
		XMVECTOR moveVec2 = XMVectorScale(dir2, distance2 * 10.0f * DeltaTime);

		XMVECTOR pos1 = XMVectorAdd(obj1CurrentPos, moveVec1);
		XMVECTOR pos2 = XMVectorAdd(obj2CurrentPos, moveVec2);

		XMFLOAT3 newPos1;
		XMStoreFloat3(&newPos1, pos1);

		XMFLOAT3 newPos2;
		XMStoreFloat3(&newPos2, pos2);

		obj1Trans->SetPosition(newPos1);
		obj2Trans->SetPosition(newPos2);

		obj1Trans->AddRotation({ 0.0f,0.0f,30.0f });
		obj2Trans->AddRotation({ 0.0f,0.0f,-30.0f });
	}
}