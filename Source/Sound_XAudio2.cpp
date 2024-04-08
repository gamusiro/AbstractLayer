/*===================================================================================
*    Date : 2022/01/20(��)
*        Author		: Gakuto.S
*        File		: Sound_XAudio2.cpp
*        Detail		:
===================================================================================*/
#include "Sound_XAudio2.h"

SoundXAudio2::SoundXAudio2()
{
	HRESULT ret{};

	// Media Foundation�̏���������
	ret = MFStartup(MF_VERSION);
	if (FAILED(ret))
		throw("Media Foundation �̏������Ɏ��s");

	// XAudio2�̏�����
	ret = XAudio2Create(&m_xaudio2);
	if (FAILED(ret))
		throw("XAudio �̏������Ɏ��s");

	// �}�X�^�[�{�C�X�̐���
	ret = m_xaudio2->CreateMasteringVoice(&m_masterVoice);
	if (FAILED(ret))
		throw("Master voice �̐����Ɏ��s");
}

SoundXAudio2::~SoundXAudio2()
{
	if (m_masterVoice)	m_masterVoice->DestroyVoice();
	if (m_xaudio2)		m_xaudio2->Release();
	MFShutdown();
}
