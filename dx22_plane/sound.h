#pragma once

#include <xaudio2.h>

// サウンドファイル
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// サンプルBGM
	SOUND_LABEL_BGM001,			// サンプルBGM
	SOUND_LABEL_BGM002,			// サンプルBGM
	//SOUND_LABEL_BGM003,			// サンプルBGM
	//SOUND_LABEL_BGM004,			// サンプルBGM
	SOUND_LABEL_SE000,			// サンプルSE
	SOUND_LABEL_SE001,			// サンプルSE
	SOUND_LABEL_SE002,			// サンプルSE
	SOUND_LABEL_SE003,			// サンプルSE
	SOUND_LABEL_SE004,			// サンプルSE
	SOUND_LABEL_SE005,			// サンプルSE
	SOUND_LABEL_SE006,			// サンプルSE
	SOUND_LABEL_SE007,			// サンプルSE
	SOUND_LABEL_SE008,			// サンプルSE
	//SOUND_LABEL_SE007,			// サンプルSE
	//SOUND_LABEL_SE008,			// サンプルSE



	SOUND_LABEL_MAX,
} SOUND_LABEL;

class Sound {
private:
	// パラメータ構造体
	typedef struct
	{
		LPCSTR filename;	// 音声ファイルまでのパスを設定
		bool bLoop;			// trueでループ。通常BGMはture、SEはfalse。
	} PARAM;

	PARAM m_param[SOUND_LABEL_MAX] =
	{
		{"sound/BGM/bgm1.wav", true},	// サンプルBGM（ループさせるのでtrue設定）
		{"sound/BGM/Title.wav", true},	// サンプルBGM
		{"sound/BGM/Result.wav", true},	// サンプルBGM
		//{"sound/BGM/GAMEOVER.wav", true},	// サンプルBGM
		//{"sound/BGM/ENDING.wav", true},	// サンプルBGM
		{"sound/SE/slash.wav", false},  		// サンプルSE（ループしないのでfalse設定）
		{"sound/SE/slashHit.wav", false},		// サンプルSE
		{"sound/SE/count.wav", false},		// サンプルSE
		{"sound/SE/start.wav", false},		// サンプルSE
		{"sound/SE/dram.wav", false},		// サンプルSE
		{"sound/SE/finish.wav", false},		// サンプルSE
		{"sound/SE/rank.wav", false},		// サンプルSE
		{"sound/SE/jump.wav", false},		// サンプルSE
		{"sound/SE/dash.wav", false},		// サンプルSE



	};

	IXAudio2* m_pXAudio2 = NULL;
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;
	IXAudio2SourceVoice* m_pSourceVoice[SOUND_LABEL_MAX]{};
	WAVEFORMATEXTENSIBLE m_wfx[SOUND_LABEL_MAX]{}; // WAVフォーマット
	XAUDIO2_BUFFER m_buffer[SOUND_LABEL_MAX]{};
	BYTE* m_DataBuffer[SOUND_LABEL_MAX]{};

	HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);
	HRESULT ReadChunkData(HANDLE, void*, DWORD, DWORD);

public:

	Sound() = default;	// デフォルトコンストラクタ
	~Sound() = default;	// デフォルトデストラクタ

	// ゲームループ開始前に呼び出すサウンドの初期化処理
	HRESULT Init(void);

	// ゲームループ終了後に呼び出すサウンドの解放処理
	void Uninit(void);

	// 引数で指定したサウンドを再生する
	void Play(SOUND_LABEL label);

	// 引数で指定したサウンドを停止する
	void Stop(SOUND_LABEL label);

	// 引数で指定したサウンドの再生を再開する
	void Resume(SOUND_LABEL label);

	// 引数で指定したサウンドの音量を調整する
	void SetVolume(SOUND_LABEL label, float volume);
};