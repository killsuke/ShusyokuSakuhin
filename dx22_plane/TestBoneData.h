#pragma once
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
	DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::Identity;
};

struct Part
{
	std::string name = "";
	std::unique_ptr<TestBone> attachBone = nullptr;
};