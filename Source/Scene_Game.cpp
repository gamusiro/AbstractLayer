/*===================================================================================
*    Date : 2023/01/05(��)
*        Author		: Gakuto.S
*        File		: SceneGame.cpp
*        Detail		:
===================================================================================*/
#include "Scene_Game.h"

#include "Application.h"
#include "Summary_Objects.h"
#include "Summary_Scenes.h"

SceneGame::SceneGame()
	:m_table(new ObjectTable()),
	m_plane(new ObjectPlane("table.jpg")),
	m_pins(new ObjectPins(m_table)),
	m_cursors(new ObjectCursors(m_table)),
	m_comitButton(new ObjectPlane("Comit.png")),
	m_hitSprite(new ObjectSprite("Hits.png", 1.0f, 5.0f)),
	m_browSprite(new ObjectSprite("Brow.png", 1.0f, 5.0f)),
	m_hitRefSprite(new ObjectSprite("HitReference.png")),
	m_browRefSprite(new ObjectSprite("BrowReference.png")),
	m_gameBGM(),
	m_drawButton(false),
	m_finish(false),
	m_hitCount(0),
	m_browCount(0),
	m_frameCounter(WindowWin::s_FRAME_COUNT * 2 + 1)
{
	//this->Init();
	
}

SceneGame::~SceneGame()
{
	//this->Uninit();
	
	delete m_browRefSprite;
	delete m_hitRefSprite;
	delete m_browSprite;
	delete m_hitSprite;
	delete m_comitButton;
	delete m_cursors;

	//m_cursors = nullptr;	// �ʐM���b�Z�[�W�𑗂�֐��Ŏg�����߁Anullptr�����Ă���
}

void SceneGame::Init()
{
	this->gameObjectInit();
}

void SceneGame::Uninit()
{
	Application::Get()->SetSoundUpdateOK(false);
	Application::Get()->Sound()->DestroySoundController(m_gameBGM);

	m_browRefSprite->Uninit();
	m_hitRefSprite->Uninit();
	m_browSprite->Uninit();
	m_hitSprite->Uninit();
	m_comitButton->Uninit();
	m_cursors->Uninit();
}

void SceneGame::Update()
{
	// �J�[�\���̍X�V
	int column = m_cursors->GetCurrentlyColumn();
	bool check = m_pins->GetPinWholeStateOfAColumn(column);

	m_cursors->SetCurrentlyColumnFull(check);
	m_cursors->Update();

	// �s���̐ݒ�
	updatePinControl(column);

	// ����ړ������ꍇ�͍s�̐擪�ɃJ�[�\����߂�
	if (m_changeColumn)
		m_cursors->SetCurrentlyRow(0, false);

	// ���̗�ֈړ��\�ȏꍇ�̓{�^����`�悷��
	m_drawButton = false;
	if (check)
	{
		m_drawButton = true;
		DirectX::XMFLOAT3 pos = m_table->GetComitButtonPosition(column);
		pos.y -= 0.01f;
		m_comitButton->SetPosition(pos);
	}

	// �V�[����؂�ւ���
	if (m_finish && m_frameCounter > WindowWin::s_FRAME_COUNT)
		Application::Get()->SetScene(new SceneResult(m_table, m_pins, m_plane, m_cursors->GetCurrentlyColumn()));
}

void SceneGame::Draw()
{
	// 3D�I�u�W�F�N�g�̕`��
	m_plane->Draw();
	m_table->Draw();
	m_pins->Draw();

	// �R�~�b�g�{�^���̕`��
	if (m_drawButton)
		m_comitButton->Draw();

	// �A���t�@�`�����l���̊֌W�㓧�ߏ������l�����āA���Ԃɕ`�悵�Ȃ���΂Ȃ�Ȃ�
	m_cursors->Draw();

	this->drawUI();

	m_hitRefSprite->Draw();
	m_browRefSprite->Draw();

	if (m_frameCounter < WindowWin::s_FRAME_COUNT * 2)
	{
		m_frameCounter++;
		m_hitSprite->Draw();
		m_browSprite->Draw();
	}
}

void SceneGame::gameObjectInit()
{
	IGraphics* graphics = Application::Get()->Graphics();

	// �J�����̐ݒ�
	graphics::CAMERA param{};
	param.position = DirectX::XMFLOAT3(0, 15.0f, -1.0f);
	param.focus = DirectX::XMFLOAT3(0, 0, 0);
	param.up = DirectX::XMFLOAT3(0, 1, 0);
	graphics->SetCamera(param);

	// �n��
	m_plane->Init();
	m_plane->SetPosition({ 0.0f, -1.0f, 0.0f });
	m_plane->SetScale({ 50.0f, 30.0f, 25.0f });

	// �Q�[���e�[�u��
	m_table->Init();
	m_table->SetPosition({ 0, 0, -2 });

	// �s���}�l�[�W���[
	m_pins->Init();

	// �J�[�\��
	m_cursors->Init();

	// �R�~�b�g�{�^��
	m_comitButton->Init();
	m_comitButton->SetScale({ 1.8f, 1.8f, 1.8f });

	// �q�b�gUI
	m_hitSprite->Init();
	m_hitSprite->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f - 100.0f, IGraphics::k_SCREEN_UI_HEIGHT / 3.0f, 0.0f });
	m_hitSprite->SetSize({ 200.0f, 100.0f });

	// �u���[UI
	m_browSprite->Init();
	m_browSprite->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f + 100.0f, IGraphics::k_SCREEN_UI_HEIGHT / 3.0f + 3.0f, 0.0f });
	m_browSprite->SetSize({ 200.0f, 100.0f });

	// �q�b�g����UI
	m_hitRefSprite->Init();
	m_hitRefSprite->SetSize({ 400.0f, 80.0f });
	m_hitRefSprite->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f - 250.0f, 50.0f, 0.0f });

	// �u���[����UI
	m_browRefSprite->Init();
	m_browRefSprite->SetSize({ 400.0f, 80.0f });
	m_browRefSprite->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f + 250.0f, 50.0f, 0.0f });

	// BGM�̓ǂݍ��ݏ���
	m_gameBGM = Application::Get()->Sound()->CreateSoundController("game.wav", true);
	m_gameBGM->Play();

	Application::Get()->SetSoundUpdateOK(true);
}

void SceneGame::updatePinControl(int column)
{
	// ���݂̃J�[�\�������擾����
	m_changeColumn = false;
	int getPinWholeIndex	= m_cursors->GetPinWholeIndex();
	int colorIndex			= m_cursors->GetColorIndex();

	// ����
	ObjectCursors::COMIT_TYPE type = m_cursors->GetComitButton();
	switch (type)
	{
	case ObjectCursors::COMIT_TYPE::ERASE:
		// �s�����폜����
		m_pins->UnRegisterPins(getPinWholeIndex);
		break;
	case ObjectCursors::COMIT_TYPE::PUTON:
		// �s����o�^����
		m_pins->RegisterPins(getPinWholeIndex, colorIndex);
		break;
	case ObjectCursors::COMIT_TYPE::CHECK:
	{
		// �q�b�g���u���[���`�F�b�N����
		m_hitCount	= 0;
		m_browCount = 0;
		m_finish = m_pins->CheckHitOrBrow(column, m_hitCount, m_browCount);

		// UV�ʒu�ƁA�����̕\���ʒu�𒲐�
		m_hitSprite->SetUVStart({ 0.0f, float(m_hitCount) });
		m_hitSprite->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f - 100.0f, IGraphics::k_SCREEN_UI_HEIGHT / 3.0f + (2.5f * float(m_hitCount)), 0.0f});

		m_browSprite->SetUVStart({ 0.0f, float(m_browCount) });
		m_browSprite->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f + 100.0f, IGraphics::k_SCREEN_UI_HEIGHT / 3.0f + 3.0f - (1.5f * float(m_browCount)), 0.0f });

		// ���̗�Ɉړ����邽�ߏ�ɃJ�[�\����߂�
		//m_cursors->SetCurrentlyRow(0);
		m_changeColumn = true;

		m_frameCounter = 0;
	}
		break;
	default:
		break;
	}
}
