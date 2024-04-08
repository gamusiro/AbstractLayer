/*===================================================================================
*    Date : 2022/01/20(金)
*        Author		: Gakuto.S
*        File		: Sound_XAudio2.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Sound.h"

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

private:
    IXAudio2*               m_xaudio2;
    IXAudio2MasteringVoice* m_masterVoice;
};

