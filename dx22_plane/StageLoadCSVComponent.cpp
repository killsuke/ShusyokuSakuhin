#include "StageLoadCSVComponent.h"
#include "CSV_Data.h"
#include "TerrainManagerComponent.h"
#include <fstream>

using namespace DirectX::SimpleMath;

StageLoadCSVComponent::StageLoadCSVComponent(GameObject& obj) : Component(obj) {
	m_sortNum = STAGE_LOAD_CSV; // ソート番号を設定
}

void StageLoadCSVComponent::Update() {

}

void StageLoadCSVComponent::LoadStageCSV(const std::string& fileName, GameObject& terrainManager) {
	const float firstPointX = -100.0f; // 初期X座標
	const float firstPointY = 100.0f; // 初期X座標

	float posx = firstPointX;
	float posy = firstPointY;
	std::string usefilename = "csv/" + fileName;
	std::ifstream file(usefilename);
	if (!file.is_open()) {
		throw std::runtime_error("ファイルが見つかりませんでした: " + fileName);
	}

	// データを2次元ベクターに保存 
	std::vector<std::vector<CSV_Data>> data;
	std::string line;
	// 1行目をスキップ 
	//std::getline(file, line);
	while (std::getline(file, line)) {	// ファイルから1行ずつ読み込む
		std::stringstream ss(line);	// 取得した行を文字列ストリームに変換
		std::string cell;
		std::vector<CSV_Data> row; // CSVの行を格納するベクター

		//posx = firstPointX;
		while (std::getline(ss, cell, ',')) {	// カンマで区切られたセルを取得

			// BOM（このファイルは UTF-8 ですよという宣言のようなもの）を削除
			if (!cell.empty() && (unsigned char)cell[0] == 0xEF &&
				(unsigned char)cell[1] == 0xBB &&
				(unsigned char)cell[2] == 0xBF) {
				cell.erase(0, 3);
			}

			std::string cellName = ""; // セルの名前を格納する変数
			// 空のセルをスキップ 
			if (cell.empty()) {

				cellName = "NoData"; // 空のセルは "NoData" として扱う
			}
			else {
				cellName = cell; // セルの内容をそのまま使用
			}

			
			// データを行ベクターに追加 
			CSV_Data nowcell = { cellName,Vector2(posx, posy) };	// 位置補正と追加
			row.push_back(nowcell);
			// X座標を更新
			posx += 10.0f;
		}
		// 行データを2次元ベクターに追加 
		data.push_back(row);
		// 次の列へ進む 
		posy -= 10.0f;
		posx = firstPointX;
	}

	file.close(); // 縦方向にデータを読み取り、1次元ベクターに変換 

	// 行で読み込んだデータを列に並び変え
	// 結果を格納する一次元のベクター（列優先で詰めていく）
	std::vector<CSV_Data> result;

	// 最大列数を求める
	size_t maxCols = 0;
	for (auto& row : data) {
		if (row.size() > maxCols) {
			maxCols = row.size();
		}
	}

	// まず列ごとにループ（左→右方向）
	for (size_t col = 0; col < maxCols; ++col) {

		// 次に各列に対して行方向にループ（上→下方向）
		for (size_t row = 0; row < data.size(); ++row) {

			// 行データの中に現在の列が存在しているか確認（行ごとにセル数が異なる可能性があるため）
			if (col < data[row].size()) {

				const auto& cell = data[row][col];

				// まず NoData を除外
				if (cell.kind == "NoData") {
					continue;
				}

				// ここで判定
				// もし「Tで始まるものだけ」なら↓
				if (cell.kind[0] == 'T') {
					result.push_back(cell);
				}
			}
		}
	}

	auto terrinMn = terrainManager.GetComponent<TerrainManagerComponent>();

	// 地形データを移す
	if (terrinMn != nullptr) {
		terrinMn->SetTerrainData(std::move(result)); // TerrainManagerComponentにデータをセット
	}
}