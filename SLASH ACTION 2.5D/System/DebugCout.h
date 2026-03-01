#pragma once		// ここを0にするとデバッグ用のウィンドウが出ない
//#define _DEBUG	// ここを1にするとデバッグ用のウィンドウが出る
// ↑この宣言もcppに、↓使いたい関数をcppに書いて、宣言をヘッダーに…無理かな？
// templateのせいでこうなってるから、もしかしたらここの仕様変更が入るかも？それか、明示的にインスタンス化する？
#include <iostream>
#include <string>
#include <consoleapi.h>
#include <psapi.h>
#include <windows.h>
#include <string>
#include <locale>
#include <sstream>
#include <iomanip>

namespace Debug {
	namespace {
		static FILE* fp = nullptr;
		static PROCESS_MEMORY_COUNTERS_EX pmc;	// メモリ使用量
	}

	static void DebugFirst() {	// デバッグ用ウィンドウを使うための準備
#if _DEBUG
		AllocConsole();
		freopen_s(&fp, "CON", "w", stdout);
#endif
	}

	// オブジェクトの名前と数値
	template<typename T>
	static void ObjectNum(const std::string& name, const T& value) {
#if _DEBUG
		std::cout << name << "：" << value << std::endl;
#endif
	}

	// オブジェクトの名前と数値を２つ
	template<typename T>
	static void ObjectNum2(const std::string& name1, const T& value1, const std::string& name2, const T& value2) {
#if _DEBUG
		std::cout << name1 << "：" << value1 << "  " << name2 << "：" << value2 << std::endl;
#endif
	}

	// たぶんこれは使わない
	static void DebugClear() {
#if _DEBUG
		std::system("cls");
#endif
	}

	// 数値を見やすくする
	static std::string FormatNumber(size_t number) {
		std::stringstream ss;		// 文字ストリーム作成、数値を文字列に変換
		ss.imbue(std::locale(""));	// 三桁ごとにカンマ打ち
		ss << std::fixed << number; // 数値をストリームに挿入
		return ss.str();			// ストリームの内容を文字列として返す
	}

	// メモリ使用量を見てくれる関数
	static void PrintMemory(std::string memory) {
#if _DEBUG
		if (GetProcessMemoryInfo(GetCurrentProcess(),		// プロセスに対する操作を行うためにハンドル取得
			(PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) { // メモリ使用量を計ってその値を格納
			if (memory == "Working") {
				// 全てのメモリ使用量を見るならこっち
				std::cout << "メモリ使用量：" << FormatNumber(pmc.WorkingSetSize / 1024)  << " KB" << std::endl;
			}
			else {
				// 特定のメモリ使用量を見るならこっち
				std::cout << "メモリ使用量：" << FormatNumber(pmc.PrivateUsage / 1024) << " KB" << std::endl;
			}
		}
		else {
			// エラー処理
			std::cerr << "メモリサイズの取得失敗" << std::endl;
		}
#endif
	}

	static void DebugFin() {
#if _DEBUG
		if (fp != nullptr) {
			fclose(fp);
			fp = nullptr;
		}
		FreeConsole();		// コンソールウィンドウを解放
#endif // _DEBUG
	}
}