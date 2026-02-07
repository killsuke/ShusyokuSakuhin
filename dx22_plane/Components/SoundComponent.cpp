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
	m_pSourceVoice.clear();
}

SoundComponent::~SoundComponent()
{
	for (const auto& [label, pSourceVoice] : m_pSourceVoice) {
		if (pSourceVoice) {
			pSourceVoice->Stop(0);
			pSourceVoice->FlushSourceBuffers();
			pSourceVoice->DestroyVoice();
		}
	}

	m_pSourceVoice.clear();
}

void SoundComponent::Update()
{
	// 特に更新処理は無し
}

// =============================================================================
// サウンドラベルの追加
// =============================================================================
void SoundComponent::AddSoundLabel(const std::string& label) {

	auto it = m_pSourceVoice.find(label);
	if (it == m_pSourceVoice.end()) {
		// 見つからなかった場合、新しいエントリを追加
		m_pSourceVoice.emplace(label, nullptr);
	}
}

//=============================================================================
// 再生
//=============================================================================
void SoundComponent::Play(const std::string& label)
{
	auto it = m_pSourceVoice.find(label);
	if (it == m_pSourceVoice.end()) return;

	IXAudio2* xAudio = SoundManager::GetXAudio2();
	if (xAudio == nullptr) {
		return;
	}

	SoundResource* soundRes = SoundManager::GetSoundResource(label);
	if (soundRes == nullptr) {
		return;
	}

	IXAudio2SourceVoice*& pSV = it->second;

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
// 停止
//=============================================================================
void SoundComponent::Stop(const std::string& label)
{
	if (m_pSourceVoice.empty()) {
		return;
	}

	auto it = m_pSourceVoice.find(label);
	if (it == m_pSourceVoice.end()) {
		return;
	}

	IXAudio2SourceVoice* sourceVoice = it->second;

	/*XAUDIO2_VOICE_STATE xa2state;
	m_pSourceVoice[(int)label]->GetState(&xa2state);
	if (xa2state.BuffersQueued)
	{
		m_pSourceVoice[(int)label]->Stop(0);
	}*/
	XAUDIO2_VOICE_STATE xa2state = {};
	sourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued > 0)
	{
		HRESULT hr = sourceVoice->Stop(0);
		if (FAILED(hr))
		{
			// ログ出力やエラーハンドリング
		}
	}
}

//=============================================================================
// 一時停止
//=============================================================================
void SoundComponent::Resume(const std::string& label)
{
	if (m_pSourceVoice.empty()) {
		return;
	}

	auto it = m_pSourceVoice.find(label);
	if (it == m_pSourceVoice.end()) {
		return;
	}

	IXAudio2SourceVoice* sourceVoice = it->second;

	if (sourceVoice == nullptr) {
		return;
	}

	sourceVoice->Start();
}

//=============================================================================
// 音量調整
//=============================================================================
void SoundComponent::SetVolume(const std::string& label, float volume) {

	if (m_pSourceVoice.empty()) {
		return;
	}

	auto it = m_pSourceVoice.find(label);
	if (it == m_pSourceVoice.end()) {
		return;
	}

	IXAudio2SourceVoice* sourceVoice = it->second;

	if(sourceVoice == nullptr) {
		return;
	}

	sourceVoice->SetVolume(volume);
}
