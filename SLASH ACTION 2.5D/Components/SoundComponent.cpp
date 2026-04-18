#include "SoundComponent.h"
#include <algorithm>

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

SoundComponent::SoundComponent(GameObject& obj) :Component(obj)
{
	m_SortNum = ComponentTypeManager::GetID_FromName("SOUND"); // ソート番号を設定
	m_pSourceVoices.clear();
}

SoundComponent::~SoundComponent()
{
	HRESULT hr = S_OK;
	// 全てのソースボイスを破棄
	for (const std::pair<const std::string, SourceVoiceData>& entry : m_pSourceVoices) {
		const SourceVoiceData& sourceVoiceData = entry.second;
		if (sourceVoiceData.pSourceVoice) {
			hr = sourceVoiceData.pSourceVoice->Stop(0);
			if (FAILED(hr)) {
				MessageBoxW(nullptr, L"ソースボイスの停止に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
				continue;
			}
			hr = sourceVoiceData.pSourceVoice->FlushSourceBuffers();
			if (FAILED(hr)) {
				MessageBoxW(nullptr, L"ソースボイスのバッファのフラッシュに失敗しました。", L"Error", MB_ICONERROR | MB_OK);
				continue;
			}
			sourceVoiceData.pSourceVoice->DestroyVoice();
		}
	}

	m_pSourceVoices.clear();
}

void SoundComponent::Update()
{
	// 特に更新処理は無し
}

// =============================================================================
// サウンドラベルの追加
// =============================================================================
void SoundComponent::AddSoundLabel(const std::string& label) {

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		// 見つからなかった場合、新しいエントリを追加
		m_pSourceVoices.emplace(label, nullptr);
	}
}

void SoundComponent::Play() {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.begin();

	if (it == m_pSourceVoices.end()) {
		return;
	};

	const std::string key = it->first;

	IXAudio2* xAudio = SoundManager::GetXAudio2();
	if (xAudio == nullptr) {
		return;
	}

	SoundResource* soundRes = SoundManager::GetSoundResource(key);
	if (soundRes == nullptr) {
		return;
	}

	IXAudio2SourceVoice*& pSV = it->second.pSourceVoice;

	HRESULT hr = S_OK;

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの作成に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}
	else
	{
		hr = pSV->Stop(0);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの停止に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
		hr = pSV->FlushSourceBuffers();
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスのバッファのフラッシュに失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}

	hr = pSV->SubmitSourceBuffer(&soundRes->buffer);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスへのバッファの送信に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}

	hr = pSV->Start(0);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスの再生の開始に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}
}

//=============================================================================
// 再生
//=============================================================================
void SoundComponent::Play(const std::string& label)
{
	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		return;
	};

	IXAudio2* xAudio = SoundManager::GetXAudio2();
	if (xAudio == nullptr) {
		return;
	}

	SoundResource* soundRes = SoundManager::GetSoundResource(label);
	if (soundRes == nullptr) {
		return;
	}

	IXAudio2SourceVoice*& pSV = it->second.pSourceVoice;

	HRESULT hr = S_OK;

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの作成に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}
	else
	{
		hr = pSV->Stop(0);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの停止に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
		hr = pSV->FlushSourceBuffers();
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスのバッファのフラッシュに失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}

	hr = pSV->SubmitSourceBuffer(&soundRes->buffer);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスへのバッファの送信に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}

	hr = pSV->Start(0);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスの再生の開始に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}
}

void SoundComponent::Play(const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.begin();
	if (it == m_pSourceVoices.end()) {
		return;
	};

	const std::string key = it->first;

	IXAudio2* xAudio = SoundManager::GetXAudio2();
	if (xAudio == nullptr) {
		return;
	}

	SoundResource* soundRes = SoundManager::GetSoundResource(key);
	if (soundRes == nullptr) {
		return;
	}

	IXAudio2SourceVoice*& pSV = it->second.pSourceVoice;

	HRESULT hr = S_OK;

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの作成に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}
	else
	{
		hr = pSV->Stop(0);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの停止に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}

		hr = pSV->FlushSourceBuffers();
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスのバッファのフラッシュに失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}

	hr = pSV->SubmitSourceBuffer(&soundRes->buffer);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスへのバッファの送信に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}

	hr = pSV->SetVolume(volume);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスの音量の設定に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}

	hr = pSV->Start(0);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスの再生の開始に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}
}

void SoundComponent::Play(const std::string& label, const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		return;
	};

	IXAudio2* xAudio = SoundManager::GetXAudio2();
	if (xAudio == nullptr) {
		return;
	}

	SoundResource* soundRes = SoundManager::GetSoundResource(label);
	if (soundRes == nullptr) {
		return;
	}

	IXAudio2SourceVoice*& pSV = it->second.pSourceVoice;

	HRESULT hr = S_OK;

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの作成に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}
	else
	{
		hr = pSV->Stop(0);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの停止に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
		
		hr = pSV->FlushSourceBuffers();
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスのバッファのフラッシュに失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}

	hr = pSV->SubmitSourceBuffer(&soundRes->buffer);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスへのバッファの送信に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}

	hr = pSV->SetVolume(volume);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスの音量の設定に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}

	hr = pSV->Start(0);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスの再生の開始に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}
}

void SoundComponent::PlayOnce() {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.begin();

	if (it == m_pSourceVoices.end()) {
		return;
	};

	if (it->second.isPlayed == true) {
		return;
	}

	it->second.isPlayed = true;

	const std::string key = it->first;

	IXAudio2* xAudio = SoundManager::GetXAudio2();
	if (xAudio == nullptr) {
		return;
	}

	SoundResource* soundRes = SoundManager::GetSoundResource(key);
	if (soundRes == nullptr) {
		return;
	}

	IXAudio2SourceVoice*& pSV = it->second.pSourceVoice;

	HRESULT hr = S_OK;

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの作成に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}
	else
	{
		hr = pSV->Stop(0);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの停止に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}

		hr = pSV->FlushSourceBuffers();
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスのバッファのフラッシュに失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}

	hr = pSV->SubmitSourceBuffer(&soundRes->buffer);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスへのバッファの送信に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}

	hr = pSV->Start(0);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスの再生の開始に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
	}
}

void SoundComponent::PlayOnce(const std::string& label) {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		return;
	};

	if (it->second.isPlayed == true) {
		return;
	}

	it->second.isPlayed = true;

	IXAudio2* xAudio = SoundManager::GetXAudio2();
	if (xAudio == nullptr) {
		return;
	}

	SoundResource* soundRes = SoundManager::GetSoundResource(label);
	if (soundRes == nullptr) {
		return;
	}

	IXAudio2SourceVoice*& pSV = it->second.pSourceVoice;

	HRESULT hr = S_OK;

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの作成に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}
	else
	{
		hr = pSV->Stop(0);
		if(FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの停止に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}

		hr = pSV->FlushSourceBuffers();
		if(FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスのバッファのフラッシュに失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}

	hr = pSV->SubmitSourceBuffer(&soundRes->buffer);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスへのバッファの送信に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}
	
	hr = pSV->Start(0);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスの再生の開始に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
	}
}

void SoundComponent::PlayOnce(const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.begin();
	if (it == m_pSourceVoices.end()) {
		return;
	};

	const std::string key = it->first;

	if (it->second.isPlayed == true) {
		return;
	}

	it->second.isPlayed = true;

	IXAudio2* xAudio = SoundManager::GetXAudio2();
	if (xAudio == nullptr) {
		return;
	}

	SoundResource* soundRes = SoundManager::GetSoundResource(key);
	if (soundRes == nullptr) {
		return;
	}

	IXAudio2SourceVoice*& pSV = it->second.pSourceVoice;

	HRESULT hr = S_OK;

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの作成に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}
	else
	{
		hr = pSV->Stop(0);
		if(FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの停止に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}

		hr = pSV->FlushSourceBuffers();
		if(FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスのバッファのフラッシュに失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}

	hr = pSV->SubmitSourceBuffer(&soundRes->buffer);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスへのバッファの送信に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}

	hr = pSV->SetVolume(volume);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスの音量の設定に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}

	hr = pSV->Start(0);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスの再生の開始に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
	}
}

void SoundComponent::PlayOnce(const std::string& label, const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		return;
	};

	if (it->second.isPlayed == true) {
		return;
	}

	it->second.isPlayed = true;

	IXAudio2* xAudio = SoundManager::GetXAudio2();
	if (xAudio == nullptr) {
		return;
	}

	SoundResource* soundRes = SoundManager::GetSoundResource(label);
	if (soundRes == nullptr) {
		return;
	}

	IXAudio2SourceVoice*& pSV = it->second.pSourceVoice;

	HRESULT hr = S_OK;

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの作成に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}
	else
	{
		hr = pSV->Stop(0);
		if(FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスの停止に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}

		hr = pSV->FlushSourceBuffers();
		if(FAILED(hr)) {
			MessageBoxW(nullptr, L"ソースボイスのバッファのフラッシュに失敗しました。", L"Error", MB_ICONERROR | MB_OK);
			return;
		}
	}

	hr = pSV->SubmitSourceBuffer(&soundRes->buffer);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスへのバッファの送信に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}

	hr = pSV->SetVolume(volume);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスの音量の設定に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
		return;
	}

	hr = pSV->Start(0);
	if(FAILED(hr)) {
		MessageBoxW(nullptr, L"ソースボイスの再生の開始に失敗しました。", L"Error", MB_ICONERROR | MB_OK);
	}
}

void SoundComponent::Stop() {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.begin();

	if (it == m_pSourceVoices.end()) {
		return;
	};

	it->second.isPlayed = false;

	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;
	if(sourceVoice == nullptr) {
		return;
	}

	XAUDIO2_VOICE_STATE xa2state = {};
	sourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued > 0)
	{
		// 一時停止
		HRESULT hr = sourceVoice->Stop(0);
		if (FAILED(hr))
		{
			MessageBoxW(nullptr, L"サウンドの停止に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
		}
	}
}

//=============================================================================
// 停止
//=============================================================================
void SoundComponent::Stop(const std::string& label)
{
	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		return;
	}

	it->second.isPlayed = false;

	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;
	if(sourceVoice == nullptr) {
		return;
	}

	XAUDIO2_VOICE_STATE xa2state = {};
	sourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued > 0)
	{
		// 一時停止
		HRESULT hr = sourceVoice->Stop(0);
		if (FAILED(hr))
		{
			MessageBoxW(nullptr, L"サウンドの停止に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
		}
	}
}

void SoundComponent::StopAll() {

	if (m_pSourceVoices.empty()) {
		return;
	}

	for (std::pair<const std::string, SourceVoiceData>& entry : m_pSourceVoices) {

		entry.second.isPlayed = false;
		IXAudio2SourceVoice* sourceVoice = entry.second.pSourceVoice;
		if(sourceVoice == nullptr) {
			continue;
		}
		XAUDIO2_VOICE_STATE xa2state = {};
		sourceVoice->GetState(&xa2state);
		if (xa2state.BuffersQueued > 0)
		{
			// 一時停止
			HRESULT hr = sourceVoice->Stop(0);
			if (FAILED(hr))
			{
				MessageBoxW(nullptr, L"サウンドの停止に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
			}
		}
	}
}

//=============================================================================
// 再開
//=============================================================================
void SoundComponent::Resume() {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.begin();

	if (it == m_pSourceVoices.end()) {
		return;
	};

	// サウンド再生再開
	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;

	if (sourceVoice == nullptr) {
		return;
	}

	HRESULT hr = sourceVoice->Start();
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"サウンドの再開に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
	}
}

void SoundComponent::Resume(const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.begin();
	if (it == m_pSourceVoices.end()) {
		return;
	};

	// サウンド再生再開
	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;
	if (sourceVoice == nullptr) {
		return;
	}

	HREFTYPE hr = S_OK;

	hr = sourceVoice->SetVolume(volume);
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"サウンドの音量調整に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
		return;
	}

	hr = sourceVoice->Start();
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"サウンドの再開に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
	}
}

void SoundComponent::Resume(const std::string& label) {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		return;
	}

	// サウンド再生再開
	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;

	if (sourceVoice == nullptr) {
		return;
	}

	HRESULT hr = sourceVoice->Start();
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"サウンドの再開に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
	}
}

void SoundComponent::Resume(const std::string& label, const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		return;
	}

	// サウンド再生再開
	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;
	if (sourceVoice == nullptr) {
		return;
	}

	HRESULT hr = sourceVoice->SetVolume(volume);
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"サウンドの音量調整に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
		return;
	}

	hr = sourceVoice->Start();
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"サウンドの再開に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
	}
}

void SoundComponent::SetVolume(const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.begin();

	if (it == m_pSourceVoices.end()) {
		return;
	};

	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;

	if (sourceVoice == nullptr) {
		return;
	}

	const float maxVolume = it->second.maxVolume;

	const float clampedVolume = std::clamp(volume, 0.0f, maxVolume);

	// 音量調整
	HRESULT hr = sourceVoice->SetVolume(clampedVolume);
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"サウンドの音量調整に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
	}
}

//=============================================================================
// 音量調整
//=============================================================================
void SoundComponent::SetVolume(const std::string& label, const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}

	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		return;
	}

	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;

	if (sourceVoice == nullptr) {
		return;
	}

	const float maxVolume = it->second.maxVolume;
	const float clampedVolume = std::clamp(volume, 0.0f, maxVolume);

	// 音量調整
	HRESULT hr = sourceVoice->SetVolume(clampedVolume);
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"サウンドの音量調整に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
	}
}

void SoundComponent::AddVolume(const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}
	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.begin();
	if (it == m_pSourceVoices.end()) {
		return;
	};
	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;
	if (sourceVoice == nullptr) {
		return;
	}
	// 音量調整
	float currentVolume = 0.0f;
	sourceVoice->GetVolume(&currentVolume);

	float newVolume = currentVolume + volume;

	// 最大音量を超えた場合は最大音量に設定、最小音量を下回った場合は最小音量に設定
	if (newVolume > 0.0f) {

		newVolume = std::min(it->second.maxVolume, newVolume);
	}
	else if (newVolume < 0.0f) {

		newVolume = std::max(0.0f, newVolume);
	}

	HRESULT hr = sourceVoice->SetVolume(newVolume);
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"サウンドの音量調整に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
	}
}

void SoundComponent::AddVolume(const std::string& label, const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}
	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		return;
	}
	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;
	if (sourceVoice == nullptr) {
		return;
	}
	// 音量調整
	float currentVolume = 0.0f;
	sourceVoice->GetVolume(&currentVolume);

	float newVolume = currentVolume + volume;

	// 最大音量を超えた場合は最大音量に設定、最小音量を下回った場合は最小音量に設定
	if (newVolume > 0.0f) {
		newVolume = std::min(it->second.maxVolume, newVolume);
	}
	else if (newVolume < 0.0f) {
		newVolume = std::max(0.0f, newVolume);
	}

	HRESULT hr = sourceVoice->SetVolume(newVolume);
	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"サウンドの音量調整に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
	}
}

void SoundComponent::SetMaxVolume(const float volume) {

	const float clampedVolume = std::clamp(volume, 0.0f, 1.0f);

	if (m_pSourceVoices.empty()) {
		return;
	}
	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.begin();
	if (it == m_pSourceVoices.end()) {
		return;
	};
	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;
	if (sourceVoice == nullptr) {
		return;
	}

	it->second.maxVolume = clampedVolume;
}

void SoundComponent::SetMaxVolume(const std::string& label, const float volume) {

	const float clampedVolume = std::clamp(volume, 0.0f, 1.0f);
	if (m_pSourceVoices.empty()) {
		return;
	}
	std::unordered_map<std::string, SourceVoiceData>::iterator it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		return;
	}
	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;
	if (sourceVoice == nullptr) {
		return;
	}

	it->second.maxVolume = clampedVolume;
}
