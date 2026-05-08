#pragma once
#include "JsonComponent.h"
class EnemyJsonComponent :  public JsonComponent
{
private:
	std::vector<EnemyStatus> m_EnemyStatus;	// 敵キャラ情報のベクター

public:
	EnemyJsonComponent(GameObject& obj);
	~EnemyJsonComponent() = default;

	void Update() override;	// 更新処理

	void MakeSampleStatus() override;	// サンプルを作成

	nlohmann::ordered_json  EnemyToJson(const std::vector<EnemyStatus>& enemies);
	std::vector<EnemyStatus> LoadEnemyJsonFile(const std::string& filepath);

	std::vector<EnemyStatus> GetEnemyStatus() const { return m_EnemyStatus; } // 敵キャラ情報の取得
};