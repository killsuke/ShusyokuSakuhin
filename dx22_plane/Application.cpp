#include <chrono>
#include <thread>
#include "Application.h"
#include "DirectXRender.h"
#include "Game.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "ComponentTypeManager.h"
#include "TimeManager.h"

// Application.cppの先頭などにこれを追加すればOK
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

const auto ClassName = TEXT("2024 framework ひな型");     //!< ウィンドウクラス名.
const auto WindowName = TEXT("2024 framework ひな型(フィールド描画)");    //!< ウィンドウ名.

HINSTANCE  Application::m_hInst;        // インスタンスハンドル
HWND       Application::m_hWnd;         // ウィンドウハンドル
HWND       Application::m_hWnd2;         // ウィンドウハンドル
uint32_t   Application::m_Width;        // ウィンドウの横幅
uint32_t   Application::m_Height;       // ウィンドウの縦幅

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Application::Application(uint32_t width, uint32_t height)
{
	m_Height = height;
	m_Width = width;

	timeBeginPeriod(1);
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Application::~Application()
{
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
	auto hInst = GetModuleHandle(nullptr);
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
	m_hInst = hInst;

	// ウィンドウのサイズを設定.
	RECT rc = {};
	rc.right = static_cast<LONG>(m_Width);
	rc.bottom = static_cast<LONG>(m_Height);

	// ウィンドウサイズを調整.
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rc, style, FALSE);

	// ウィンドウを生成.
	m_hWnd = CreateWindowEx(
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
		m_hInst,
		nullptr);

	if (m_hWnd == nullptr)
	{
		return false;
	}

	// ウィンドウを表示.
	ShowWindow(m_hWnd, SW_SHOWNORMAL);

	// ウィンドウを更新.
	UpdateWindow(m_hWnd);

	// ウィンドウにフォーカスを設定.
	SetFocus(m_hWnd);



	// ウィンドウの設定.
	//WNDCLASSEX wc2 = {};
	//wc2.cbSize = sizeof(WNDCLASSEX);
	//wc2.style = CS_HREDRAW | CS_VREDRAW;
	//wc2.lpfnWndProc = WndProc;
	//wc2.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	//wc2.hCursor = LoadCursor(hInst, IDC_ARROW);
	//wc2.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	//wc2.lpszMenuName = nullptr;
	//wc2.lpszClassName = ClassName;
	//wc2.hIconSm = LoadIcon(hInst, IDI_APPLICATION);

	//// ウィンドウの登録.
	//if (!RegisterClassEx(&wc2))
	//{
	//	return false;
	//}

	//// インスタンスハンドル設定.
	//m_hInst = hInst;

	//// ウィンドウのサイズを設定.
	//RECT rc2 = {};
	//rc2.right = static_cast<LONG>(m_Width);
	//rc2.bottom = static_cast<LONG>(m_Height);

	//// ウィンドウサイズを調整.
	//auto style2 = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	//AdjustWindowRect(&rc2, style2, FALSE);

	//// ウィンドウを生成.
	//m_hWnd2 = CreateWindowEx(
	//	0,
	//	//        WS_EX_TOPMOST,
	//	ClassName,
	//	WindowName,
	//	style2,
	//	CW_USEDEFAULT,
	//	CW_USEDEFAULT,
	//	rc2.right - rc2.left,
	//	rc2.bottom - rc2.top,
	//	nullptr,
	//	nullptr,
	//	m_hInst,
	//	nullptr);

	//if (m_hWnd2 == nullptr)
	//{
	//	return false;
	//}

	//// ウィンドウを表示.
	//ShowWindow(m_hWnd2, SW_SHOWNORMAL);

	//// ウィンドウを更新.
	//UpdateWindow(m_hWnd2);

	//// ウィンドウにフォーカスを設定.
	//SetFocus(m_hWnd2);

	// 正常終了.
	return true;
}

//-----------------------------------------------------------------------------
// ウィンドウの終了処理
//-----------------------------------------------------------------------------
void Application::TermWnd()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// ウィンドウの登録を解除.
	if (m_hInst != nullptr)
	{
		UnregisterClass(ClassName, m_hInst);
	}

	m_hInst = nullptr;
	m_hWnd = nullptr;
	//	m_hWnd2 = nullptr;
}

//-----------------------------------------------------------------------------
// メインループ
//-----------------------------------------------------------------------------
void Application::MainLoop()
{
	MSG msg = {};

	// 描画初期化
	DirectXRender::Init();
	TimeManager::Init();
	ComponentTypeManager::Init();
	ComponentTypeManager::LoadComponentTypeJsonFile("json/component.json");
	SceneManager::Init();

	//ComponentTypeManager::MakeSampleJson();

	auto deviceContext = DirectXRender::GetDeviceContext();
	auto device = DirectXRender::GetDevice();

	// ImGuiコンテキストの作成
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // キーボード操作を有効化
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// ImGuiのスタイル設定（オプション）
	ImGui::StyleColorsDark();

	// デバイスやウィンドウへのバインド
	ImGui_ImplWin32_Init(m_hWnd);  // hwnd: ウィンドウハンドル
	ImGui_ImplDX11_Init(device, deviceContext);


	// FPS計測用変数
	int fpsCounter = 0;
	long long oldTick = GetTickCount64(); // 前回計測時の時間
	long long nowTick = oldTick; // 今回計測時の時間

	// FPS固定用変数
	LARGE_INTEGER liWork; // workがつく変数は作業用変数
	long long frequency;// どれくらい細かく時間をカウントできるか
	QueryPerformanceFrequency(&liWork);
	frequency = liWork.QuadPart;
	// 時間（単位：カウント）取得
	QueryPerformanceCounter(&liWork);
	long long oldCount = liWork.QuadPart;// 前回計測時の時間
	long long nowCount = oldCount;// 今回計測時の時間

	// ゲームループ
	while (1)
	{
		TimeManager::Update();

		float num = 0.0f;
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
			QueryPerformanceCounter(&liWork);// 現在時間を取得
			nowCount = liWork.QuadPart;

			// 前回フレームからの経過時間（秒）を計算
			double elapsedSec = (double)(nowCount - oldCount) / frequency;

			// 60FPS = 1フレーム約16.666ms = 約0.0166秒
			// 経過時間がそれ未満なら、CPUをスリープさせる
			if (elapsedSec < 1.0 / 60.0) {

				// 足りない時間をミリ秒に換算してSleeo
				DWORD sleepTime = (DWORD)(((1.0 / 60.0) - elapsedSec) * 1000.0);

				if (sleepTime > 0) {
					Sleep(sleepTime);	// 最小単位は1ms。Sleep(0)は意味がないので除外
				}
				continue;	// 次のループへ

			}

			// ImGuiのフレーム開始
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			// ImGuiのUI要素をここに追加
			ImGui::Begin("Debug Window");
			ImGui::Text("マウスの座標");
			DirectX::SimpleMath::Vector2 vec2 = Input::GetMousePositionNormalize();

			// マウス座標デバッグ
			ImGui::Text("X_Mouse : %.3f", vec2.x); // 現在の値を表示
			ImGui::Text("Y_Mouse : %.3f", vec2.y); // 現在の値を表示

			if (ImGui::Button("Click Me!")) {
				ImGui::Text("FPS: %d", fpsCounter);
			}

			bool isChecked = false;
			ImGui::Checkbox("Enable Feature", &isChecked);

			ImGui::SliderFloat("Float Slider", &num, 0.0f, 1.0f);

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddCircle(ImVec2(150, 150), 50, ImColor(255, 0, 0));

			ImGui::End();

			SceneManager::Update(); // シーンの更新

			fpsCounter++; // ゲーム処理を実行したら＋１する
			oldCount = nowCount;


			nowTick = GetTickCount64();// 現在時間を取得
			// 前回計測から1000ミリ秒が経過したか？
			if (nowTick >= oldTick + 1000)
			{
#if _DEBUG
				// FPS表示
				char str[32];
				wsprintfA(str, "FPS=%d", fpsCounter);
				SetWindowTextA(m_hWnd, str);
				//std::cout << "ここFPS" << std::endl; // コンソールに出力

#endif // デバッグモードならFPSをウィンドウタイトルに表示
				// カウンターリセット
				fpsCounter = 0;
				oldTick = nowTick;
			}
		}
	}

	ComponentTypeManager::UnInit();

	// 終了処理
	SceneManager::UnInit();

	// 描画初期化
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
		int res = MessageBoxA(NULL, "終了しますか？", "確認", MB_OKCANCEL);
		if (res == IDOK) {
			DestroyWindow(hWnd);  // 「WM_DESTROY」メッセージを送る
		}
	}
	break;

	case WM_KEYDOWN: //キー入力があったメッセージ
	{
		if (LOWORD(wParam) == VK_ESCAPE) { //入力されたキーがESCAPEなら
			PostMessage(hWnd, WM_CLOSE, wParam, lParam);//「WM_CLOSE」を送る
		}
	}
	break;

	default:
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
			return true;
		}
		// 受け取ったメッセージに対してデフォルトの処理を実行
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	}

	return 0;
}
