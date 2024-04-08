/*===================================================================================
*    Date : 2022/01/20(��)
*        Author		: Gakuto.S
*        File		: Sound_XAudio2.h
*        Detail		:
===================================================================================*/
#pragma once
#include "../IF_Sound.h"

// include
#include <xaudio2.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>


// link library
#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")

class SoundXAudio2Controller;

class SoundXAudio2 : public ISound
{
public:
    //**************************************************
    /// \brief �R���X�g���N�^
    /// 
    /// \return �Ȃ�
    //**************************************************
    SoundXAudio2();

    //**************************************************
    /// \brief �f�X�g���N�^
    /// 
    /// \return �Ȃ�
    //**************************************************
    ~SoundXAudio2();

public:
    //**************************************************
    /// \brief �X�V����
    /// 
    /// \return �Ȃ�
    //**************************************************
    void Update() override;

    //**************************************************
    /// \brief ��������
    /// 
    /// \param[in] fileName
    /// \param[in] stream
    /// 
    /// \return �Ȃ�
    //**************************************************
    ISoundController* CreateSoundController(
        const char* fileName,
        bool stream = false
    ) override;

    //**************************************************
    /// \brief �������
    /// 
    /// \param[in] controller
    /// 
    /// \return �Ȃ�
    //**************************************************
    void DestroySoundController(ISoundController* controller);

    //**************************************************
    /// \brief ��~���̃f�[�^���Đ����鏈��
    /// 
    /// \return �Ȃ�
    //**************************************************
    void Play() override;

    //**************************************************
    /// \brief �Đ����̃f�[�^���~�߂鏈��
    /// 
    /// \return �Ȃ�
    //**************************************************
    void Stop() override;

    //**************************************************
   /// \brief �}�X�^�[�{�����[�����擾
   /// 
   /// \return �}�X�^�[�{�����[��
   //**************************************************
    int GetMasterVolume() { return m_masterVolume; }

    //**************************************************
   /// \brief BGM�{�����[�����擾
   /// 
   /// \return BGM�[�{�����[��
   //**************************************************
    int GetBGMVolume() { return m_bgmVolume; }

    //**************************************************
   /// \brief �}�X�^�[�{�����[�����擾
   /// 
   /// \return �}�X�^�[�{�����[��
   //**************************************************
    int GetSEVolume() { return m_seVolume; }

private:
    IXAudio2* m_xaudio2;
    IXAudio2MasteringVoice* m_masterVoice;

    int m_masterVolume;     // 0 ~ 100
    int m_bgmVolume;        // 0 ~ 100
    int m_seVolume;         // 0 ~ 100

private:
    std::vector<SoundXAudio2Controller*> m_backGround;
    std::vector<SoundXAudio2Controller*> m_effects;

    std::vector<size_t> m_runningBGMIndices;
    std::vector<size_t> m_runningSEIndices;
};

