
#include "main.h"
#include "audio.h"





IXAudio2*				Audio::m_xAudio = NULL;
IXAudio2MasteringVoice*	Audio::m_masteringVoice = NULL;


void Audio::InitMaster()
{
	// COM初期化
	(void)CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio生成
	XAudio2Create(&m_xAudio, 0);

	// マスタリングボイス生成
	m_xAudio->CreateMasteringVoice(&m_masteringVoice);
}


void Audio::UninitMaster()
{
	m_masteringVoice->DestroyVoice();
	m_xAudio->Release();
	CoUninitialize();
}









void Audio::Load(const char *fileName)
{

	// サウンドデータ読込
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)fileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		m_pSoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)m_pSoundData, buflen);


		m_length = readlen;
		m_playLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// サウンドソース生成
	m_xAudio->CreateSourceVoice(&m_pSourceVoice, &wfx);
	assert(m_pSourceVoice);
}


void Audio::Uninit()
{
	m_pSourceVoice->Stop();
	m_pSourceVoice->DestroyVoice();

	delete[] m_pSoundData;
}





void Audio::Play(bool isLoop)
{
	m_pSourceVoice->Stop();
	m_pSourceVoice->FlushSourceBuffers();


	// バッファ設定
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = m_length;
	bufinfo.pAudioData = m_pSoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = m_playLength;

	// ループ設定
	if (isLoop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = m_playLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	m_pSourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

/*
	float outputMatrix[4] = { 0.0f , 0.0f, 1.0f , 0.0f };
	m_SourceVoice->SetOutputMatrix(m_MasteringVoice, 2, 2, outputMatrix);
	//m_SourceVoice->SetVolume(0.1f);
*/


	// 再生
	m_pSourceVoice->Start();

}



