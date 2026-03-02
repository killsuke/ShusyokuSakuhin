#include "StringHelper.h"

std::wstring StringToWString(const std::string& str) {

    // 必要な UTF-16 の文字数を計算
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);

    // UTF-16 のバッファを確保する
    std::wstring wstr(size_needed, 0);

    // UTF-8 から UTF-16 変換を行う
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);

    // 終端の '\0' を削除
    wstr.pop_back();
    return wstr;
}