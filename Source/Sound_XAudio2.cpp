/*===================================================================================
*    Date : 2022/01/20(金)
*        Author		: Gakuto.S
*        File		: Sound_XAudio2.cpp
*        Detail		:
===================================================================================*/
#include "Sound_XAudio2.h"

SoundXAudio2::SoundXAudio2()
{
	HRESULT ret{};

	// Media Foundationの初期化処理
	ret = MFStartup(MF_VERSION);
	if (FAILED(ret))
		throw("Media Foundation の初期化に失敗");

	// XAudio2の初期化
	ret = XAudio2Create(&m_xaudio2);
	if (FAILED(ret))
		throw("XAudio の初期化に失敗");

	// マスターボイスの生成
	ret = m_xaudio2->CreateMasteringVoice(&m_masterVoice);
	if (FAILED(ret))
		throw("Master voice の生成に失敗");
}

SoundXAudio2::~SoundXAudio2()
{
	if (m_masterVoice)	m_masterVoice->DestroyVoice();
	if (m_xaudio2)		m_xaudio2->Release();
	MFShutdown();
}
