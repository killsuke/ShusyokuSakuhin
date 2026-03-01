#pragma once

#include <xaudio2.h>
#include <unordered_map>
#include <string>
#include "Component.h"
#include "Manager/SoundManager.h"

class SoundComponent final : public Component {
private:

	std::unordered_map<std::string, IXAudio2SourceVoice*> m_pSourceVoice;

public:

	SoundComponent(GameObject& obj);	// デフォルトコンストラクタ
	~SoundComponent();	// デフォルトデストラクタ

	void Update() override;

	void AddSoundLabel(const std::string& label);

	// 引数で指定したサウンドを再生する
	void Play(const std::string& label);

	// 引数で指定したサウンドを停止する
	void Stop(const std::string& label);

	// 引数で指定したサウンドの再生を再開する
	void Resume(const std::string& label);

	// 引数で指定したサウンドの音量を調整する
	void SetVolume(const std::string& label,const float volume);
};