#include "BonePartsComponent.h"
#include <iostream>
#include "Input/Input.h"

using namespace DirectX::SimpleMath;

BonePartsComponent::BonePartsComponent(GameObject& obj) : Component(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("ANIMATION_TRANSFORM"); // ソート番号を設定

	myPart.attachBone = std::make_unique<TestBone>();
	rlcom = m_Object->AddComponent<RenderLineComponent>();
	rlcom->SetStartAndEndPosition(startBonePos, endBonePos);
	rlcom->ChangeTexture("assets/texture/NoTexture.png");
	rlcom->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	rlcom->SetThickness(5.0f);

	startBoneLength = startBonePos.Length();
	endBoneLength = endBonePos.Length();

	startVec = startBonePos;
	startVec.Normalize();

	endVec = endBonePos;
	endVec.Normalize();
}

void BonePartsComponent::Update() {

	auto trans = m_Object->GetComponent<TransformComponent>();

	if (Input::GetKeyPress(VK_O)) {
		rote += 0.5f;
	}
	if (Input::GetKeyPress(VK_P)) {
		rote -= 0.5f;
	}

	rote -= 0.4f;

	// ワールド行列を計算
	if (myPart.attachBone->parent != nullptr) {

		if(m_referencePoint == nullptr) {
			return;
		}

		myPart.attachBone->localMat = MakeLocalMatrix(*trans);

		auto transPoint = m_referencePoint->GetComponent<TransformComponent>();
		auto posPoint = transPoint->GetPosition();
	
		
		// SRT情報作成
		// 各行列を生成
		Matrix r = Matrix::CreateFromQuaternion(myPart.attachBone->parent->quaternion);
		Matrix s = Matrix::CreateScale(Vector3(1.0f,1.0f,1.0f));
		Matrix t = Matrix::CreateTranslation(posPoint);

		// ワールド行列を作成し、保存
		Matrix pointMtx = s * r * t;

		auto transRot = trans->GetRotation();

		// 「 度 」から「 ラジアン 」へ変換
		float PitchRadians = DirectX::XMConvertToRadians(transRot.x); // X軸回転
		float YawRadians = DirectX::XMConvertToRadians(transRot.y);     // Y軸回転
		float RollRadians = DirectX::XMConvertToRadians(rote);   // Z軸回転

		// クォータニオン作成
		Quaternion childQ = Quaternion::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);

		// ピボット（回転中心）決め
		Matrix t1 = Matrix::CreateTranslation(-posPoint); // 親を原点に移動
		Matrix rot = Matrix::CreateFromQuaternion(childQ); // 回転
		Matrix t2 = Matrix::CreateTranslation(posPoint); // 元の位置に戻す

		// 親子関係を影響させること前提で回転後の行列を生成
		Matrix pivotRotation = t1 * rot * t2;

		// 親のワールド行列を回転で曲げた行列を生成
		Matrix rotatedParent = pointMtx * pivotRotation;

		// 回転を含めたワールド行列を生成
		Matrix rotatedWorld = myPart.attachBone->localMat * rotatedParent;

		myPart.attachBone->worldMat = rotatedWorld;
	}
	else {
		myPart.attachBone->worldMat = MakeWorldMatrix(*trans);
	}

	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;

	// ワールド行列を分解
	bool success = myPart.attachBone->worldMat.Decompose(scale, rotation, translation);
	if (success == true) {

		// 分解した情報をトランスフォームにセット
		trans->SetPosition(translation);

		myPart.attachBone->quaternion = rotation;

		// クォータニオン対応を急ぐ
		trans->SetQuaternion(rotation);

		Vector3 start = Vector3::Transform(startVec, rotation);
		Vector3 end = Vector3::Transform(endVec,rotation);

		rlcom->SetStartAndEndPosition(start * startBoneLength + translation, end * endBoneLength + translation);
	}
	else {
		std::cout << "失敗" << std::endl;
	}
}

// ※スケール成分を含めるとこの後の行列が狂い出すので、スケール成分は含めない
// ワールド行列を生成
DirectX::SimpleMath::Matrix BonePartsComponent::MakeWorldMatrix(const TransformComponent& trans) {

	Vector3 pos = trans.GetPosition();
	Vector3 rot = trans.GetRotation();

	// 「 度 」から「 ラジアン 」へ変換
	float PitchRadians = DirectX::XMConvertToRadians(rot.x); // X軸回転
	float YawRadians = DirectX::XMConvertToRadians(rot.y);     // Y軸回転
	float RollRadians = DirectX::XMConvertToRadians(rot.z);   // Z軸回転

	// クォータニオン作成
	Quaternion q = Quaternion::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);

	// SRT情報作成
	// 各行列を生成
	Matrix r = Matrix::CreateFromQuaternion(q);
	Matrix s = Matrix::CreateScale(Vector3(1.0f,1.0f,1.0f));
	Matrix t = Matrix::CreateTranslation(pos);

	return s * r * t;
}

// ローカル行列生成
DirectX::SimpleMath::Matrix BonePartsComponent::MakeLocalMatrix(const TransformComponent& trans) {

	Vector3 pos = trans.GetLocalPosition();
	Vector3 rot = trans.GetLocalRotation();

	// 「 度 」から「 ラジアン 」へ変換
	float PitchRadians = DirectX::XMConvertToRadians(rot.x); // X軸回転
	float YawRadians = DirectX::XMConvertToRadians(rot.y);     // Y軸回転
	float RollRadians = DirectX::XMConvertToRadians(rot.z);   // Z軸回転

	// クォータニオン作成
	Quaternion q = Quaternion::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);

	// SRT情報作成
	// 各行列を生成
	Matrix r = Matrix::CreateFromQuaternion(q);
	Matrix s = Matrix::CreateScale(Vector3(1.0f, 1.0f, 1.0f));
	Matrix t = Matrix::CreateTranslation(pos);

	return s * r * t;
}