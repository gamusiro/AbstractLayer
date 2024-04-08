/*===================================================================================
*    Date : 2022/01/21(�y)
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

    // �R���X�g���N�^
    VoiceCallback()
        :eventBufferEnd(CreateEvent(NULL, FALSE, FALSE, NULL))
    {}

    // �f�X�g���N�^
    ~VoiceCallback()
    {// �C�x���g�n���h���̔j��
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
    /// \brief �R���X�g���N�^
    /// 
    /// \return �Ȃ�
    //**************************************************
    SoundXAudio2Controller(
        bool streaming,
        size_t id,
        bool loop
    );

public:
    //**************************************************
    /// \brief �f�[�^�̓ǂݍ���
    /// 
    /// \return �Ȃ�
    //**************************************************
    bool Load(
        const char* fileName,
        IXAudio2* xaudio2
    );

    //**************************************************
    /// \brief �f�[�^�̉��
    /// 
    /// \return �Ȃ�
    //**************************************************
    void Unload();

    //**************************************************
    /// \brief �f�[�^�̍X�V
    /// 
    /// \return �Ȃ�
    //**************************************************
    void Update();

    //**************************************************
    /// \brief �Đ��J�n�b���ݒ�()
    /// 
    /// \return �Ȃ�
    //**************************************************
    void SetPlayPosition(unsigned int sec);

public:
    //**************************************************
    /// \brief �Đ�����
    /// 
    /// \return �Ȃ�
    //**************************************************
    void Play() override;

    //**************************************************
    /// \brief ��~����
    /// 
    /// \return �Ȃ�
    //**************************************************
    void Stop() override;

    //**************************************************
    /// \brief ���ʒ���
    /// 
    /// \param[in] volume 0 ~ 100
    /// 
    /// \return �Ȃ�
    //**************************************************
    void SetVolume(unsigned int volume) override;

private:
    //**************************************************
    /// \brief �f�[�^�̃��[�h
    /// 
    /// \return �Ȃ�
    //**************************************************
    bool LoadAudioData();

    //**************************************************
    /// \brief �f�[�^�̃��[�h
    /// 
    /// \return �Ȃ�
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

