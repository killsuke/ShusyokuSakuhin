#pragma once
#include "CSVObjectManagerComponent.h"

class EnemyManagerComponent :  public CSVObjectManagerComponent
{
private:

public:
	EnemyManagerComponent(GameObject& obj);
	~EnemyManagerComponent() = default;

	void Update() override;

	void CreateEnemies(std::vector<EnemyStatus> status); // “G‚ğ¶¬‚·‚éŠÖ”
};
