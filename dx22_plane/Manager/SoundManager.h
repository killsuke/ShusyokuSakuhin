#pragma once
#include <xaudio2.h>
#include <unordered_map>
#include <string>

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

	static inline IXAudio2* m_pXAudio2 = nullptr;						// XAudio2インターフェース
	static inline IXAudio2MasteringVoice* m_pMasteringVoice = nullptr;	// マスタリングボイス

	// コンストラクタ・デストラクタを削除
	SoundManager() = delete;
	~SoundManager() = delete;

	// コピー・ムーブも削除
	SoundManager(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;

	static HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);
	static HRESULT ReadChunkData(HANDLE, void*, DWORD, DWORD);

public:

	// ゲームループ開始前に呼び出すサウンドの初期化処理
	static HRESULT Init();

	// ゲームループ終了後に呼び出すサウンドの解放処理
	static void UnInit();

	static HRESULT LoadWave(const std::string& key, const char* filename, bool loop);

	static IXAudio2* GetXAudio2() { return m_pXAudio2; };
	static IXAudio2MasteringVoice* GetMasteringVoice() { return m_pMasteringVoice; };
	static SoundResource* GetSoundResource(const std::string& key) {
		auto it = m_SoundResources.find(key);
		if (it != m_SoundResources.end()) {
			return &it->second;
		}
		return nullptr;
	}
};