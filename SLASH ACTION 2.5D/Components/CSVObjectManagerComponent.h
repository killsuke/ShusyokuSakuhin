#pragma once
#include "Component.h"
#include "Structs/CSV_Data.h"
#include "Structs/ObjectStatus.h"
#include <vector>
#include <string>
#include <utility>

class CSVObjectManagerComponent : public Component
{
protected:
	std::vector<CSV_Data> m_CsvObjData; // CSVファイルから読み取ったデータを格納する2次元ベクター

	CSVObjectManagerComponent(GameObject& obj);
	~CSVObjectManagerComponent() = default;

private:

public:

	void Update() override;

	void SetCsvObjData(std::vector<CSV_Data>&& data) {
		m_CsvObjData = std::move(data); // 地形データをセット
	}
};
