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
	// 全てのソースボイスを破棄
	for (const std::pair<const std::string, SourceVoiceData>& entry : m_pSourceVoices) {
		const SourceVoiceData& sourceVoiceData = entry.second;
		if (sourceVoiceData.pSourceVoice) {
			sourceVoiceData.pSourceVoice->Stop(0);
			sourceVoiceData.pSourceVoice->FlushSourceBuffers();
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

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		HRESULT hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) return;
	}
	else
	{
		pSV->Stop(0);
		pSV->FlushSourceBuffers();
	}

	pSV->SubmitSourceBuffer(&soundRes->buffer);
	pSV->Start(0);

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

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		HRESULT hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) return;
	}
	else
	{
		pSV->Stop(0);
		pSV->FlushSourceBuffers();
	}

	pSV->SubmitSourceBuffer(&soundRes->buffer);
	pSV->Start(0);
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

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		HRESULT hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) return;
	}
	else
	{
		pSV->Stop(0);
		pSV->FlushSourceBuffers();
	}

	pSV->SubmitSourceBuffer(&soundRes->buffer);

	pSV->SetVolume(volume);
	pSV->Start(0);
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

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		HRESULT hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) return;
	}
	else
	{
		pSV->Stop(0);
		pSV->FlushSourceBuffers();
	}

	pSV->SubmitSourceBuffer(&soundRes->buffer);
	pSV->SetVolume(volume);
	pSV->Start(0);
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

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		HRESULT hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) return;
	}
	else
	{
		pSV->Stop(0);
		pSV->FlushSourceBuffers();
	}

	pSV->SubmitSourceBuffer(&soundRes->buffer);
	pSV->Start(0);
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

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		HRESULT hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) return;
	}
	else
	{
		pSV->Stop(0);
		pSV->FlushSourceBuffers();
	}

	pSV->SubmitSourceBuffer(&soundRes->buffer);
	pSV->Start(0);
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

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		HRESULT hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) return;
	}
	else
	{
		pSV->Stop(0);
		pSV->FlushSourceBuffers();
	}

	pSV->SubmitSourceBuffer(&soundRes->buffer);

	pSV->SetVolume(volume);
	pSV->Start(0);
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

	// ソースボイスが存在しない場合は作成、存在する場合は停止してバッファをクリア
	if (pSV == nullptr)
	{
		HRESULT hr = xAudio->CreateSourceVoice(&pSV, &soundRes->wfx.Format);
		if (FAILED(hr)) return;
	}
	else
	{
		pSV->Stop(0);
		pSV->FlushSourceBuffers();
	}

	pSV->SubmitSourceBuffer(&soundRes->buffer);
	pSV->SetVolume(volume);
	pSV->Start(0);
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

	XAUDIO2_VOICE_STATE xa2state = {};
	sourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued > 0)
	{
		// 一時停止
		HRESULT hr = sourceVoice->Stop(0);
		if (FAILED(hr))
		{
			// ログ出力やエラーハンドリング
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

	XAUDIO2_VOICE_STATE xa2state = {};
	sourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued > 0)
	{
		// 一時停止
		HRESULT hr = sourceVoice->Stop(0);
		if (FAILED(hr))
		{
			// ログ出力やエラーハンドリング
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
		XAUDIO2_VOICE_STATE xa2state = {};
		sourceVoice->GetState(&xa2state);
		if (xa2state.BuffersQueued > 0)
		{
			// 一時停止
			HRESULT hr = sourceVoice->Stop(0);
			if (FAILED(hr))
			{
				// ログ出力やエラーハンドリング
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

	sourceVoice->Start();
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
	sourceVoice->SetVolume(volume);
	sourceVoice->Start();
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

	sourceVoice->Start();
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
	sourceVoice->SetVolume(volume);
	sourceVoice->Start();
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
	sourceVoice->SetVolume(clampedVolume);
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
	sourceVoice->SetVolume(clampedVolume);
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
	if(newVolume > 0.0f) {

		newVolume = std::min(it->second.maxVolume, newVolume);
	}
	else if(newVolume < 0.0f) {

		newVolume = std::max(0.0f, newVolume);
	}

	sourceVoice->SetVolume(newVolume);
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
	if(newVolume > 0.0f) {
		newVolume = std::min(it->second.maxVolume, newVolume);
	}
	else if(newVolume < 0.0f) {
		newVolume = std::max(0.0f, newVolume);
	}

	sourceVoice->SetVolume(newVolume);
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
