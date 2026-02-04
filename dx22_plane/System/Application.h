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

private:
    static inline HINSTANCE   m_HInst;        // インスタンスハンドル
    static inline HWND        m_HWnd;         // ウィンドウハンドル
    static inline uint32_t    m_Width = 0;        // ウィンドウの横幅
    static inline uint32_t    m_Height = 0;       // ウィンドウの縦幅
	const double m_FixedDeltaTime = 1.0 / 60.0; // 固定更新の時間
	static inline double m_AccumulatorTime = 0.0f;       // 固定更新用のアキュムレータ
    static inline bool m_IsEndGame = false; // ゲームを終了させるフラグ

    Application() = default;
    ~Application() = default;

    // コピー・ムーブも削除
    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    static bool InitApp();
    static void TermApp();
    static bool InitWnd();
    static void TermWnd();
    static void MainLoop();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

public:

    static void Init(const uint32_t& width, const uint32_t& height);
    static void UnInit();
    static void Run();

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
        return m_HWnd;
    }

    static void GameEnd() {
        m_IsEndGame = true;
    }
};