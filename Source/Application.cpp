/*===================================================================================
*    Date : 2022/11/06(日)
*        Author		: Gakuto.S
*        File		: Application.cpp
*        Detail		:
===================================================================================*/
#include "Photon/Photon_Base.h"
#include "Application.h"

// ウィンドウ
#include "Window_Win.h"

// グラフィックスクラス
#include "Vulkan/Vulkan_Graphics.h"
#include "Direct12/Direct12_Graphics.h"

// サウンドクラス
#include "XAudio2/Sound_XAudio2.h"

// シーン
#include "Summary_Objects.h"
#include "Summary_Scenes.h"

bool g_SoundThreadFlag{};

// サウンド用スレッドを作成
DWORD __stdcall Thread(LPVOID* data)
{
    // サウンドの更新処理
    while (g_SoundThreadFlag)
    {
        if(Application::Get()->GetSoundUpdateOK())
            Application::SoundUpdate();
    }

    ExitThread(0);
}


// ウィンドウプロシージャ
static LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HANDLE th;

    switch (msg)
    {
    case WM_CREATE: // ウィンドウ生成時にサウンド処理用スレッドを立てる
        g_SoundThreadFlag = true;
        th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread, (LPVOID)"ストリーミング再生", 0, NULL);
        break;

    case WM_DESTROY:    // ウィンドウの破棄時
        g_SoundThreadFlag = false;

        // 処理を終了するために true にする
        Application::Get()->SetSoundUpdateOK(true);
        while(1)
        {
             //スレッドが終わったかチェック
            DWORD result{};
             GetExitCodeThread(th, &result);

             //終わったらハンドルを閉じる。
             if(STILL_ACTIVE != result)
             {
                 CloseHandle(th);

                 //ループを抜ける。
                 break;
             }
        }

        PostQuitMessage(0);
        break;

    case WM_SIZE:       // 画面サイズの変更時
    {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        Application::Get()->Graphics()->Resize(width, height);
    }
    break;
    default:
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* インスタンスの取得 */
Application* Application::Get()
{
    static Application app{};
    return &app;
}

/* メインループ */
bool Application::Run()
{
    if (!m_window->Close())
    {
        Update();
        Draw();
        return true;
    }

    // ループ終了
    return false;
}

/* シーンの設定 */
void Application::SetScene(IScene* scene)
{
    if (m_currentScene)
    {
        m_currentScene->Uninit();
        delete m_currentScene;
    }

    if (scene)
    {
        m_currentScene = scene;
        m_currentScene->Init();
    }
}

void Application::SetTransion(IScene* scene)
{
    m_fade->SetNextScene(scene);
}


/* 初期化処理 */
bool Application::Init()
{
    // ウィンドウの生成
    WindowWin* windowsPlatform = new WindowWin(640, 480, L"HitAndBlow", WndProc);
    m_window = windowsPlatform;

    // 描画処理クラスの生成
    // m_graphics = new VulkanGraphics(640, 480, windowsPlatform);
    m_graphics = new Direct12Graphics(640, 480, windowsPlatform);

    // サウンド処理クラスの生成
    m_sound = new SoundXAudio2();

    // 入力制御
    GamePad::Init();

    // シーンの初期化
    this->SetScene(new SceneTitle());

    // フェードオブジェクトの初期化
    m_fade = new ObjectFade("Fade.png");
    m_fade->Init();
    m_fade->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f, IGraphics::k_SCREEN_UI_HEIGHT / 2.0f, 0.0f });   // 画面の中心
    m_fade->SetSize({ IGraphics::k_SCREEN_UI_WIDTH, IGraphics::k_SCREEN_UI_HEIGHT });                           // 画面サイズ

    m_window->ShowUp();

    m_isHost = false;
    return true;
}

/* 終了処理 */
void Application::Uninit()
{
    GamePad::Uninit();

    m_fade->Uninit();
    delete m_fade;

    delete m_currentScene;
    delete m_sound;
    delete m_graphics;
    delete m_window;
}

void Application::SetIsHost(bool host)
{
    if (m_isHost && !host)
        PhotonBase::getInstance().disconnect();
    
#ifdef _DEBUG
    if (!m_isHost && host)
        std::cout << "ホストになりました" << std::endl;

    if (m_isHost && !host)
        std::cout << "ホストではなくなりました" << std::endl;
#endif // _DEBUG

    m_isHost = host;
}

// 更新処理
void Application::Update()
{
    GamePad::Update();

    PhotonBase::getInstance().run();

    // フェード状態でなければ、シーンの更新
    if(m_fade->GetFadeState())
        m_currentScene->Update();

    // フェードの更新
    m_fade->Update();
}

// 描画処理
void Application::Draw()
{
    m_graphics->Clear();
    m_currentScene->Draw();
    m_fade->Draw();
    m_graphics->Present();
}

void Application::SoundUpdate()
{
    ISound* sound = Get()->m_sound;
    if (sound)
        sound->Update();
}
