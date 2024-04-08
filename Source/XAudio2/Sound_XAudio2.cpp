/*===================================================================================
*    Date : 2022/01/20(金)
*        Author		: Gakuto.S
*        File		: Sound_XAudio2.cpp
*        Detail		:
===================================================================================*/
#include "Sound_XAudio2.h"

#include "Sound_XAudio2_Controller.h"

SoundXAudio2::SoundXAudio2()
	:m_masterVolume(10),
	m_bgmVolume(50),
	m_seVolume(50)
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

void SoundXAudio2::Update()
{
	for (size_t i = 0; i < m_backGround.size(); ++i)
	{
		if (m_backGround[i] != nullptr)
		{
			m_backGround[i]->Update();
		}
			
	}
}

ISoundController* SoundXAudio2::CreateSoundController(const char* fileName, bool stream)
{
	SoundXAudio2Controller* ret{};

	if (stream)
	{// ストリーミング再生
		size_t index = m_backGround.size();
		ret = new SoundXAudio2Controller(stream, index, true);
		ret->Load(fileName, m_xaudio2);
		ret->SetVolume((m_bgmVolume * m_masterVolume) / 100);

		// BGMとして管理する
		m_backGround.push_back(ret);
	}
	else
	{
		size_t index = m_effects.size();
		ret = new SoundXAudio2Controller(stream, index, false);
		ret->Load(fileName, m_xaudio2);
		ret->SetVolume((m_seVolume * m_masterVolume) / 100);

		// SEとして管理する
		m_effects.push_back(ret);
	}
	
	return ret;
}

void SoundXAudio2::DestroySoundController(ISoundController* controller)
{
	controller->Stop();

	if (controller->m_stream)
	{// 更新対象から外す
		m_backGround[controller->m_controlID]->Unload();
		m_backGround[controller->m_controlID] = nullptr;
	}
	else
	{
		m_effects[controller->m_controlID]->Unload();
		m_effects[controller->m_controlID] = nullptr;
	}
	delete controller;
}

void SoundXAudio2::Play()
{
	for (auto& index : m_runningBGMIndices)
		m_backGround[index]->Play();

	for (auto& index : m_runningSEIndices)
		m_effects[index]->Play();

	m_runningBGMIndices.clear();
	m_runningSEIndices.clear();
}

void SoundXAudio2::Stop()
{
	// 再生されているBGMを停止する処理
	for (auto& bgm : m_backGround)
	{
		if (bgm)
		{
			if (bgm->GetRunning())
			{
				bgm->Stop();
				m_runningBGMIndices.push_back(bgm->m_controlID);
			}
		}
	}

	// 再生されているSEを停止する処理
	for (auto& se : m_effects)
	{
		if (se)
		{
			if (se->GetRunning())
			{
				se->Stop();
				m_runningSEIndices.push_back(se->m_controlID);
			}
		}
	}
}
