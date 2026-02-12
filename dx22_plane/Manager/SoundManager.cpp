#include "SoundManager.h"

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
	/**** Create XAudio2 ****/
	hr = XAudio2Create(&m_pXAudio2, 0);		// 第二引数は､動作フラグ デバッグモードの指定(現在は未使用なので0にする)
	//hr=XAudio2Create(&g_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);		// 第三引数は、windowsでは無視
	if (FAILED(hr)) {
		CoUninitialize();
		return -1;
	}

	// スピーカーへの最終出力ノード作成
	/**** Create Mastering Voice ****/
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);			// 今回はＰＣのデフォルト設定に任せている
	/*, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL*/		// 本当６個の引数を持っている
	if (FAILED(hr)) {
		if (m_pXAudio2)	m_pXAudio2->Release();
		CoUninitialize();
		return -1;
	}

	LoadWave("slash", "sound/SE/slash.wav", false);
	LoadWave("slashHit", "sound/SE/slashHit.wav", false);

	return hr;
}

void SoundManager::UnInit() {

	for(const auto& [key, res] : m_SoundResources)
	{
		if(res.pDataBuffer)
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
HRESULT SoundManager::FindChunk(const HANDLE& hFile,const DWORD& fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
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

HRESULT SoundManager::ReadChunkData(const HANDLE& hFile,void* buffer,const DWORD& buffersize,const DWORD& bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}


HRESULT SoundManager::LoadWave(const std::string& key, const char* filename,const bool loop) {

	SoundResource res{};

	const HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD chunkSize = 0;
	DWORD chunkPosition = 0;
	DWORD filetype = 0;

	HRESULT hr = S_OK;

	// RIFF
	hr = FindChunk(hFile, fourccRIFF, chunkSize, chunkPosition);
	if(FAILED(hr)) {
		CloseHandle(hFile);
		return hr;
	}

	hr = ReadChunkData(hFile, &filetype, sizeof(DWORD), chunkPosition);
	if(FAILED(hr)) {
		CloseHandle(hFile);
		return hr;
	}

	// FMT
	hr = FindChunk(hFile, fourccFMT, chunkSize, chunkPosition);
	if(FAILED(hr)) {
		CloseHandle(hFile);
		return hr;
	}

	hr = ReadChunkData(hFile, &res.wfx, chunkSize, chunkPosition);
	if(FAILED(hr)) {
		CloseHandle(hFile);
		return hr;
	}

	// DATA
	hr = FindChunk(hFile, fourccDATA, chunkSize, chunkPosition);
	res.pDataBuffer = new BYTE[chunkSize];
	if(!res.pDataBuffer) {
		CloseHandle(hFile);
		return E_FAIL;
	}

	hr = ReadChunkData(hFile, res.pDataBuffer, chunkSize, chunkPosition);
	if(FAILED(hr)) {
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