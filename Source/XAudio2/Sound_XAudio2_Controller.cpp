/*===================================================================================
*    Date : 2022/01/21(土)
*        Author		: Gakuto.S
*        File		: Sound_XAudio2_Controller.cpp
*        Detail		:
===================================================================================*/
#include "Sound_XAudio2_Controller.h"

#include "../ThirdLib/strconv.h"

using namespace std;

SoundXAudio2Controller::SoundXAudio2Controller(
	bool streaming,
	size_t id,
	bool loop
)
	:m_loop(loop),
	m_sourceVoice(),
    m_sourceReader(),
	m_mediaType(),
	m_voiceCallback(),
	m_waveFormatEx(),
	m_dataBuffers(),
    m_loading(false),
    m_bufferIndex(0),
	ISoundController(streaming, id)
{
}

bool SoundXAudio2Controller::Load(const char* fileName, IXAudio2* xaudio2)
{
	HRESULT ret{};

	// メディアデータの探索処理
	std::string filePath;
	if(m_stream)
		filePath = "Material\\Sound\\BGM\\";
	else
		filePath = "Material\\Sound\\SE\\";

	std::wstring wfilePath = cp_to_wide(string(filePath + fileName), CP_ACP);
	ret = MFCreateSourceReaderFromURL(wfilePath.c_str(), nullptr, &m_sourceReader);
	if (FAILED(ret))
		return false;

	// メディアタイプの指定
	ret = MFCreateMediaType(&m_mediaType);
	if (FAILED(ret))
		return false;

	m_mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	m_mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	m_sourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, m_mediaType);
	m_mediaType->Release();
	m_sourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &m_mediaType);


	// オーディオ情報の取得処理
	ret = MFCreateWaveFormatExFromMFMediaType(m_mediaType, &m_waveFormatEx, nullptr);
	if (FAILED(ret))
		return false;

	// ソースボイスの生成処理
	ret = xaudio2->CreateSourceVoice(&m_sourceVoice, m_waveFormatEx, XAUDIO2_VOICE_USEFILTER, 2.0f, &m_voiceCallback);
	if (FAILED(ret))
		return false;

	if (m_stream)
		this->SetPlayPosition(0);
	else
		this->LoadAudioData();

	return true;
}

void SoundXAudio2Controller::Unload()
{
	CoTaskMemFree(m_waveFormatEx);

	// メディアタイプの破棄
	if (m_mediaType)
	{
		m_mediaType->Release();
		m_mediaType = nullptr;
	}

	// ソースリーダーの破棄
	if (m_sourceReader)
	{
		m_sourceReader->Release();
		m_sourceReader = nullptr;
	}

	// ソースボイスの解放
	if (m_sourceVoice)
	{
		m_sourceVoice->DestroyVoice();
		m_sourceVoice = nullptr;
	}
}

void SoundXAudio2Controller::Update()
{
	// 再生中でなければ
	if (!m_running || !m_sourceVoice)
		return;

	if (m_stream && !m_loading)	
	{// ストリーミング再生
		XAUDIO2_VOICE_STATE state{};
		m_sourceVoice->GetState(&state);

		if (state.BuffersQueued <= 0)
		{// 再生が終了したら

			if (m_loop)
			{// ループ再生であれば
				this->SetPlayPosition(0);
				this->Play();
			}
			return;
		}


		if (WaitForSingleObject(m_voiceCallback.eventBufferEnd, 10) == WAIT_OBJECT_0)
		{// オーディオの読み込み
			this->LoadAudioData(m_bufferIndex);
			m_bufferIndex = (m_bufferIndex + 1) % k_BUFFER_COUNT;
		}
	}
}

void SoundXAudio2Controller::SetPlayPosition(unsigned int sec)
{
	const LONGLONG k_A_SECOND = 10000000;

	m_sourceVoice->Stop();
	m_sourceVoice->FlushSourceBuffers();
	m_dataBuffers.clear();

	PROPVARIANT varPosition;
	PropVariantInit(&varPosition);

	varPosition.vt = VT_I8;									// guidTimeFormatをGUID_NULLとするならば、VT_I8とする
	varPosition.hVal.QuadPart = k_A_SECOND * LONGLONG(sec);	// sec秒 

	HRESULT hr = m_sourceReader->SetCurrentPosition(GUID_NULL, varPosition);
	PropVariantClear(&varPosition);

	m_loading = true;
	for (int i = 0; i < k_BUFFER_COUNT; ++i)
		this->LoadAudioData(i);

	m_loading = false;
	m_bufferIndex = 0;
}

void SoundXAudio2Controller::Play()
{
	if (!m_stream)
	{
		m_sourceVoice->FlushSourceBuffers();
		XAUDIO2_BUFFER audioBuf{};
		audioBuf.AudioBytes = uint32_t(m_dataBuffers[0].size());
		audioBuf.pAudioData = m_dataBuffers[0].data();
		//audioBuf.Flags		= XAUDIO2_END_OF_STREAM;
		m_sourceVoice->SubmitSourceBuffer(&audioBuf);
	}


	m_running = true;			// 再生している状態に設定
	m_sourceVoice->Start();
}

void SoundXAudio2Controller::Stop()
{
	m_sourceVoice->Stop(XAUDIO2_PLAY_TAILS);
	m_running = false;			// 停止状態に設定
}

void SoundXAudio2Controller::SetVolume(unsigned int volume)
{
	// volumeを 0.0f ~ 1.0fまでの値に変換する
	float set = 0.01f * float(volume);

	m_sourceVoice->SetVolume(set);
}

bool SoundXAudio2Controller::LoadAudioData()
{
	// データの読み込み処理
	DWORD totalSize = 0;
	while (true)
	{
		HRESULT ret{};
		IMFSample* sample = nullptr;
		DWORD flags = 0;
		ret = m_sourceReader->ReadSample(
			MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &sample);
		if ((FAILED(ret) || !sample) || flags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			if (sample) sample->Release();
			break;
		}

		// ファイルデータの長さ取得(やり方が分からないので最後までループしてオーディオサイズを図る)
		BYTE* buffer;
		DWORD length = 0;
		IMFMediaBuffer* mediaBuffer = nullptr;
		std::vector<BYTE> work;
		ret = sample->ConvertToContiguousBuffer(&mediaBuffer);
		if (FAILED(ret) || !mediaBuffer)
			return false;

		mediaBuffer->Lock(&buffer, nullptr, &length);
		totalSize += length;

		work.resize(length);
		memcpy(work.data(), buffer, length);
		mediaBuffer->Unlock();

		mediaBuffer->Release();
		sample->Release();

		m_dataBuffers[0].resize(m_dataBuffers[0].size() + work.size());
		memcpy(m_dataBuffers[0].data() + (m_dataBuffers[0].size() - length), work.data(), length);
	}

	return true;
}

bool SoundXAudio2Controller::LoadAudioData(int index)
{
	HRESULT ret{};
	IMFSample*	sample = nullptr;
	DWORD       flags = 0;

	// データの読み込み処理
	ret = m_sourceReader->ReadSample(
		MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &sample);
	if ((FAILED(ret) || !sample) || (flags & MF_SOURCE_READERF_ENDOFSTREAM))
		return false;

	// オーディオデータの本体を取得する処理
	BYTE* buffer = nullptr;
	DWORD length = 0;
	IMFMediaBuffer* mediaBuffer = nullptr;
	ret = sample->ConvertToContiguousBuffer(&mediaBuffer);
	if (FAILED(ret) || !mediaBuffer)
		return false;

	mediaBuffer->Lock(&buffer, nullptr, &length);

	// 取得データをコピーする
	m_dataBuffers[index].clear();
	m_dataBuffers[index].resize(length);
	memcpy(m_dataBuffers[index].data(), buffer, length);

	mediaBuffer->Unlock();

	mediaBuffer->Release();
	sample->Release();


	// 不正な値でなければデータをキューに積む
	if (length > 0)
	{
		XAUDIO2_BUFFER audioBuf{};
		audioBuf.AudioBytes = uint32_t(m_dataBuffers[index].size());
		audioBuf.pAudioData = m_dataBuffers[index].data();
		m_sourceVoice->SubmitSourceBuffer(&audioBuf);
	}

	return true;
}
