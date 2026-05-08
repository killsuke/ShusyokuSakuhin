#pragma once

#include <xaudio2.h>
#include <unordered_map>
#include <string>
#include "Component.h"
#include "Manager/SoundManager.h"

struct SourceVoiceData {

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	float maxVolume = 1.0f;
	bool isPlayed = false;
};

class SoundComponent final : public Component {
private:

	std::unordered_map<std::string, SourceVoiceData> m_SourceVoices;

public:

	SoundComponent(GameObject& obj);	// デフォルトコンストラクタ
	~SoundComponent();	// デフォルトデストラクタ

	void Update() override;

	void AddSoundLabel(const std::string& label);

	// 登録されたサウンドが１つなら
	void Play();
	
	// 引数で指定したサウンドを再生する
	void Play(const std::string& label);

	void Play(const float volume);

	void Play(const std::string& label, const float volume);

	// 登録されたサウンドが１つなら
	void PlayOnce();

	// 引数で指定したサウンドを１回だけ再生する
	void PlayOnce(const std::string& label);

	void PlayOnce(const float volume);

	void PlayOnce(const std::string& label, const float volume);

	// 登録されたサウンドが１つなら
	void Stop();

	// 引数で指定したサウンドを停止する
	void Stop(const std::string& label);

	void StopAll();

	// 登録されたサウンドが１つなら
	void Resume();

	void Resume(const float volume);

	// 引数で指定したサウンドの再生を再開する
	void Resume(const std::string& label);

	void Resume(const std::string& label, const float volume);

	// 登録されたサウンドが１つなら
	void SetVolume(const float volume);

	// 引数で指定したサウンドの音量を調整する
	void SetVolume(const std::string& label,const float volume);

	void AddVolume(const float volume);

	void AddVolume(const std::string& label, const float volume);

	void SetMaxVolume(const float volume);

	void SetMaxVolume(const std::string& label, const float volume);
};