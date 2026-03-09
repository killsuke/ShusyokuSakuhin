#include "SoundComponent.h"

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
	for (const auto& [label, pSourceVoice] : m_pSourceVoices) {
		if (pSourceVoice.pSourceVoice) {
			pSourceVoice.pSourceVoice->Stop(0);
			pSourceVoice.pSourceVoice->FlushSourceBuffers();
			pSourceVoice.pSourceVoice->DestroyVoice();
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

	const auto it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		// 見つからなかった場合、新しいエントリを追加
		m_pSourceVoices.emplace(label, nullptr);
	}
}

void SoundComponent::Play() {

	if (m_pSourceVoices.empty()) {
		return;
	}

	const auto it = m_pSourceVoices.begin();

	if (it == m_pSourceVoices.end()) {
		return;
	};

	const auto key = it->first;

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

	const auto it = m_pSourceVoices.find(label);
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

	const auto it = m_pSourceVoices.begin();
	if (it == m_pSourceVoices.end()) {
		return;
	};

	const auto key = it->first;

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

	const auto it = m_pSourceVoices.find(label);
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

	const auto it = m_pSourceVoices.begin();

	if (it == m_pSourceVoices.end()) {
		return;
	};

	if (it->second.isPlayed == true) {
		return;
	}

	it->second.isPlayed = true;

	const auto key = it->first;

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

	const auto it = m_pSourceVoices.find(label);
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

	const auto it = m_pSourceVoices.begin();
	if (it == m_pSourceVoices.end()) {
		return;
	};

	const auto key = it->first;

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

	const auto it = m_pSourceVoices.find(label);
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

	const auto it = m_pSourceVoices.begin();

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

	const auto it = m_pSourceVoices.find(label);
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

	for (auto& [label, sourceVoiceData] : m_pSourceVoices) {

		sourceVoiceData.isPlayed = false;
		IXAudio2SourceVoice* sourceVoice = sourceVoiceData.pSourceVoice;
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

	const auto it = m_pSourceVoices.begin();

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

	const auto it = m_pSourceVoices.begin();
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

	const auto it = m_pSourceVoices.find(label);
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

	const auto it = m_pSourceVoices.find(label);
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

	const auto it = m_pSourceVoices.begin();

	if (it == m_pSourceVoices.end()) {
		return;
	};

	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;

	if (sourceVoice == nullptr) {
		return;
	}

	// 音量調整
	sourceVoice->SetVolume(volume);
}

//=============================================================================
// 音量調整
//=============================================================================
void SoundComponent::SetVolume(const std::string& label, const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}

	const auto it = m_pSourceVoices.find(label);
	if (it == m_pSourceVoices.end()) {
		return;
	}

	IXAudio2SourceVoice* sourceVoice = it->second.pSourceVoice;

	if (sourceVoice == nullptr) {
		return;
	}

	// 音量調整
	sourceVoice->SetVolume(volume);
}


void SoundComponent::AddVolume(const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}
	const auto it = m_pSourceVoices.begin();
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
	sourceVoice->SetVolume(currentVolume + volume);
}

void SoundComponent::AddVolume(const std::string& label, const float volume) {

	if (m_pSourceVoices.empty()) {
		return;
	}
	const auto it = m_pSourceVoices.find(label);
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
	sourceVoice->SetVolume(currentVolume + volume);
}