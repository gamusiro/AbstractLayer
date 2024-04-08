/*===================================================================================
*    Date : 2023/01/15(��)
*        Author		: Gakuto.S
*        File		: Object_Cursors.h
*        Detail		:
===================================================================================*/
#include "Object_Cursors.h"

#include "Application.h"
#include "Summary_Objects.h"

using namespace DirectX;
using namespace std;

void ObjectCursors::Init()
{
	// �I�u�W�F�N�g�̏���������
	m_columnCursor = new ObjectPlane("Cursor.png");
	m_columnCursor->Init();

	m_colorCursor = new ObjectPlane("Cursor.png");
	m_colorCursor->Init();
	m_colorCursor->SetRotate({0, XMConvertToRadians(90.0f), 0});

	m_comitCursor = new ObjectPlane("ComitCursor.png");
	m_comitCursor->Init();

	// �T�E���h�̓ǂݍ���
	ISound* sound = Application::Get()->Sound();
	m_cursorMoveSE = sound->CreateSoundController("Cursor.wav");
	m_comitSE = sound->CreateSoundController("Comit.wav");
}

void ObjectCursors::Uninit()
{
	// �T�E���h�̉��
	ISound* sound = Application::Get()->Sound();
	sound->DestroySoundController(m_comitSE);
	sound->DestroySoundController(m_cursorMoveSE);

	// �I�u�W�F�N�g�̉������
	m_comitCursor->Uninit();
	delete m_comitCursor;

	m_colorCursor->Uninit();
	delete m_colorCursor;

	m_columnCursor->Uninit();
	delete m_columnCursor;
}

void ObjectCursors::Update(bool key)
{
	// �J�[�\�����k��������
	++m_degree;
	float add = std::fabsf(cosf(XMConvertToRadians(m_degree))) * 0.5f;
	XMFLOAT3 updateScale = k_SCALE;
	updateScale.x += add;
	updateScale.z += add;

	int mx = ObjectTable::k_ROW;
	if (m_currentlyColumnFull)
		mx++;

	m_colorCursor->SetScale(updateScale);
	m_columnCursor->SetScale(updateScale);
	m_comitCursor->SetScale(updateScale);

	// �{�^���������ꂽ�����ׂ�t���O
	m_keyCode = -1;

	if (key)
	{
		if (GamePad::Button::Trigger(0, PadButton::LEFT) || GamePad::Stick::Left::Trigger(0, PadStick::LEFT)
			|| Keyboard::Trigger(VK_LEFT) || Keyboard::Trigger('A'))
		{// �J���[�I��
			m_colorIndex--;
			m_keyCode = VK_LEFT;

			if (m_colorIndex < 0)
				m_colorIndex = ObjectTable::k_COLOR_NUM - 1;
		}

		if (GamePad::Button::Trigger(0, PadButton::RIGHT) || GamePad::Stick::Left::Trigger(0, PadStick::RIGHT)
			|| Keyboard::Trigger(VK_RIGHT) || Keyboard::Trigger('D'))
		{// �J���[�I��
			m_colorIndex++;
			m_colorIndex %= ObjectTable::k_COLOR_NUM;

			m_keyCode = VK_RIGHT;
		}

		if (GamePad::Button::Trigger(0, PadButton::UP) || GamePad::Stick::Left::Trigger(0, PadStick::UP)
			|| Keyboard::Trigger(VK_UP) || Keyboard::Trigger('W'))
		{// �s���z�[���p�J�[�\���I��
			m_rowIndex--;
			m_keyCode = VK_UP;

			if (m_rowIndex < 0)
				m_rowIndex = mx - 1;
		}

		if (GamePad::Button::Trigger(0, PadButton::DOWN) || GamePad::Stick::Left::Trigger(0, PadStick::DOWN)
			|| Keyboard::Trigger(VK_DOWN) || Keyboard::Trigger('S'))
		{// �s���z�[���p�J�[�\���I��
			m_rowIndex++;
			m_rowIndex %= mx;

			m_keyCode = VK_DOWN;
		}
	}

	// �J���[�J�[�\���̃|�W�V�����ݒ�
	XMFLOAT3 pos = m_table->GetColorPinWholePinSetPosition(m_colorIndex);
	pos.y -= 0.01f;						// �s���z�[���Əd�Ȃ��Ă��܂��̂�y������������
	m_colorCursor->SetPosition(pos);

	// �s���z�[���p�J�[�\���̃|�W�V�����ݒ�
	pos = m_table->GetPinWholePinSetPosition(m_currentlyColumn * ObjectTable::k_ROW + m_rowIndex);
	m_columnCursor->SetPosition(pos);

	// �R�~�b�g�p�J�[�\���|�W�V�����̐ݒ�
	pos = m_table->GetComitButtonPosition(m_currentlyColumn);
	m_comitCursor->SetPosition(pos);

	// �ړ������Ƃ��p�̉���炷
	if (m_keyCode != -1)
	{
		m_cursorMoveSE->Stop();
		m_cursorMoveSE->Play();
	}
}

void ObjectCursors::Draw()
{
	if (m_currentlyColumnFull && m_rowIndex >= ObjectTable::k_ROW)
	{
		m_comitCursor->Draw();
	}
	else
	{
		m_columnCursor->Draw();
		m_colorCursor->Draw();
	}
}

int ObjectCursors::GetPinWholeIndex()
{
	return ObjectTable::k_ROW * m_currentlyColumn + m_rowIndex;
}

void ObjectCursors::SetCurrentlyRow(int row, bool sound)
{
	m_rowIndex = row;

	if (sound)
	{// ����炷
		m_cursorMoveSE->Stop();
		m_cursorMoveSE->Play();
	}
}

void ObjectCursors::SetColorIndex(int color, bool sound)
{
	m_colorIndex = color;

	if (sound)
	{// ����炷
		m_cursorMoveSE->Stop();
		m_cursorMoveSE->Play();
	}
}

void ObjectCursors::SetCurrentlyColumn(int column)
{
	m_currentlyColumn = column;

	m_comitSE->Stop();
	m_comitSE->Play();
}

ObjectCursors::COMIT_TYPE ObjectCursors::GetComitButton(bool increment)
{
	COMIT_TYPE ret = COMIT_TYPE::NONE;

	// ����
	if (GamePad::Button::Trigger(0, PadButton::B) 
		|| Keyboard::Trigger(VK_RETURN))
	{
		// �s����4�h�����Ă��āA�s���w���Ă��Ȃ����l�������玟�̗��
		if (m_currentlyColumnFull && m_rowIndex >= ObjectTable::k_ROW)
		{
			if(increment)
				m_currentlyColumn++;
			//m_rowIndex = 0;
			ret = COMIT_TYPE::CHECK;

			m_comitSE->Stop();
			m_comitSE->Play();
		}
		else
		{
			if (increment)
			{
				m_rowIndex++;
				m_rowIndex %= ObjectTable::k_ROW;
			}
			ret = COMIT_TYPE::PUTON;
		}
	}
	// �폜
	else if (GamePad::Button::Trigger(0, PadButton::A) 
		|| Keyboard::Trigger(VK_BACK))
	{
		ret = COMIT_TYPE::ERASE;
	}

	return ret;
}
