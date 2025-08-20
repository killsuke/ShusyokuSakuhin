#include "TerrainManagerComponent.h"
#include "Transform.h"
#include "Collider.h"
#include "Render3D.h"
#include "Render3DColliderAABBComponent.h"
#include "Render3DColliderOBBComponent.h"
#include "GameObjectManager.h"
#include "CubeMesh.h"
#include "TerrainJsonComponent.h"

TerrainManagerComponent::TerrainManagerComponent(GameObject& obj) : CSVObjectManagerComponent(obj) {
	m_sortNum = ComponentTypeManager::GetID_FromName("CSV_OBJECT_MANAGER"); // ソート番号を設定
}

void TerrainManagerComponent::Update() {


}

void TerrainManagerComponent::CreateTerrains(std::vector<TerrainStatus> status) {
	// この中で生成する
	if(m_csvObjData.empty()) {
		return; // データがない場合は何もしない
	}

	unsigned int num = 0; // 生成したオブジェクトの数をカウント

	for(auto& data: m_csvObjData) {
		std::string kind = data.kind; // 地形の種類

		TerrainStatus tS;
		for(auto& t : status) {
			if(t.kind == kind) {
				tS.kind = t.kind;
				tS.scale = t.scale;
				tS.angle = t.angle;
				tS.texture = t.texture;
				tS.shaderVS = t.shaderVS;
				tS.shaderPS = t.shaderPS;
				break;
			}
		}

		std::string name = "terrain_" + std::to_string(num);
		
		auto terrainObj = GameObjectManager::AddObject(name,"Terrain");
		auto transform = terrainObj->AddComponent<TransformComponent>();
		transform->SetPosition({ data.position.x, data.position.y, 0.0f });
		transform->SetScale(tS.scale);
		transform->SetRotation(tS.angle);

		auto collider = terrainObj->AddComponent<ColliderComponent>();
		collider->SetOffsetSizeAABB(DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f));

		CubeMesh cubeMesh;
		auto render = terrainObj->AddComponent<Render3DComponent>();
		render->SetMesh(cubeMesh);
		render->SetShader(tS.shaderVS,tS.shaderPS);
		render->SetTexture(tS.texture);

		CubeMesh cubeMeshCollider;
		auto renderCollider = terrainObj->AddComponent<Render3DColliderAABBComponent>();
		renderCollider->SetMesh(cubeMeshCollider);
		renderCollider->SetShader("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
		renderCollider->SetTexture("assets/texture/NoTexture.png");
		renderCollider->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f)); // 半透明の緑色

		num++;
	}

}