/*===================================================================================
*    Date : 2023/01/05(木)
*        Author		: Gakuto.S
*        File		: Scene_Title.cpp
*        Detail		:
===================================================================================*/
#include "Scene_Title.h"
#include "Photon/Photon_Base.h"

#include "Application.h"
#include "Summary_Objects.h"
#include "Summary_Scenes.h"
using namespace DirectX;
using namespace std;

SceneTitle::SceneTitle()
	:m_soloModeSprite(new ObjectSprite("Solo.png")),
	m_onlineModeSprite(new ObjectSprite("Battle.png")),
	m_titleSprite(new ObjectSprite("Title.png")),
	m_bgmController(nullptr),
	m_radian(),
	m_cursorIndex(0)
{
	Application::Get()->SetIsHost(false);
	PhotonBase::getInstance().disconnect();
}

SceneTitle::~SceneTitle()
{
	
}

void SceneTitle::Init()
{
	IGraphics* graphics = Application::Get()->Graphics();

	// カメラの設定
	graphics::CAMERA param{};
	param.position = DirectX::XMFLOAT3(12.0f, 3.0f, 2.0f);
	param.focus = DirectX::XMFLOAT3(8.0f, 0, 2.0f);
	param.up = DirectX::XMFLOAT3(0, 1, 0);
	graphics->SetCamera(param);

	// ソロモードスプライト
	m_soloModeSprite->Init();
	m_soloModeSprite->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f, IGraphics::k_SCREEN_UI_HEIGHT / 10.0f * 6.0f, 0.0f });
	m_soloModeSprite->SetSize(k_DEFAULT_SPRITE_SIZE);

	// オンラインモードスプライト
	m_onlineModeSprite->Init();
	m_onlineModeSprite->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f, IGraphics::k_SCREEN_UI_HEIGHT / 10.0f * 8.0f, 0.0f });
	m_onlineModeSprite->SetSize(k_DEFAULT_SPRITE_SIZE);

	// タイトルスプライト
	m_titleSprite->Init();
	m_titleSprite->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f, IGraphics::k_SCREEN_UI_HEIGHT / 10.0f * 2.5f, 0.0f });
	m_titleSprite->SetSize({ IGraphics::k_SCREEN_UI_WIDTH, IGraphics::k_SCREEN_UI_HEIGHT / 2.0f });

	// テーブルの作成
	ObjectTable* table = new ObjectTable();
	table->Init();
	m_objects.push_back(table);

	ObjectPlane* plane = new ObjectPlane("table.jpg");
	plane->Init();
	plane->SetPosition({ 0, -1.0f, 0 });
	plane->SetScale(k_PLANE_SCALE);
	m_objects.push_back(plane);

	vector<ObjectPin::COLOR> colors
	{
		ObjectPin::COLOR::BLUE,
		ObjectPin::COLOR::RED,
		ObjectPin::COLOR::GREEN,
		ObjectPin::COLOR::YELLOW
	};

	for (int i = 0; i < ObjectTable::k_ROW; ++i)
	{
		ObjectPin* pin = new ObjectPin(colors[i]);
		pin->Init();
		pin->SetPosition(table->GetPinWholePinSetPosition(ObjectTable::k_ROW * ObjectTable::k_COL + i));
		pin->SetScale(ObjectPins::k_PIN_SCALE);
		m_objects.push_back(pin);
	}

	// BGMの読み込み処理
	ISound* sound = Application::Get()->Sound();
	m_bgmController = sound->CreateSoundController("title.wav", true);
	m_bgmController->Play();

	Application::Get()->SetSoundUpdateOK(true);
}

void SceneTitle::Uninit()
{
	Application::Get()->SetSoundUpdateOK(false);

	ISound* sound = Application::Get()->Sound();
	sound->DestroySoundController(m_bgmController);

	for (auto& object : m_objects)
	{
		object->Uninit();
		delete object;
	}

	m_titleSprite->Uninit();
	delete m_titleSprite;

	m_onlineModeSprite->Uninit();
	delete m_onlineModeSprite;

	m_soloModeSprite->Uninit();
	delete m_soloModeSprite;
}

void SceneTitle::Update()
{
	// カーソルの移動
	if (GamePad::Button::Trigger(0, PadButton::DOWN)
		|| Keyboard::Trigger('S'))
	{
		m_cursorIndex++;
		m_cursorIndex %= k_SPRITE_NUM;
	}
	else if (GamePad::Button::Trigger(0, PadButton::DOWN)
		|| Keyboard::Trigger('W'))
	{
		m_cursorIndex--;
		if (m_cursorIndex < 0)
			m_cursorIndex = k_SPRITE_NUM - 1;
	}

	// 表示サイズの変更
	if (m_cursorIndex % k_SPRITE_NUM == 0)
	{
		m_soloModeSprite->SetSize(k_PREVIEW_SPRITE_SIZE);
		m_onlineModeSprite->SetSize(k_DEFAULT_SPRITE_SIZE);
	}
	else
	{
		m_soloModeSprite->SetSize(k_DEFAULT_SPRITE_SIZE);
		m_onlineModeSprite->SetSize(k_PREVIEW_SPRITE_SIZE);
	}
	
	// モードが選択されたら
	if (GamePad::Button::Trigger(0, PadButton::B)
		|| Keyboard::Trigger(VK_RETURN))
	{
		if (m_cursorIndex % k_SPRITE_NUM == 0)
		{// ソロモード
			Application::Get()->SetTransion(new SceneGame());
		}
		else
		{// バトルモード
			PhotonBase::getInstance().connect();
			Application::Get()->SetTransion(new SceneLobby(false));
		}
	}

	// ピンの回転
	m_radian += 0.01f;
	for (size_t i = 2; i < m_objects.size(); ++i)
		m_objects[i]->SetRotate({ 0.0f, m_radian, 0.0f });
}

void SceneTitle::Draw()
{
	for (auto& object : m_objects)
		object->Draw();

	m_soloModeSprite->Draw();
	m_onlineModeSprite->Draw();
	m_titleSprite->Draw();
}
