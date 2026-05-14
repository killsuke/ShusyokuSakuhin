#include <chrono>
#include <thread>
#include "System/Application.h"
#include "System/DirectXRender.h"
#include "Manager/GameObjectManager.h"
#include "Manager/SceneManager.h"
#include "Manager/TextureManager.h"
#include "Manager/SoundManager.h"
#include "Manager/ComponentTypeManager.h"
#include "Manager/EventBusManager.h"
#include "Manager/TimeManager.h"
#include "Manager/ShaderManager.h"
#include "DebugSystem/DebugSystem.h"
#include "Input/Input.h"

const wchar_t* ClassName = TEXT("SLASH_ACTION_2.5D");     //!< ウィンドウクラス名.
const wchar_t* WindowName = TEXT("SLASH_ACTION_2.5D");    //!< ウィンドウ名.

namespace {
	constexpr int MAX_LOOP = 3;	// 固定更新の最大ループ回数（無限ループ防止用）
}

void Application::Init(const uint32_t& width, const uint32_t& height) {
	m_Height = height;
	m_Width = width;

	timeBeginPeriod(1);
}

void Application::UnInit() {
	timeEndPeriod(1);
}

//-----------------------------------------------------------------------------
// 実行
//-----------------------------------------------------------------------------
void Application::Run()
{
	if (InitApp())
	{
		MainLoop();
	}

	TermApp();
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
bool Application::InitApp()
{
	// ウィンドウの初期化.
	if (!InitWnd())
	{
		return false;
	}

	// 正常終了.
	return true;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Application::TermApp()
{
	// ウィンドウの終了処理.
	TermWnd();
}

//-----------------------------------------------------------------------------
// ウィンドウの初期化処理
//-----------------------------------------------------------------------------
bool Application::InitWnd()
{
	// インスタンスハンドルを取得.
	const HMODULE hInst = GetModuleHandle(nullptr);
	if (hInst == nullptr)
	{
		return false;
	}

	// ウィンドウの設定.
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInst, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = ClassName;
	wc.hIconSm = LoadIcon(hInst, IDI_APPLICATION);

	// ウィンドウの登録.
	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	// インスタンスハンドル設定.
	m_HInst = hInst;

	// ウィンドウのサイズを設定.
	RECT rc = {};
	rc.right = static_cast<LONG>(m_Width);
	rc.bottom = static_cast<LONG>(m_Height);

	// ウィンドウサイズを調整.
	const long style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rc, style, FALSE);

	// ウィンドウを生成.
	m_HWnd = CreateWindowEx(
		0,
		//        WS_EX_TOPMOST,
		ClassName,
		WindowName,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		m_HInst,
		nullptr);

	if (m_HWnd == nullptr)
	{
		return false;
	}

	// ウィンドウを表示.
	ShowWindow(m_HWnd, SW_SHOWNORMAL);

	// ウィンドウを更新.
	UpdateWindow(m_HWnd);

	// ウィンドウにフォーカスを設定.
	SetFocus(m_HWnd);

	return true;
}

//-----------------------------------------------------------------------------
// ウィンドウの終了処理
//-----------------------------------------------------------------------------
void Application::TermWnd()
{
	// ウィンドウの登録を解除.
	if (m_HInst != nullptr)
	{
		UnregisterClass(ClassName, m_HInst);
	}

	m_HInst = nullptr;
	m_HWnd = nullptr;
}

//-----------------------------------------------------------------------------
// メインループ
//-----------------------------------------------------------------------------
void Application::MainLoop()
{
	MSG msg = {};

	// 描画初期化
	HRESULT hr = DirectXRender::Init();
	if (FAILED(hr)) {
		DirectXRender::UnInit();
		return;
	}

	TimeManager::Init();
	ShaderManager::Init();
	ComponentTypeManager::Init();
	ComponentTypeManager::LoadComponentTypeJsonFile("Json/component.json");
	SoundManager::Init();	// サウンドの初期化
	TextureManager::Init(); // テクスチャマネージャーの初期化処理
	GameObjectManager::Init();	// ゲームオブジェクトのマネージャーを初期化
	SceneManager::Init();
	EventBusManager::Init();
	DebugSystem::Init();
	Input::Init();

	ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();
	ID3D11Device* device = DirectXRender::GetDevice();

	// FPS計測用変数
	int fpsCounter = 0;

	// ゲームループ
	while (1)
	{
		// 新たにメッセージがあれば
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ウィンドウプロシージャにメッセージを送る
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// 「WM_QUIT」メッセージを受け取ったらループを抜ける
			if (msg.message == WM_QUIT) {
				break;
			}
		}
		else
		{
			Input::Update();
			TimeManager::Update();

			int loopCount = 0;

			while (TimeManager::ShouldFixedUpdate() && loopCount < MAX_LOOP) {

				SceneManager::Update(); // シーンの更新

				loopCount++;
			}

			// ズレによって更新されなかった場合更新する
			if (loopCount == 0) {
				SceneManager::Update(); // シーンの更新
			}

			SceneManager::Draw();   // シーンの描画

			fpsCounter++; // ゲーム処理を実行したら＋１する

			m_FpsTimer += TimeManager::GetDeltaTime(); // 前回からの時間を加算

			// 前回計測から1000ミリ秒が経過したか？
			if (m_FpsTimer >= 1.0f)
			{
#if _DEBUG
				// FPS表示
				char str[64];
				wsprintfA(str, "FPS : %d", fpsCounter);
				SetWindowTextA(m_HWnd, str);

#endif // デバッグモードならFPSをウィンドウタイトルに表示
				// カウンターリセット
				fpsCounter = 0;
				m_FpsTimer = 0.0f;
			}

			if (m_IsEndGame == true) {
				break;
			}
		}
	}


	// 片付け
	Input::UnInit();
	DebugSystem::UnInit();
	ComponentTypeManager::UnInit();
	EventBusManager::UnInit();
	SceneManager::UnInit();
	GameObjectManager::UnInit();	// ゲームオブジェクトのマネージャーを終了
	TextureManager::UnInit(); // テクスチャマネージャーの終了処理
	SoundManager::UnInit();	// サウンドの終了処理
	ShaderManager::UnInit();
	DirectXRender::UnInit();
}

//-----------------------------------------------------------------------------
// ウィンドウプロシージャ
//-----------------------------------------------------------------------------
LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_DESTROY:// ウィンドウ破棄のメッセージ
	{
		PostQuitMessage(0);// 「WM_QUIT」メッセージを送る　→　アプリ終了
	}
	break;

	case WM_CLOSE:  // 「x」ボタンが押されたら
	{
		int res = MessageBoxW(NULL, L"終了しますか？", L"確認", MB_OKCANCEL);
		if (res == IDOK) {
			DestroyWindow(hWnd);  // 「WM_DESTROY」メッセージを送る
		}
	}
	break;

	case WM_SIZE: // ウィンドウサイズが変更されたメッセージ
	{
		// ウィンドウの幅と高さを取得
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);

		if (width == 0 || height == 0) {
			break; // 最小化された場合はリサイズ処理をスキップ
		}

		DirectXRender::OnResize(width, height);
	}
	break;

	case WM_KEYDOWN: //キー入力があったメッセージ
	{
		if (LOWORD(wParam) == VK_ESCAPE) { //入力されたキーがESCAPEなら
			PostMessage(hWnd, WM_CLOSE, wParam, lParam);//「WM_CLOSE」を送る
		}
	}
	break;

	case WM_ENTERSIZEMOVE:	// サイズ変更・移動開始
	{
		TimeManager::SetTimeManagerActive(false);
	}
	break;
	case WM_EXITSIZEMOVE:	// サイズ変更・移動終了
	{
		TimeManager::SetTimeManagerActive(true);
	}
	break;
	default:
	{
		// 受け取ったメッセージに対してデフォルトの処理を実行
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	}

	return 0;
}
