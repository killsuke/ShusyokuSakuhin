#include "SoundManager.h"

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

namespace {

	std::string BGMPath = "sound/BGM/";
	std::string SEPath = "sound/SE/";
}

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

	//LoadSoundJsonFile();

	//LoadSoundFiles();

	LoadFolder(BGMPath, true);
	LoadFolder(SEPath, false);

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

// 指定されたフォルダ内のすべてのファイルを読み込む
void SoundManager::LoadFolder(const std::string& path, const bool loop) {

	// 指定されたディレクトリのフォルダを開き読み込む
	for (auto& file : directory_iterator(path)) {

		// 通常ファイルであれば読み込む
		if (file.is_regular_file()) {

			std::string name = file.path().stem().string(); // 拡張子を除いたファイル名
			std::string full_path = file.path().string(); // フルパス

			const char* c_path = full_path.c_str();

			LoadWave(name, c_path, loop);
		}
	}
}

// WAVファイルを読み込む関数
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