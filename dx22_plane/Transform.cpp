#include "Transform.h"

// ベクター内にエンティティ追加
void TransformSystem::AddComponent(uint32_t entity, const TransformComponent& comp) {
	components.emplace(entity, comp);	// ２つの要素をまとめて挿入
}

// エンティティからコンポーネントを削除
void TransformSystem::RemoveComponent(uint32_t entity) {
	size_t removed = components.erase(entity);	// 削除された要素を取得
	if (removed == 0) {
		std::cout << "Warning : Entity" << entity << "Not found in TransformSystem!!" << std::endl;
	}
}

// トランスフォームのコンポーネント取得
TransformComponent* TransformSystem::GetComponent(uint32_t entity) {
	auto it = components.find(entity);
	if (it != components.end()) {
		return &it->second;
	}
	return nullptr;
}

// 更新処理
void TransformSystem::Update() {
	for (auto& pair : components) {
		TransformComponent& transform = pair.second;	// 参照で取得、unorderd_mapのため、２つセットの内の２つ目を参照、なのでsecond

		// 「 度 」から「 ラジアン 」へ変換
		float PitchRadians = DirectX::XMConvertToRadians(transform.m_Rotation.x); // X軸回転
		float YawRadians = DirectX::XMConvertToRadians(transform.m_Rotation.y);     // Y軸回転
		float RollRadians = DirectX::XMConvertToRadians(transform.m_Rotation.z);   // Z軸回転

		// SRT情報作成
		DirectX::SimpleMath::Matrix r = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(YawRadians, PitchRadians, RollRadians);
		DirectX::SimpleMath::Matrix t = DirectX::SimpleMath::Matrix::CreateTranslation(transform.m_Position.x, transform.m_Position.y, transform.m_Position.z);
		DirectX::SimpleMath::Matrix s = DirectX::SimpleMath::Matrix::CreateScale(transform.m_Scale.x, transform.m_Scale.y, transform.m_Scale.z);

		// ワールド行列を作成し、保存
		transform.worldMatrix = s * r * t;
	}
}