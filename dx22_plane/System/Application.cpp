#include <chrono>
#include <thread>
#include "System/Application.h"
#include "System/DirectXRender.h"
#include "Manager/GameObjectManager.h"
#include "Manager/SceneManager.h"
#include "Manager/ComponentTypeManager.h"
#include "Manager/EventBusManager.h"
#include "Manager/TimeManager.h"
#include "Manager/ShaderManager.h"
#include "DebugSystem/DebugSystem.h"

#if _DEBUG

// Application.cppの先頭などにこれを追加すればOK
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

const auto ClassName = TEXT("SLASH_ACTION_2.5D");     //!< ウィンドウクラス名.
const auto WindowName = TEXT("SLASH_ACTION_2.5D");    //!< ウィンドウ名.

HINSTANCE  Application::m_hInst;        // インスタンスハンドル
HWND       Application::m_hWnd;         // ウィンドウハンドル
uint32_t   Application::m_Width;        // ウィンドウの横幅
uint32_t   Application::m_Height;       // ウィンドウの縦幅
double	   Application::m_accumulatorTime = 0.0;

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

	return true;
}

//-----------------------------------------------------------------------------
// ウィンドウの終了処理
//-----------------------------------------------------------------------------
void Application::TermWnd()
{
#if _DEBUG

	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

#endif

	// ウィンドウの登録を解除.
	if (m_hInst != nullptr)
	{
		UnregisterClass(ClassName, m_hInst);
	}

	m_hInst = nullptr;
	m_hWnd = nullptr;
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
	ComponentTypeManager::Init();
	ComponentTypeManager::LoadComponentTypeJsonFile("json/component.json");
	SceneManager::Init();
	EventBusManager::Init();
	ShaderManager::Init();
	DebugSystem::Init();
	
	ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();
	ID3D11Device* device = DirectXRender::GetDevice();

#if _DEBUG

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
#endif

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
#if _DEBUG

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
#endif
			double deltaTime = 0.016;

			m_accumulatorTime += deltaTime;

			while (m_accumulatorTime >= 0.016) {

				TimeManager::Update();

				SceneManager::Update(); // シーンの更新

		
				m_accumulatorTime -= 0.016;
			}

			SceneManager::Draw();   // シーンの描画

			fpsCounter++; // ゲーム処理を実行したら＋１する

			nowTick = GetTickCount64();// 現在時間を取得
			// 前回計測から1000ミリ秒が経過したか？
			if (nowTick >= oldTick + 1000)
			{
#if _DEBUG
				// FPS表示
				char str[64];
				wsprintfA(str, "FPS : %d", fpsCounter);
				SetWindowTextA(m_hWnd, str);

#endif // デバッグモードならFPSをウィンドウタイトルに表示
				// カウンターリセット
				fpsCounter = 0;
				oldTick = nowTick;
			}

			if (m_IsEndGame == true) {
				break;
			}
		}
	}


	// 片付け
	DebugSystem::UnInit();
	ComponentTypeManager::UnInit();
	EventBusManager::UnInit();
	SceneManager::UnInit();
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

	case WM_ENTERSIZEMOVE:	// サイズ変更・移動開始
	{
		
	}
	break;
	case WM_EXITSIZEMOVE:	// サイズ変更・移動終了
	{
		TimeManager::Reset();
	}
	break;
	default:
	{
#if _DEBUG

		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
			return true;
		}

#endif
		// 受け取ったメッセージに対してデフォルトの処理を実行
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	}

	return 0;
}
