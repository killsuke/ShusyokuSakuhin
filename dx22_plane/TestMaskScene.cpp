#include "TestMaskScene.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Camera.h"
#include "RenderMask3D.h"
#include "SquareMesh.h"

TestMaskScene::TestMaskScene() {
	auto camera = GameObjectManager::AddObject("camera", "Camera");
	auto cameraTrans = camera->AddComponent<TransformComponent>();
	cameraTrans->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, -300.0f));
	camera->AddComponent<Camera>();

	auto testSquare = GameObjectManager::AddObject("testSquare", "TestSquare");
	auto testSquareTrans = testSquare->AddComponent<TransformComponent>();
	testSquareTrans->SetScale({ 100.0f, 100.0f, 1.0f });
	testSquareTrans->SetPosition({ 0.0f, 0.0f, 0.0f });
	SquareMesh squareMesh;	// 四角形のメッシュ
	auto testSquareRe = testSquare->AddComponent<RenderMask3DComponent>();
	testSquareRe->SetMesh(squareMesh);
	testSquareRe->SetShader("shader/unlitTextureVS.hlsl", "MaskTexturePS.hlsl");
	testSquareRe->SetTextureAndMask("assets/texture/title_car.png","assets/texture/mask.png");
	testSquareRe->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}

TestMaskScene::~TestMaskScene() {

}

void TestMaskScene::Update() {


}