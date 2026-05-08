#include "TestMaskScene.h"
#include "Manager/GameObjectManager.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "Components/RenderMask3DComponent.h"
#include "Mesh/SquareMesh.h"

TestMaskScene::TestMaskScene() {
	auto camera = GameObjectManager::AddObject("camera", "Camera");
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, -300.0f));
	camera->AddComponent<CameraComponent>();

	auto testSquare = GameObjectManager::AddObject("testSquare", "TestSquare");
	auto testSquareTrans = testSquare->AddComponent<TransformComponent>();
	testSquareTrans->SetScale({ 100.0f, 100.0f, 1.0f });
	testSquareTrans->SetPosition({ 0.0f, 0.0f, 0.0f });

	auto testSquareRe = testSquare->AddComponent<RenderMask3DComponent>();
	testSquareRe->CreateMesh<SquareMesh>();
	testSquareRe->SetShader("unlitTextureVS.hlsl", "MaskTexturePS.hlsl");
//	testSquareRe->SetTextureAndMask("title_car.png","mask.png");
	testSquareRe->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}

TestMaskScene::~TestMaskScene() {

}

void TestMaskScene::Update() {


}