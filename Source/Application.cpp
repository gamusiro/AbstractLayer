/*===================================================================================
*    Date : 2022/11/06(��)
*        Author		: Gakuto.S
*        File		: Application.cpp
*        Detail		:
===================================================================================*/
#include "Photon/Photon_Base.h"
#include "Application.h"

// �E�B���h�E
#include "Window_Win.h"

// �O���t�B�b�N�X�N���X
#include "Vulkan/Vulkan_Graphics.h"
#include "Direct12/Direct12_Graphics.h"

// �T�E���h�N���X
#include "XAudio2/Sound_XAudio2.h"

// �V�[��
#include "Summary_Objects.h"
#include "Summary_Scenes.h"

bool g_SoundThreadFlag{};

// �T�E���h�p�X���b�h���쐬
DWORD __stdcall Thread(LPVOID* data)
{
    // �T�E���h�̍X�V����
    while (g_SoundThreadFlag)
    {
        if(Application::Get()->GetSoundUpdateOK())
            Application::SoundUpdate();
    }

    ExitThread(0);
}


// �E�B���h�E�v���V�[�W��
static LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HANDLE th;

    switch (msg)
    {
    case WM_CREATE: // �E�B���h�E�������ɃT�E���h�����p�X���b�h�𗧂Ă�
        g_SoundThreadFlag = true;
        th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread, (LPVOID)"�X�g���[�~���O�Đ�", 0, NULL);
        break;

    case WM_DESTROY:    // �E�B���h�E�̔j����
        g_SoundThreadFlag = false;

        // �������I�����邽�߂� true �ɂ���
        Application::Get()->SetSoundUpdateOK(true);
        while(1)
        {
             //�X���b�h���I��������`�F�b�N
            DWORD result{};
             GetExitCodeThread(th, &result);

             //�I�������n���h�������B
             if(STILL_ACTIVE != result)
             {
                 CloseHandle(th);

                 //���[�v�𔲂���B
                 break;
             }
        }

        PostQuitMessage(0);
        break;

    case WM_SIZE:       // ��ʃT�C�Y�̕ύX��
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

/* �C���X�^���X�̎擾 */
Application* Application::Get()
{
    static Application app{};
    return &app;
}

/* ���C�����[�v */
bool Application::Run()
{
    if (!m_window->Close())
    {
        Update();
        Draw();
        return true;
    }

    // ���[�v�I��
    return false;
}

/* �V�[���̐ݒ� */
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


/* ���������� */
bool Application::Init()
{
    // �E�B���h�E�̐���
    WindowWin* windowsPlatform = new WindowWin(640, 480, L"HitAndBlow", WndProc);
    m_window = windowsPlatform;

    // �`�揈���N���X�̐���
    // m_graphics = new VulkanGraphics(640, 480, windowsPlatform);
    m_graphics = new Direct12Graphics(640, 480, windowsPlatform);

    // �T�E���h�����N���X�̐���
    m_sound = new SoundXAudio2();

    // ���͐���
    GamePad::Init();

    // �V�[���̏�����
    this->SetScene(new SceneTitle());

    // �t�F�[�h�I�u�W�F�N�g�̏�����
    m_fade = new ObjectFade("Fade.png");
    m_fade->Init();
    m_fade->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f, IGraphics::k_SCREEN_UI_HEIGHT / 2.0f, 0.0f });   // ��ʂ̒��S
    m_fade->SetSize({ IGraphics::k_SCREEN_UI_WIDTH, IGraphics::k_SCREEN_UI_HEIGHT });                           // ��ʃT�C�Y

    m_window->ShowUp();

    m_isHost = false;
    return true;
}

/* �I������ */
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
        std::cout << "�z�X�g�ɂȂ�܂���" << std::endl;

    if (m_isHost && !host)
        std::cout << "�z�X�g�ł͂Ȃ��Ȃ�܂���" << std::endl;
#endif // _DEBUG

    m_isHost = host;
}

// �X�V����
void Application::Update()
{
    GamePad::Update();

    PhotonBase::getInstance().run();

    // �t�F�[�h��ԂłȂ���΁A�V�[���̍X�V
    if(m_fade->GetFadeState())
        m_currentScene->Update();

    // �t�F�[�h�̍X�V
    m_fade->Update();
}

// �`�揈��
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
