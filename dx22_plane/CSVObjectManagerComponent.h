#pragma once
#include "Component.h"
#include "CSV_Data.h"
#include "ObjectStatus.h"
#include <vector>
#include <string>
#include <utility>

class CSVObjectManagerComponent : public Component
{
protected:
	std::vector<CSV_Data> m_csvObjData; // CSVファイルから読み取ったデータを格納する2次元ベクター

	CSVObjectManagerComponent(GameObject& obj);
	~CSVObjectManagerComponent() = default;

private:

public:

	void Update() override;

	inline void SetCsvObjData(std::vector<CSV_Data>&& data) {
		m_csvObjData = std::move(data); // 地形データをセット
	}
};
