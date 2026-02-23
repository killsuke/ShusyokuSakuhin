#pragma once
#include "CSVObjectManagerComponent.h"
#include <string>

class EnemyCreateComponent final :  public CSVObjectManagerComponent
{
public:
	EnemyCreateComponent(GameObject& obj);
	~EnemyCreateComponent() = default;

	void Update() override;

	void CreateEnemies(std::vector<EnemyStatus> status); // “G‚ğ¶¬‚·‚éŠÖ”
	void CreateKind(const std::string& kind, GameObject& obj);
};
