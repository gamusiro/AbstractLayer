/*===================================================================================
*    Date : 2022/11/06(日)
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

// 入力
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
    /// \brief Applicationクラスのインスタンス取得処理
    /// 
    /// \return pointer
    //**************************************************
    static Application* Get();

    //**************************************************
    /// \brief メインループ
    /// 
    /// \return trueならアプリケーション実行中
    //**************************************************
    bool Run();

    //**************************************************
    /// \brief シーン切り替え
    /// 
    /// \return なし
    //**************************************************
    void SetScene(IScene* scene);

    //**************************************************
    /// \brief フェードシーン切り替え
    /// 
    /// \return なし
    //**************************************************
    void SetTransion(IScene* scene);

    //**************************************************
    /// \brief 初期化処理
    /// 
    /// \return true なら成功
    //**************************************************
    bool Init();
    
    //**************************************************
    /// \brief 終了処理
    /// 
    /// \return なし
    //**************************************************
    void Uninit();

    //**************************************************
    /// \brief Graphicsクラスのインスタンス取得
    /// 
    /// \return pointer
    //**************************************************
    IGraphics* Graphics() { return m_graphics; }

    //**************************************************
    /// \brief Soundクラスのインスタンス取得
    /// 
    /// \return pointer
    //**************************************************
    ISound* Sound() { return m_sound; }

    //**************************************************
    /// \brief 現在のシーンの取得
    /// 
    /// \return なし
    //**************************************************
    IScene* GetCurrentlyScene() { return m_currentScene; }

    //**************************************************
    /// \brief ホストの設定
    /// 
    /// \return なし
    //**************************************************
    void SetIsHost(bool host);

    //**************************************************
    /// \brief ホストかどうか調べる
    /// 
    /// \return なし
    //**************************************************
    bool IsHost() { return m_isHost; }

    //**************************************************
    /// \brief ホストの設定
    /// 
    /// \return なし
    //**************************************************
    void SetSoundUpdateOK(bool ok) { m_soundUpdateOK = ok; }

    //**************************************************
    /// \brief ホストかどうか調べる
    /// 
    /// \return なし
    //**************************************************
    bool GetSoundUpdateOK() { return m_soundUpdateOK; }

private:
    //**************************************************
    /// \brief 更新処理
    /// 
    /// \return なし
    //**************************************************
    void Update();
    
    //**************************************************
    /// \brief 描画処理
    /// 
    /// \return なし
    //**************************************************
    void Draw();

public:
    //**************************************************
    /// \brief 更新処理関数(マルチスレッドにするためにこのような定義にする)
    /// 
    /// \return なし
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

