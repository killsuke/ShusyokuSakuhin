#include "input.h"
#include "System/Application.h"

BYTE Input::keyState[256] = {};
BYTE Input::keyState_old[256] = {};
XINPUT_STATE Input::controllerState = {};
XINPUT_STATE Input::controllerState_old = {};
int Input::VibrationTime = 0;
bool Input::isCurrentlyLPressed = false;
bool Input::isCurrentlyRPressed = false;

// 左クリックの方
bool Input::isLeftButtonPressed = false;
bool Input::isLeftButtonTriggered = false;
bool Input::isLeftButtonReleased = false;

// 右クリックの方
bool Input::isRightButtonPressed = false;
bool Input::isRightButtonTriggered = false;
bool Input::isRightButtonReleased = false;

//コンストラクタ
Input::Input()
{
}

//デストラクタ
Input::~Input()
{
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
	for (int i = 0; i < 256; ++i) {
		keyState_old[i] = keyState[i]; 
	}
	controllerState_old = controllerState;

	//キー入力を更新
	const BOOL hr = GetKeyboardState(keyState);

	//コントローラー入力を更新(XInput)
	XInputGetState(0, &controllerState);

	//振動継続時間をカウント
	if (VibrationTime > 0) {
		VibrationTime--;
		if (VibrationTime == 0) { //振動継続時間が経った時に振動を止める
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
	return keyState[key] & 0x80;
}
bool Input::GetKeyTrigger(int key) //トリガー
{
	return (keyState[key] & 0x80) && !(keyState_old[key] & 0x80);
}
bool Input::GetKeyRelease(int key) //リリース
{
	return !(keyState[key] & 0x80) && (keyState_old[key] & 0x80);
}

//左アナログスティック
DirectX::XMFLOAT2 Input::GetLeftAnalogStick(void)
{
	const SHORT x = controllerState.Gamepad.sThumbLX; // -32768～32767
	const SHORT y = controllerState.Gamepad.sThumbLY; // -32768～32767

	DirectX::XMFLOAT2 res;
	res.x = x / 32767.0f; //-1～1
	res.y = y / 32767.0f; //-1～1
	return res;
}
//右アナログスティック
DirectX::XMFLOAT2 Input::GetRightAnalogStick(void)
{
	const SHORT x = controllerState.Gamepad.sThumbRX; // -32768～32767
	const SHORT y = controllerState.Gamepad.sThumbRY; // -32768～32767

	DirectX::XMFLOAT2 res;
	res.x = x / 32767.0f; //-1～1
	res.y = y / 32767.0f; //-1～1
	return res;
}

//左トリガー
float Input::GetLeftTrigger(void)
{
	const BYTE t = controllerState.Gamepad.bLeftTrigger; // 0～255
	return t / 255.0f;
}
//右トリガー
float Input::GetRightTrigger(void)
{
	const BYTE t = controllerState.Gamepad.bRightTrigger; // 0～255
	return t / 255.0f;
}

//ボタン入力
bool Input::GetButtonPress(WORD btn) //プレス
{
	return (controllerState.Gamepad.wButtons & btn) != 0;
}
bool Input::GetButtonTrigger(WORD btn) //トリガー
{
	return (controllerState.Gamepad.wButtons & btn) != 0 && (controllerState_old.Gamepad.wButtons & btn) == 0;
}
bool Input::GetButtonRelease(WORD btn) //リリース
{
	return (controllerState.Gamepad.wButtons & btn) == 0 && (controllerState_old.Gamepad.wButtons & btn) != 0;
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
	VibrationTime = frame;
}

// マウスの座標情報を返す
DirectX::XMFLOAT2 Input::GetMousePositionNormalize() {
	static POINT prevPt = { -1,-1 };	// 前フレームのマウスの座標
	static DirectX::XMFLOAT2 vec2 = {};		// 前フレームの正規化された座標

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
				vec2.x = static_cast<float>(pt.x) / static_cast<float>(width) * SCREEN_WIDTH - (SCREEN_WIDTH / 2.0f);
				vec2.y = (static_cast<float>(pt.y) / static_cast<float>(height) * SCREEN_HEIGHT - (SCREEN_HEIGHT / 2.0f)) * -1.0f;

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
	isCurrentlyLPressed = IsMouseLeftButtonDown();
	isCurrentlyRPressed = IsMouseRightButtonDown();

	// トリガー
	isLeftButtonTriggered = !isLeftButtonPressed && isCurrentlyLPressed;
	isRightButtonTriggered = !isRightButtonPressed && isCurrentlyRPressed;

	// リリース
	isLeftButtonReleased = isLeftButtonPressed && !isCurrentlyLPressed;
	isRightButtonReleased = isRightButtonPressed && !isCurrentlyRPressed;

	// プレス
	isLeftButtonPressed = isCurrentlyLPressed;
	isRightButtonPressed = isCurrentlyRPressed;
}

// プレス
bool Input::MouseLeftPress() {
	if (isLeftButtonPressed == true) {
		return isLeftButtonPressed;
	}
	return false;
}

bool Input::MouseRightPress() {
	if (isRightButtonPressed == true) {
		return isRightButtonPressed;
	}
	return false;
}

// リリース
bool Input::MouseLeftRelease() {
	if (isLeftButtonReleased == true) {
		return isLeftButtonReleased;
	}
	return false;
}

bool Input::MouseRightRelease() {
	if (isRightButtonReleased == true) {
		return isRightButtonReleased;
	}
	return false;
}

// トリガー
bool Input::MouseLeftTrigger() {
	if (isLeftButtonTriggered == true) {
		return isLeftButtonTriggered;
	}
	return false;
}

bool Input::MouseRightTrigger() {
	if (isRightButtonTriggered == true) {
		return isRightButtonTriggered;
	}
	return false;
}