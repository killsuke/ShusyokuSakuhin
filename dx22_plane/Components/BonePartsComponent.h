#pragma once
#include "Transform.h"
#include "RenderLineComponent.h"
#include "Structs/TestBoneData.h"
#include <vector>
#include <memory>
#include <string>
#include <SimpleMath.h>

class BonePartsComponent : public Component
{
private:
	Part myPart = {};
	float rote = 0.0f;
	RenderLineComponent* rlcom = nullptr;
	DirectX::SimpleMath::Vector3 startBonePos = { -50.0f,0.0f,0.0f };
	DirectX::SimpleMath::Vector3 endBonePos = { 50.0f,0.0f,0.0f };
	DirectX::SimpleMath::Vector3 startVec = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 endVec = DirectX::SimpleMath::Vector3::Zero;
	float startBoneLength = 50.0f; // ボーンの長さ
	float endBoneLength = 50.0f; // ボーンの長さ
	GameObject* m_referencePoint = nullptr; // 参照点

public:
	BonePartsComponent(GameObject& obj);
	~BonePartsComponent() = default;
	void Update() override;

	void SetPartName(const std::string& name) {
		myPart.name = name;
		myPart.attachBone->name = name;
	};

	void SetBoneParent(TestBone* parentBone) {
		myPart.attachBone->parent = parentBone;
	};

	void AddBoneChildren(TestBone* childBone) {
		myPart.attachBone->children.push_back(childBone);
	};

	void SetStartBonePosition(const DirectX::SimpleMath::Vector3& pos) {
		startBonePos = pos;
		startBoneLength = startBonePos.Length();
		startVec = startBonePos;
		startVec.Normalize();
		rlcom->SetStartPosition(pos);
	};

	void SetEndBonePosition(const DirectX::SimpleMath::Vector3& pos) {
		endBonePos = pos;
		endBoneLength = endBonePos.Length();
		endVec = endBonePos;
		endVec.Normalize();
		rlcom->SetEndPosition(pos);
	};

	void SetReferencePoint(GameObject* obj) {
		m_referencePoint = obj;
	};

	TestBone* GetAttachBone() { return myPart.attachBone.get(); };

	DirectX::SimpleMath::Matrix MakeWorldMatrix(const TransformComponent& trans);
	DirectX::SimpleMath::Matrix MakeLocalMatrix(const TransformComponent& trans);

	GameObject* GetStartPoint()const { return rlcom->GetStartPoint(); };
	GameObject* GetEndPoint()const { return rlcom->GetEndPoint(); };
};