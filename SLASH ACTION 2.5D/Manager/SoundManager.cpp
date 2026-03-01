#include "SoundManager.h"
using json = nlohmann::ordered_json;
using namespace std::filesystem;

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif
#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif


HRESULT SoundManager::Init() {

	HRESULT hr;

	// COMの初期化
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		CoUninitialize();
		return -1;
	}

	// 音声エンジン作成
	hr = XAudio2Create(&m_pXAudio2, 0);		// 第二引数は､動作フラグ デバッグモードの指定(現在は未使用なので0にする)
	if (FAILED(hr)) {
		CoUninitialize();
		return -1;
	}

	// スピーカーへの最終出力ノード作成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);			// 今回はＰＣのデフォルト設定に任せている
	if (FAILED(hr)) {
		if (m_pXAudio2)	m_pXAudio2->Release();
		CoUninitialize();
		return -1;
	}

	LoadSoundJsonFile();

	LoadSoundFiles();

	return hr;
}

void SoundManager::UnInit() {

	for (const std::pair<const std::string, SoundResource>& entry : m_SoundResources)
	{
		const SoundResource& res = entry.second;
		if (res.pDataBuffer)
		{
			delete[] res.pDataBuffer;
		}
	}

	m_pMasteringVoice->DestroyVoice();

	if (m_pXAudio2) m_pXAudio2->Release();

	// COMの破棄
	CoUninitialize();
}

//=============================================================================
// ユーティリティ関数群
//=============================================================================
HRESULT SoundManager::FindChunk(const HANDLE& hFile, const DWORD& fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;
	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}
		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}
		dwOffset += dwChunkDataSize;
		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}
	return S_OK;
}

HRESULT SoundManager::ReadChunkData(const HANDLE& hFile, void* buffer, const DWORD& buffersize, const DWORD& bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

void SoundManager::LoadSoundJsonFile() {

	std::ifstream ifs("json/sound.json");
	if (!ifs) {
		std::cerr << "ファイルを開けません: " << "json/sound.json" << "\n";
		return;
	}

	nlohmann::json j;
	ifs >> j;

	auto loadCategory = [&](const std::string& category,const SoundType& type) {

		for (const auto& item : j.at(category)) {
			SoundStatus st;
			st.type = type;

			// file名
			std::string file = item.at("file");

			// name は拡張子を除いたもの
			st.name = file.substr(0, file.find_last_of('.'));

			// path は自動生成
			st.path = "sound/" + category + "/" + file;

			// volume と loopは任意
			st.volume = item.value("volume", 1.0f);
			st.loop = item.value("loop", type == SoundType::BGM);

			m_SoundStatuses.push_back(st);
		}
	};

	loadCategory("BGM", SoundType::BGM);
	loadCategory("SE", SoundType::SE);
//	loadCategory("VOICE", SoundType::VOICE);
}

void SoundManager::LoadSoundFiles() {

	for (const auto& file : m_SoundStatuses) {

		const std::string name = file.name;
		const std::string path = file.path;
		const bool loop = file.loop;

		const char* c_path = path.c_str();

		// パスはキャラポインタ型であるべきか理論
		LoadWave(name, c_path, loop);
	}
}

HRESULT SoundManager::LoadWave(const std::string& key, const char* filename, const bool loop) {

	SoundResource res{};

	const HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD chunkSize = 0;
	DWORD chunkPosition = 0;
	DWORD filetype = 0;

	HRESULT hr = S_OK;

	// RIFF
	hr = FindChunk(hFile, fourccRIFF, chunkSize, chunkPosition);
	if (FAILED(hr)) {
		CloseHandle(hFile);
		return hr;
	}

	hr = ReadChunkData(hFile, &filetype, sizeof(DWORD), chunkPosition);
	if (FAILED(hr)) {
		CloseHandle(hFile);
		return hr;
	}

	// FMT
	hr = FindChunk(hFile, fourccFMT, chunkSize, chunkPosition);
	if (FAILED(hr)) {
		CloseHandle(hFile);
		return hr;
	}

	hr = ReadChunkData(hFile, &res.wfx, chunkSize, chunkPosition);
	if (FAILED(hr)) {
		CloseHandle(hFile);
		return hr;
	}

	// DATA
	hr = FindChunk(hFile, fourccDATA, chunkSize, chunkPosition);
	res.pDataBuffer = new BYTE[chunkSize];
	if (!res.pDataBuffer) {
		CloseHandle(hFile);
		return E_FAIL;
	}

	hr = ReadChunkData(hFile, res.pDataBuffer, chunkSize, chunkPosition);
	if (FAILED(hr)) {
		delete[] res.pDataBuffer;
		CloseHandle(hFile);
		return hr;
	}

	CloseHandle(hFile);

	// XAudio2 Buffer設定
	res.buffer.AudioBytes = chunkSize;
	res.buffer.pAudioData = res.pDataBuffer;
	res.buffer.Flags = XAUDIO2_END_OF_STREAM;
	res.buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
	res.bLoop = loop;

	m_SoundResources[key] = res;

	return hr;
}