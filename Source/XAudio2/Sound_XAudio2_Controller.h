/*===================================================================================
*    Date : 2022/01/21(土)
*        Author		: Gakuto.S
*        File		: Sound_XAudio2_Controller.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Sound_XAudio2.h"

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
    HANDLE eventBufferEnd;

    // コンストラクタ
    VoiceCallback()
        :eventBufferEnd(CreateEvent(NULL, FALSE, FALSE, NULL))
    {}

    // デストラクタ
    ~VoiceCallback()
    {// イベントハンドルの破棄
        CloseHandle(eventBufferEnd);
    }
    void STDMETHODCALLTYPE OnStreamEnd() {  }
    void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {}
    void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
    void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) { SetEvent(eventBufferEnd); }
    void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) {}
    void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) { }
    void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error) { }
};

class SoundXAudio2Controller : public ISoundController
{
public:
    //**************************************************
    /// \brief コンストラクタ
    /// 
    /// \return なし
    //**************************************************
    SoundXAudio2Controller(
        bool streaming,
        size_t id,
        bool loop
    );

public:
    //**************************************************
    /// \brief データの読み込み
    /// 
    /// \return なし
    //**************************************************
    bool Load(
        const char* fileName,
        IXAudio2* xaudio2
    );

    //**************************************************
    /// \brief データの解放
    /// 
    /// \return なし
    //**************************************************
    void Unload();

    //**************************************************
    /// \brief データの更新
    /// 
    /// \return なし
    //**************************************************
    void Update();

    //**************************************************
    /// \brief 再生開始秒数設定()
    /// 
    /// \return なし
    //**************************************************
    void SetPlayPosition(unsigned int sec);

public:
    //**************************************************
    /// \brief 再生処理
    /// 
    /// \return なし
    //**************************************************
    void Play() override;

    //**************************************************
    /// \brief 停止処理
    /// 
    /// \return なし
    //**************************************************
    void Stop() override;

    //**************************************************
    /// \brief 音量調整
    /// 
    /// \param[in] volume 0 ~ 100
    /// 
    /// \return なし
    //**************************************************
    void SetVolume(unsigned int volume) override;

private:
    //**************************************************
    /// \brief データのロード
    /// 
    /// \return なし
    //**************************************************
    bool LoadAudioData();

    //**************************************************
    /// \brief データのロード
    /// 
    /// \return なし
    //**************************************************
    bool LoadAudioData(int index);

private:
    IXAudio2SourceVoice*                    m_sourceVoice;
    IMFSourceReader*						m_sourceReader;
	IMFMediaType*							m_mediaType;
	VoiceCallback							m_voiceCallback;
	WAVEFORMATEX*							m_waveFormatEx;
	std::map<int, std::vector<BYTE>>	    m_dataBuffers;
    bool                                    m_loop;
    bool                                    m_loading;
    int                                     m_bufferIndex;
    const int								k_BUFFER_COUNT = 3;
};

