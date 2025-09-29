#pragma once
#include "Transform.h"
#include <vector>
#include <memory>
#include <string>
#include <SimpleMath.h>

struct TestBone
{
	std::string name;
	TestBone* parent = nullptr;
	std::vector<TestBone*> children;
	DirectX::SimpleMath::Matrix localMat = DirectX::SimpleMath::Matrix::Identity;
	DirectX::SimpleMath::Matrix worldMat = DirectX::SimpleMath::Matrix::Identity;
	DirectX::SimpleMath::Vector3 position = { 0.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Vector3 rotation = { 0.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f };
};

struct Part
{
	std::string name = "";
	std::unique_ptr<TestBone> attachBone = nullptr;
};

class BonePartsComponent : public Component
{
private:
	Part myPart = {};
	float rote = 0.0f;

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

	void SetBonePosition(const DirectX::SimpleMath::Vector3& pos) {
		myPart.attachBone->position = pos;
	};
	void SetBoneRotation(const DirectX::SimpleMath::Vector3& rot) {
		myPart.attachBone->rotation = rot;
	};
	void SetBoneScale(const DirectX::SimpleMath::Vector3& scale) {
		myPart.attachBone->scale = scale;
	};

	TestBone* GetAttachBone() { return myPart.attachBone.get(); };

	DirectX::SimpleMath::Matrix MakeWorldMatrix(const TransformComponent& trans);
	DirectX::SimpleMath::Matrix MakeLocalMatrix(const TransformComponent& trans);
};