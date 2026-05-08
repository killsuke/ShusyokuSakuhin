// シングルトンにしても良いが、
// 今はひとまずStaticにして使えるようにする

#pragma once
#include <d3d11.h>  // DirectX11を使うためのヘッダーファイル
#include <DirectXMath.h>
#include <array>

#include <Xinput.h> //XInputを使うためのヘッダーファイル
#pragma comment (lib, "xinput.lib") //XInputを使うために必要

#define XINPUT_A              0x1000
#define XINPUT_B              0x2000
#define XINPUT_X              0x4000
#define XINPUT_Y              0x8000
#define XINPUT_UP             0x0001
#define XINPUT_DOWN           0x0002
#define XINPUT_LEFT           0x0004
#define XINPUT_RIGHT          0x0008
#define XINPUT_START          0x0010
#define XINPUT_BACK           0x0020
#define XINPUT_LEFT_THUMB     0x0040 //左スティック押し込み
#define XINPUT_RIGHT_THUMB    0x0080 //右スティック押し込み
#define XINPUT_LEFT_SHOULDER  0x0100 //L
#define XINPUT_RIGHT_SHOULDER 0x0200 //R

#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x45
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

namespace {
	constexpr unsigned int MAX_KEY_STATE = 256;
}

class Input final{
private:
	//キー入力情報を保存する変数
	static inline BYTE m_KeyState[MAX_KEY_STATE] = {};
	static inline BYTE m_KeyState_old[MAX_KEY_STATE] = {};

	//コントローラー入力情報を保存する変数
	static inline XINPUT_STATE m_ControllerState = {};
	static inline XINPUT_STATE m_ControllerState_old = {};

	static inline int m_VibrationTime = 0; //振動継続時間をカウントする変数

	// 現在のマウスのクリックの状態を保持
	static inline bool m_IsCurrentlyLPressed = false;
	static inline bool m_IsCurrentlyRPressed = false;

	// マウスのクリックのフラグ
	static inline bool m_IsLeftButtonPressed = false;   // 前回の状態（押され続けているか）
	static inline bool m_IsLeftButtonTriggered = false; // 押された瞬間
	static inline bool m_IsLeftButtonReleased = false;  // 離された瞬間

	static inline bool m_IsRightButtonPressed = false;   // 前回の状態（押され続けているか）
	static inline bool m_IsRightButtonTriggered = false; // 押された瞬間
	static inline bool m_IsRightButtonReleased = false;  // 離された瞬間

	static inline POINT m_PrevPt = { 0, 0 }; // 前フレームのマウス座標を保持する変数
	static inline DirectX::XMFLOAT2 m_PrevPos = {}; // 前フレームの正規化されたマウス座標を保持する変数

	// コンストラクタ・デストラクタを削除
	Input() = delete;
	~Input() = delete;

	// コピー・ムーブも削除
	Input(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(const Input&) = delete;
	Input& operator=(Input&&) = delete;


	// マウスのクリックの状態更新
	static void ClickUpdate();

public:

	static void Init();
	static void UnInit();

	static void Update(); //更新

	//キー入力
	static bool GetKeyPress(int key);   //プレス(押している間ずっと)
	static bool GetKeyTrigger(int key); //トリガー(押し始めた時)
	static bool GetKeyRelease(int key); //リリース(押し終わった時)

	//アナログスティック(コントローラー)
	static DirectX::XMFLOAT2 GetLeftAnalogStick(void);
	static DirectX::XMFLOAT2 GetRightAnalogStick(void);

	//トリガー(コントローラー)
	static float GetLeftTrigger(void);
	static float GetRightTrigger(void);

	//ボタン入力(コントローラー)
	static bool GetButtonPress(WORD btn);   //プレス(押している間ずっと)
	static bool GetButtonTrigger(WORD btn); //トリガー(押し始めた時)
	static bool GetButtonRelease(WORD btn); //リリース(押し終わった時)

	//振動(コントローラー)
	//flame：振動を継続する時間(単位：フレーム)
	//powoe：振動の強さ(0～1)
	static void SetVibration(int frame = 1, float powor = 1);

	// マウスの座標を返す
	static DirectX::XMFLOAT2 GetMousePositionNormalize();

	// マウスのクリック
	static bool IsMouseLeftButtonDown();
	static bool IsMouseRightButtonDown();

	// プレス（長押し）
	static bool MouseLeftPress();
	static bool MouseRightPress();

	// リリース（離した時）
	static bool MouseLeftRelease();
	static bool MouseRightRelease();

	// トリガー（押した時）
	static bool MouseLeftTrigger();
	static bool MouseRightTrigger();
};

