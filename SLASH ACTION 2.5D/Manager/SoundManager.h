#pragma once
#include <xaudio2.h>
#include <unordered_map>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

struct SoundResource
{
	WAVEFORMATEXTENSIBLE wfx{};	// WAVフォーマット
	XAUDIO2_BUFFER buffer{};
	BYTE* pDataBuffer = nullptr;
	bool bLoop = false;
};

class SoundManager final
{
private:

	// サウンドリソース管理用マップ
	static inline std::unordered_map<std::string, SoundResource> m_SoundResources;

	static inline IXAudio2* m_XAudio2 = nullptr;						// XAudio2インターフェース
	static inline IXAudio2MasteringVoice* m_MasteringVoice = nullptr;	// マスタリングボイス

	// コンストラクタ・デストラクタを削除
	SoundManager() = delete;
	~SoundManager() = delete;

	// コピー・ムーブも削除
	SoundManager(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;

	static HRESULT FindChunk(const HANDLE& hFile,const DWORD& fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	static HRESULT ReadChunkData(const HANDLE& hFile, void* buffer, const DWORD& buffersize, const DWORD& bufferoffset);
	static void LoadFolder(const std::string& path, const bool loop);
	static HRESULT LoadWave(const std::string& key, const wchar_t* filename, const bool loop);

public:

	// ゲームループ開始前に呼び出すサウンドの初期化処理
	static HRESULT Init();

	// ゲームループ終了後に呼び出すサウンドの解放処理
	static void UnInit();

	static IXAudio2* GetXAudio2() { return m_XAudio2; };
	static IXAudio2MasteringVoice* GetMasteringVoice() { return m_MasteringVoice; };
	static SoundResource* GetSoundResource(const std::string& key) {

		std::unordered_map<std::string, SoundResource>::const_iterator it = m_SoundResources.find(key);
		if (it != m_SoundResources.end()) {
			return const_cast<SoundResource*>(&it->second);
		}
		return nullptr;
	}
};