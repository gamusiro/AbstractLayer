/*===================================================================================
*    Date : 2022/01/20(金)
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
    /// \brief コンストラクタ
    /// 
    /// \return なし
    //**************************************************
    SoundXAudio2();

    //**************************************************
    /// \brief デストラクタ
    /// 
    /// \return なし
    //**************************************************
    ~SoundXAudio2();

public:
    //**************************************************
    /// \brief 更新処理
    /// 
    /// \return なし
    //**************************************************
    void Update() override;

    //**************************************************
    /// \brief 生成処理
    /// 
    /// \param[in] fileName
    /// \param[in] stream
    /// 
    /// \return なし
    //**************************************************
    ISoundController* CreateSoundController(
        const char* fileName,
        bool stream = false
    ) override;

    //**************************************************
    /// \brief 解放処理
    /// 
    /// \param[in] controller
    /// 
    /// \return なし
    //**************************************************
    void DestroySoundController(ISoundController* controller);

    //**************************************************
    /// \brief 停止中のデータを再生する処理
    /// 
    /// \return なし
    //**************************************************
    void Play() override;

    //**************************************************
    /// \brief 再生中のデータを止める処理
    /// 
    /// \return なし
    //**************************************************
    void Stop() override;

    //**************************************************
   /// \brief マスターボリュームを取得
   /// 
   /// \return マスターボリューム
   //**************************************************
    int GetMasterVolume() { return m_masterVolume; }

    //**************************************************
   /// \brief BGMボリュームを取得
   /// 
   /// \return BGMーボリューム
   //**************************************************
    int GetBGMVolume() { return m_bgmVolume; }

    //**************************************************
   /// \brief マスターボリュームを取得
   /// 
   /// \return マスターボリューム
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

