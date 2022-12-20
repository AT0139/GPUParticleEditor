#pragma once

#include <xaudio2.h>
#include "GameObject.h"

class Audio : public GameObject
{
private:
	static IXAudio2* m_xAudio;
	static IXAudio2MasteringVoice* m_masteringVoice;

	IXAudio2SourceVoice* m_pSourceVoice;
	BYTE* m_pSoundData;

	int	m_length;
	int	m_playLength;

public:
	static void InitMaster();
	static void UninitMaster();

	void Init() {};
	void Uninit();
	void Update() {};
	void Draw() {};

	void Load(const char* fileName);
	void Play(bool isLoop = false);
};
