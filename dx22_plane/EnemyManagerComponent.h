#pragma once
#include "CSVObjectManagerComponent.h"
#include <string>

class EnemyManagerComponent final :  public CSVObjectManagerComponent
{
private:

public:
	EnemyManagerComponent(GameObject& obj);
	~EnemyManagerComponent() = default;

	void Update() override;

	void CreateEnemies(std::vector<EnemyStatus> status); // “G‚ğ¶¬‚·‚éŠÖ”
	void CreateKind(const std::string& kind, GameObject& obj);
};
