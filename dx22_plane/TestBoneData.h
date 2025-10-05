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
	DirectX::SimpleMath::Vector3 position = { 0.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Vector3 rotation = { 0.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f };
};

struct Part
{
	std::string name = "";
	std::unique_ptr<TestBone> attachBone = nullptr;
};