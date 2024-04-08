/*===================================================================================
*    Date : 2023/01/26(木)
*        Author		: Gakuto.S
*        File		: Scene_Lobby.cpp
*        Detail		:
===================================================================================*/
#include "Scene_Lobby.h"
#include "Photon/Photon_Base.h"

#include "Application.h"
#include "Summary_Objects.h"
#include "Summary_Scenes.h"
using namespace std;
using namespace DirectX;

SceneLobby::SceneLobby(bool join)
	:m_host(new ObjectSprite("Lobby.png", 2.0f)),
	m_client(new ObjectSprite("Lobby.png", 2.0f)),
	m_hostDummy(new ObjectSprite("Searching.png")),
	m_clientDummy(new ObjectSprite("Searching.png")),
	m_backTitle(new ObjectSprite("Back_title.png")),
	m_hostOption(new ObjectSprite("Game_start.png")),
	m_clientOption(new ObjectSprite("Wait_host_control.png")),
	m_joinHost(join),
	m_joinClient(join)
{
}

SceneLobby::~SceneLobby()
{
	delete m_clientOption;
	delete m_hostOption;
	delete m_backTitle;
	delete m_clientDummy;
	delete m_hostDummy;
	delete m_client;
	delete m_host;
}

void SceneLobby::Init()
{
	// ホストの画像
	m_host->Init();
	m_host->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 4.0f * 1.0f, IGraphics::k_SCREEN_UI_HEIGHT / 2.0f, 0.0f });
	m_host->SetSize({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f, IGraphics::k_SCREEN_UI_HEIGHT });
	m_host->SetUVStart({ 1.0f, 0.0f });

	// ホストじゃない人が入室したとき用
	m_client->Init();
	m_client->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 4.0f * 3.0f, IGraphics::k_SCREEN_UI_HEIGHT / 2.0f, 0.0f });
	m_client->SetSize({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f, IGraphics::k_SCREEN_UI_HEIGHT });
	m_client->SetUVStart({ 0.0f, 0.0f });

	// ダミー画像の初期化
	m_hostDummy->Init();
	m_hostDummy->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 4.0f * 1.0f, IGraphics::k_SCREEN_UI_HEIGHT / 2.0f, 0.0f });
	m_hostDummy->SetSize({ IGraphics::k_SCREEN_UI_WIDTH / 3.0f, IGraphics::k_SCREEN_UI_HEIGHT * 0.8f });

	m_clientDummy->Init();
	m_clientDummy->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 4.0f * 3.0f, IGraphics::k_SCREEN_UI_HEIGHT / 2.0f, 0.0f });
	m_clientDummy->SetSize({ IGraphics::k_SCREEN_UI_WIDTH / 3.0f, IGraphics::k_SCREEN_UI_HEIGHT * 0.8f });

	// タイトルへ戻るボタン
	m_backTitle->Init();
	m_backTitle->SetPosition({ 200.0f, IGraphics::k_SCREEN_UI_HEIGHT - 50.0f, 0.0f });
	m_backTitle->SetSize({ 400.0f, 100.0f});

	// 個別のモード
	m_hostOption->Init();
	m_hostOption->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f, IGraphics::k_SCREEN_UI_HEIGHT / 2.0f, 0.0f });
	m_hostOption->SetSize({ 512.0f, 64.0f });

	m_clientOption->Init();
	m_clientOption->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f, IGraphics::k_SCREEN_UI_HEIGHT / 2.0f, 0.0f });
	m_clientOption->SetSize({ 512.0f, 64.0f });
}

void SceneLobby::Uninit()
{
	m_clientOption->Uninit();
	m_hostOption->Uninit();
	m_backTitle->Uninit();
	m_clientDummy->Uninit();
	m_hostDummy->Uninit();
	m_client->Uninit();
	m_host->Uninit();
}

void SceneLobby::Update()
{
	// モードが選択されたら
	if (GamePad::Button::Trigger(0, PadButton::A)
		|| Keyboard::Trigger(VK_BACK))
	{
		PhotonBase::getInstance().disconnect();
		Application::Get()->SetTransion(new SceneTitle());
	}

	// ホストであればシーンを遷移できる
	if (Application::Get()->IsHost())
	{
		if (GamePad::Button::Trigger(0, PadButton::B)
			|| Keyboard::Trigger(VK_RETURN))
		{
			// 二人が入室していれば
			if (m_joinHost && m_joinClient)
			{
				// 解答を作成
				array<int, ObjectTable::k_ROW + 1> colors{};
				random_device seed_gen;
				mt19937 engine(seed_gen());

				// カラーを設定
				for (int i = 0; i < ObjectTable::k_ROW; ++i)
					colors[i] = engine() % ObjectPins::k_COLOR_NUM;

				// ターンを決める
				colors[4] = engine() % 2;

				// 解答を転送
				PhotonBase::getInstance().sendAnswer(colors);

				// シーンの切り替え
				Application::Get()->SetTransion(new SceneGameOnline(
					{ colors[0], colors[1], colors[2], colors[3] },	// 解答カラーの設定
					1 - colors[4])									// ターンの設定
				);
			}
		}
	}
}

void SceneLobby::Draw()
{
	if (m_joinHost)
		m_host->Draw();
	else
		m_hostDummy->Draw();	// ダミー画像の描画

	if (m_joinClient)
		m_client->Draw();
	else
		m_clientDummy->Draw();	// ダミー画像の描画

	m_backTitle->Draw();		// 戻るボタンの描画

	if (m_joinHost && m_joinClient)
	{
		if (Application::Get()->IsHost())
			m_hostOption->Draw();
		else
			m_clientOption->Draw();
	}
}

void SceneLobby::Join(bool host)
{
	if (!host)
	{// ホストでなければ
		m_joinClient = true;
		PhotonBase::getInstance().sendComingClient();
	}	
	
	m_joinHost = true;
}
