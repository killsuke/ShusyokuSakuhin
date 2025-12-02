#pragma once

#include    <Windows.h>
#include    <cstdint>

// ImGui系
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#pragma comment (lib, "imgui.lib")


//-----------------------------------------------------------------------------
// Applicationクラス
//-----------------------------------------------------------------------------
class Application final
{
public:
    Application(uint32_t width, uint32_t height);
    ~Application();
    void Run();

    // 幅を取得
    static uint32_t GetWidth() {
        return m_Width;
    }

    // 高さを取得
    static uint32_t GetHeight() {
        return m_Height;
    }

    // ウインドウハンドルを返す
    static HWND GetWindow() {
        return m_hWnd;
    }

private:
    static HINSTANCE   m_hInst;        // インスタンスハンドル
    static HWND        m_hWnd;         // ウィンドウハンドル
    static uint32_t    m_Width;        // ウィンドウの横幅
    static uint32_t    m_Height;       // ウィンドウの縦幅
	const double m_fixedDeltaTime = 1.0 / 60.0; // 固定更新の時間
	static double m_accumulatorTime;       // 固定更新用のアキュムレータ

    static bool InitApp();
    static void TermApp();
    static bool InitWnd();
    static void TermWnd();
    static void MainLoop();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};