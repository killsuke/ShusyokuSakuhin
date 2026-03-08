#include "Input/Input.h"
#include "System/Application.h"

void Input::Init() {

}

void Input::UnInit() {

	//振動を終了させる
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;
	XInputSetState(0, &vibration);
}

void Input::Update()
{
	//1フレーム前の入力を記録しておく
	for (unsigned int i = 0; i < MAX_KEY_STATE; ++i) {
		m_KeyState_old[i] = m_KeyState[i]; 
	}
	m_ControllerState_old = m_ControllerState;

	//キー入力を更新
	const BOOL hr = GetKeyboardState(m_KeyState);

	//コントローラー入力を更新(XInput)
	XInputGetState(0, &m_ControllerState);

	//振動継続時間をカウント
	if (m_VibrationTime > 0) {
		m_VibrationTime--;
		if (m_VibrationTime == 0) { //振動継続時間が経った時に振動を止める
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
			vibration.wLeftMotorSpeed = 0;
			vibration.wRightMotorSpeed = 0;
			XInputSetState(0, &vibration);
		}
	}

	// 現在のマウスのクリックの状態を更新
	ClickUpdate();
}

//キー入力
bool Input::GetKeyPress(int key) //プレス
{
	return m_KeyState[key] & 0x80;
}
bool Input::GetKeyTrigger(int key) //トリガー
{
	return (m_KeyState[key] & 0x80) && !(m_KeyState_old[key] & 0x80);
}
bool Input::GetKeyRelease(int key) //リリース
{
	return !(m_KeyState[key] & 0x80) && (m_KeyState_old[key] & 0x80);
}

//左アナログスティック
DirectX::XMFLOAT2 Input::GetLeftAnalogStick(void)
{
	const SHORT x = m_ControllerState.Gamepad.sThumbLX; // -32768～32767
	const SHORT y = m_ControllerState.Gamepad.sThumbLY; // -32768～32767

	DirectX::XMFLOAT2 res;
	res.x = x / 32767.0f; //-1～1
	res.y = y / 32767.0f; //-1～1
	return res;
}
//右アナログスティック
DirectX::XMFLOAT2 Input::GetRightAnalogStick(void)
{
	const SHORT x = m_ControllerState.Gamepad.sThumbRX; // -32768～32767
	const SHORT y = m_ControllerState.Gamepad.sThumbRY; // -32768～32767

	DirectX::XMFLOAT2 res;
	res.x = x / 32767.0f; //-1～1
	res.y = y / 32767.0f; //-1～1
	return res;
}

//左トリガー
float Input::GetLeftTrigger(void)
{
	const BYTE t = m_ControllerState.Gamepad.bLeftTrigger; // 0～255
	return t / 255.0f;
}
//右トリガー
float Input::GetRightTrigger(void)
{
	const BYTE t = m_ControllerState.Gamepad.bRightTrigger; // 0～255
	return t / 255.0f;
}

//ボタン入力
bool Input::GetButtonPress(WORD btn) //プレス
{
	return (m_ControllerState.Gamepad.wButtons & btn) != 0;
}
bool Input::GetButtonTrigger(WORD btn) //トリガー
{
	return (m_ControllerState.Gamepad.wButtons & btn) != 0 && (m_ControllerState_old.Gamepad.wButtons & btn) == 0;
}
bool Input::GetButtonRelease(WORD btn) //リリース
{
	return (m_ControllerState.Gamepad.wButtons & btn) == 0 && (m_ControllerState_old.Gamepad.wButtons & btn) != 0;
}

//振動
void Input::SetVibration(int frame, float powor)
{
	// XINPUT_VIBRATION構造体のインスタンスを作成
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	// モーターの強度を設定（0～65535）
	vibration.wLeftMotorSpeed = (WORD)(powor * 65535.0f);
	vibration.wRightMotorSpeed = (WORD)(powor * 65535.0f);
	XInputSetState(0, &vibration);

	//振動継続時間を代入
	m_VibrationTime = frame;
}

// マウスの座標情報を返す
DirectX::XMFLOAT2 Input::GetMousePositionNormalize() {

	POINT pt;	// マウスの座標取得（宣言）

	if (GetCursorPos(&pt)) {	// マウスの座標取得
		const HWND hWnd = Application::GetWindow();	// 作成したウィンドウのハンドル取得

		// 縦横サイズ取得
		const uint32_t SCREEN_WIDTH = Application::GetWidth();
		const uint32_t SCREEN_HEIGHT = Application::GetHeight();

		// 前フレームと位置が違えば更新
		if (pt.x != prevPt.x || pt.y != prevPt.y) {
			// ウィンドウ座標に変換
			if (ScreenToClient(hWnd, &pt)) {
				RECT clientRect;	// ウィンドウのクライアント領域の矩形を取得
				GetClientRect(hWnd, &clientRect);	// ウィンドウのクライアント領域の座標を取得
				const int width = clientRect.right - clientRect.left;		// ウィンドウの横幅を計算
				const int height = clientRect.bottom - clientRect.top;	// ウィンドウの縦幅を計算

				// 補正処理（座標の正規化）
				vec2.x = static_cast<float>(pt.x) / static_cast<float>(width) * SCREEN_WIDTH - (SCREEN_WIDTH * 0.5f);
				vec2.y = -(static_cast<float>(pt.y) / static_cast<float>(height) * SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.5f));

				// 前フレームのマウス座標を更新
				prevPt = pt;
			}
		}
	}
	return vec2;	// マウスの座標を返す
}

// 左クリック
bool Input::IsMouseLeftButtonDown() {
	return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}

// 右クリック
bool Input::IsMouseRightButtonDown() {
	return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
}

// マウスの入力状態の更新
void Input::ClickUpdate() {
	// 入力の更新
	m_IsCurrentlyLPressed = IsMouseLeftButtonDown();
	m_IsCurrentlyRPressed = IsMouseRightButtonDown();

	// トリガー
	m_IsLeftButtonTriggered = !m_IsLeftButtonPressed && m_IsCurrentlyLPressed;
	m_IsRightButtonTriggered = !m_IsRightButtonPressed && m_IsCurrentlyRPressed;

	// リリース
	m_IsLeftButtonReleased = m_IsLeftButtonPressed && !m_IsCurrentlyLPressed;
	m_IsRightButtonReleased = m_IsRightButtonPressed && !m_IsCurrentlyRPressed;

	// プレス
	m_IsLeftButtonPressed = m_IsCurrentlyLPressed;
	m_IsRightButtonPressed = m_IsCurrentlyRPressed;
}

// プレス
bool Input::MouseLeftPress() {
	if (m_IsLeftButtonPressed == true) {
		return m_IsLeftButtonPressed;
	}
	return false;
}

bool Input::MouseRightPress() {
	if (m_IsRightButtonPressed == true) {
		return m_IsRightButtonPressed;
	}
	return false;
}

// リリース
bool Input::MouseLeftRelease() {
	if (m_IsLeftButtonReleased == true) {
		return m_IsLeftButtonReleased;
	}
	return false;
}

bool Input::MouseRightRelease() {
	if (m_IsRightButtonReleased == true) {
		return m_IsRightButtonReleased;
	}
	return false;
}

// トリガー
bool Input::MouseLeftTrigger() {
	if (m_IsLeftButtonTriggered == true) {
		return m_IsLeftButtonTriggered;
	}
	return false;
}

bool Input::MouseRightTrigger() {
	if (m_IsRightButtonTriggered == true) {
		return m_IsRightButtonTriggered;
	}
	return false;
}