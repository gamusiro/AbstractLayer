/*===================================================================================
*    Date : 2022/11/06(��)
*        Author		: Gakuto.S
*        File		: Application.h
*        Detail		: 
===================================================================================*/
#pragma once
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include "IF_Graphics.h"
#include "IF_Sound.h"
#include "Window_Win.h"

// ����
#include "Input/GamePad.h"

class IScene;
class ObjectFade;
class IWindow;

class Application
{
public:
    Application(const Application&)             = delete;
    Application& operator=(const Application&)  = delete;
    Application(Application&&)                  = delete;
    Application& operator=(Application&&)       = delete;

private:
    Application()   = default;
    ~Application()  = default;

public:
    //**************************************************
    /// \brief Application�N���X�̃C���X�^���X�擾����
    /// 
    /// \return pointer
    //**************************************************
    static Application* Get();

    //**************************************************
    /// \brief ���C�����[�v
    /// 
    /// \return true�Ȃ�A�v���P�[�V�������s��
    //**************************************************
    bool Run();

    //**************************************************
    /// \brief �V�[���؂�ւ�
    /// 
    /// \return �Ȃ�
    //**************************************************
    void SetScene(IScene* scene);

    //**************************************************
    /// \brief �t�F�[�h�V�[���؂�ւ�
    /// 
    /// \return �Ȃ�
    //**************************************************
    void SetTransion(IScene* scene);

    //**************************************************
    /// \brief ����������
    /// 
    /// \return true �Ȃ琬��
    //**************************************************
    bool Init();
    
    //**************************************************
    /// \brief �I������
    /// 
    /// \return �Ȃ�
    //**************************************************
    void Uninit();

    //**************************************************
    /// \brief Graphics�N���X�̃C���X�^���X�擾
    /// 
    /// \return pointer
    //**************************************************
    IGraphics* Graphics() { return m_graphics; }

    //**************************************************
    /// \brief Sound�N���X�̃C���X�^���X�擾
    /// 
    /// \return pointer
    //**************************************************
    ISound* Sound() { return m_sound; }

    //**************************************************
    /// \brief ���݂̃V�[���̎擾
    /// 
    /// \return �Ȃ�
    //**************************************************
    IScene* GetCurrentlyScene() { return m_currentScene; }

    //**************************************************
    /// \brief �z�X�g�̐ݒ�
    /// 
    /// \return �Ȃ�
    //**************************************************
    void SetIsHost(bool host);

    //**************************************************
    /// \brief �z�X�g���ǂ������ׂ�
    /// 
    /// \return �Ȃ�
    //**************************************************
    bool IsHost() { return m_isHost; }

    //**************************************************
    /// \brief �z�X�g�̐ݒ�
    /// 
    /// \return �Ȃ�
    //**************************************************
    void SetSoundUpdateOK(bool ok) { m_soundUpdateOK = ok; }

    //**************************************************
    /// \brief �z�X�g���ǂ������ׂ�
    /// 
    /// \return �Ȃ�
    //**************************************************
    bool GetSoundUpdateOK() { return m_soundUpdateOK; }

private:
    //**************************************************
    /// \brief �X�V����
    /// 
    /// \return �Ȃ�
    //**************************************************
    void Update();
    
    //**************************************************
    /// \brief �`�揈��
    /// 
    /// \return �Ȃ�
    //**************************************************
    void Draw();

public:
    //**************************************************
    /// \brief �X�V�����֐�(�}���`�X���b�h�ɂ��邽�߂ɂ��̂悤�Ȓ�`�ɂ���)
    /// 
    /// \return �Ȃ�
    //**************************************************
    static void SoundUpdate();

private:
    IWindow*    m_window;
    IGraphics*  m_graphics;
    ISound*     m_sound;
    IScene*     m_currentScene;
    ObjectFade* m_fade;
    bool        m_isHost;
    bool        m_soundUpdateOK;
};

