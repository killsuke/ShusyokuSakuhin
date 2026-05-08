#include "TerrainCreateComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Render3DComponent.h"
#include "Render3DColliderAABBComponent.h"
#include "Render3DColliderOBBComponent.h"
#include "RigidBodyComponent.h"
#include "Manager/GameObjectManager.h"
#include "Mesh/CubeMesh.h"
#include "Mesh/TerrainMesh.h"
#include "TerrainJsonComponent.h"
#include "MoveTerrainComponent.h"
#include "RoundShadowRenderComponent.h"
#include "RenderTerrainComponent.h"

using namespace DirectX;

namespace {
	XMFLOAT3 RIGHT = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3 LEFT = XMFLOAT3(-1.0f, 0.0f, 0.0f);
}

TerrainCreateComponent::TerrainCreateComponent(GameObject& obj) : CSVObjectManagerComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("CSV_OBJECT_MANAGER"); // ソート番号を設定
}

void TerrainCreateComponent::Update() {


}

void TerrainCreateComponent::CreateTerrains(std::vector<TerrainStatus> status, const float centerZ) {
	// この中で生成する
	if (m_CsvObjData.empty()) {
		return; // データがない場合は何もしない
	}

	unsigned int num = 0; // 生成したオブジェクトの数をカウント

	for (const CSV_Data& data : m_CsvObjData) {
		std::string kind = data.kind; // 地形の種類

		TerrainStatus tS;
		for (const TerrainStatus& t : status) {
			if (t.kind == kind) {
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

		GameObject* terrainObj = GameObjectManager::AddObject(name, "Terrain");
		TransformComponent* transform = terrainObj->AddComponent<TransformComponent>();
		transform->SetPosition({ data.position.x, data.position.y, centerZ });
		transform->SetScale(tS.scale);
		transform->SetRotation(tS.angle);

		if (kind != "T_Dummy") {

			ColliderComponent* collider = terrainObj->AddComponent<ColliderComponent>();
			collider->SetOffsetSizeAABB(XMFLOAT3(0.0f, 1.0f, 1.0f));

			if (kind == "T_MoveR") {
				MoveTerrainComponent* move = terrainObj->AddComponent<MoveTerrainComponent>();
				move->SetMoveSpeed(50.0f);
				move->SetMoveVector(RIGHT);
			}
			else if(kind == "T_MoveL") {
				MoveTerrainComponent* move = terrainObj->AddComponent<MoveTerrainComponent>();
				move->SetMoveSpeed(50.0f);
				move->SetMoveVector(LEFT);
			}

			RigidBodyComponent* rigid = terrainObj->AddComponent<RigidBodyComponent>();
			rigid->SetMass(2.0f);

			RenderTerrainComponent* render = terrainObj->AddComponent<RenderTerrainComponent>();
			render->SetShader(tS.shaderVS, tS.shaderPS);
			render->ChangeTexture(tS.texture);
			render->SetUVMagnification(XMFLOAT3(0.1f, 0.1f, 0.1f));
		}

		// デバッグ用のコライダー表示（場合によっては使うので、残しておく）
		//CubeMesh cubeMeshCollider;
		//auto renderCollider = terrainObj->AddComponent<Render3DColliderAABBComponent>();
		//renderCollider->SetMesh(cubeMeshCollider);
		//renderCollider->SetShader("unlitTextureVS.hlsl", "unlitTexturePS.hlsl");
		//renderCollider->SetTexture("NoTexture.png");
		//renderCollider->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f)); // 半透明の緑色

		num++;
	}
}