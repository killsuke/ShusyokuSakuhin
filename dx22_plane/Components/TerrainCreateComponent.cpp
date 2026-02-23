#include "TerrainCreateComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Render3D.h"
#include "Render3DColliderAABBComponent.h"
#include "Render3DColliderOBBComponent.h"
#include "RigidBodyComponent.h"
#include "Manager/GameObjectManager.h"
#include "Mesh/CubeMesh.h"
#include "TerrainJsonComponent.h"
#include "MoveTerrainComponent.h"

using namespace DirectX;

TerrainCreateComponent::TerrainCreateComponent(GameObject& obj) : CSVObjectManagerComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("CSV_OBJECT_MANAGER"); // ソート番号を設定
}

void TerrainCreateComponent::Update() {


}

void TerrainCreateComponent::CreateTerrains(std::vector<TerrainStatus> status, const float centerZ) {
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
		transform->SetPosition({ data.position.x, data.position.y, centerZ });
		transform->SetScale(tS.scale);
		transform->SetRotation(tS.angle);

		if (kind != "T_Dummy") {

			auto collider = terrainObj->AddComponent<ColliderComponent>();
			collider->SetOffsetSizeAABB(XMFLOAT3(0.0f, 1.0f, 1.0f));

			if (kind == "T_Move") {
				auto move = terrainObj->AddComponent<MoveTerrainComponent>();
				move->SetMoveSpeed(80.0f);
				move->SetMoveVector(XMFLOAT3(1.0f, 0.0f, 0.0f));
			}

			auto rigid = terrainObj->AddComponent<RigidBodyComponent>();
			rigid->SetMass(2.0f);

			auto render = terrainObj->AddComponent<Render3DComponent>();
			render->CreateMesh<CubeMesh>();
			render->SetShader(tS.shaderVS, tS.shaderPS);
			render->ChangeTexture(tS.texture);
		}

		//CubeMesh cubeMeshCollider;
		//auto renderCollider = terrainObj->AddComponent<Render3DColliderAABBComponent>();
		//renderCollider->SetMesh(cubeMeshCollider);
		//renderCollider->SetShader("ShaderResource/unlitTextureVS.hlsl", "ShaderResource/unlitTexturePS.hlsl");
		//renderCollider->SetTexture("assets/texture/NoTexture.png");
		//renderCollider->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f)); // 半透明の緑色

		num++;
	}

}