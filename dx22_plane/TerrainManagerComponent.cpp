#include "TerrainManagerComponent.h"
#include "Transform.h"
#include "Collider.h"
#include "Render3D.h"
#include "Render3DColliderAABBComponent.h"
#include "Render3DColliderOBBComponent.h"
#include "GameObjectManager.h"
#include "SquareMesh.h"
#include <string>

TerrainManagerComponent::TerrainManagerComponent(GameObject& obj) : Component(obj) {
	m_sortNum = TERRAIN_MANAGER; // ソート番号を設定
}

void TerrainManagerComponent::Update() {


}

void TerrainManagerComponent::CreateTerrain() {
	// この中で生成する

	if(m_terrainData.empty()) {
		return; // データがない場合は何もしない
	}

	unsigned int num = 0; // 生成したオブジェクトの数をカウント

	for(auto& data: m_terrainData) {
		std::string name = "terrain_" + std::to_string(num);
		
		auto terrainObj = GameObjectManager::AddObject(name,"Terrain");
		auto transform = terrainObj->AddComponent<TransformComponent>();
		transform->SetPosition({ data.position.x, data.position.y, 0.0f });
		transform->SetScale({ 5.0f,5.0f, 1.0f });
		auto collider = terrainObj->AddComponent<ColliderComponent>();
		collider->SetOffsetSizeAABB(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

		SquareMesh squareMesh;
		auto render = terrainObj->AddComponent<Render3DComponent>();
		render->SetMesh(squareMesh);
		render->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		render->SetTexture("assets/texture/NoTexture.png");

		SquareMesh squareMeshCollider;
		auto renderCollider = terrainObj->AddComponent<Render3DColliderAABBComponent>();
		renderCollider->SetMesh(squareMeshCollider);
		renderCollider->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		renderCollider->SetTexture("assets/texture/NoTexture.png");
		renderCollider->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f)); // 半透明の緑色

		num++;
	}

}