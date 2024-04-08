/*===================================================================================
*    Date : 2022/01/20(��)
*        Author		: Gakuto.S
*        File		: IF_Sound.h
*        Detail		:
===================================================================================*/
#pragma once
#include <map>
#include <vector>

#include "IF_SoundController.h"

class ISound
{
public:
    //**************************************************
    /// \brief �X�V����
    /// 
    /// \return �Ȃ�
    //**************************************************
	virtual void Update() = 0;

    //**************************************************
    /// \brief ��������
    /// 
    /// \return �Ȃ�
    //**************************************************
    virtual ISoundController* CreateSoundController(
        const char* fileName,
        bool stream = false
    ) = 0;

    //**************************************************
    /// \brief �������
    /// 
    /// \return �Ȃ�
    //**************************************************
    virtual void DestroySoundController(ISoundController* controller) = 0;

    //**************************************************
    /// \brief ��~���̃f�[�^���Đ����鏈��
    /// 
    /// \return �Ȃ�
    //**************************************************
    virtual void Play() = 0;

    //**************************************************
    /// \brief �Đ����̃f�[�^���~�߂鏈��
    /// 
    /// \return �Ȃ�
    //**************************************************
    virtual void Stop() = 0;
};
