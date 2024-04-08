/*
*	Date : 2022/05/05(��)
*		Author : ���ނ��邧
*		Update : ----/--/--()
*/
#include "GamePad.h"


GamePad::STATE GamePad::m_State[GAMEPAD_MAX]{};

// ����������
bool GamePad::Init()
{
	XInputEnable(true);
	return true;
}

// �I������
void GamePad::Uninit()
{
	for (DWORD i = 0; i < GAMEPAD_MAX; i++)
	{// �o�C�u���[�V�����̒�~
		m_State[i].vibration = { 0, 0 };
		XInputSetState(i, &m_State[i].vibration);
	}

	XInputEnable(false);
}

// �X�V����
void GamePad::Update()
{
	for (DWORD i = 0; i < GAMEPAD_MAX; ++i)
	{
		// �o�C�u���[�V�����̐ݒ�
		XInputSetState(i, &m_State[i].vibration);

		// �p�b�h���̍X�V����
		DWORD ret = 0;
		m_State[i].last = m_State[i].current;
		ret = XInputGetState(i, &m_State[i].current);
	}
}

// �{�^����������Ă���
bool GamePad::Button::Press(int padNum, DWORD type)
{
	if (m_State[padNum].current.Gamepad.wButtons & type)
		return true;
	return false;
}

// �{�^���������ꂽ
bool GamePad::Button::Trigger(int padNum, DWORD type)
{
	if (m_State[padNum].current.Gamepad.wButtons & type)
		if (!(m_State[padNum].last.Gamepad.wButtons & type))
			return true;
	return false;
}

// �{�^���������ꂽ
bool GamePad::Button::Release(int padNum, DWORD type)
{
	if (m_State[padNum].last.Gamepad.wButtons & type)
		if (!(m_State[padNum].current.Gamepad.wButtons & type))
			return true;
	return false;
}

// ���X�e�B�b�N�̓��͒l
std::array<float, 2> GamePad::Stick::Left::GetVal(int padNum)
{
	return Stick::GetStick(m_State[padNum].current.Gamepad.sThumbLX, m_State[padNum].current.Gamepad.sThumbLY);
}
bool GamePad::Stick::Left::Press(int padNum, int dir)
{
	if (dir == PadStick::UP || dir == PadStick::DOWN)
		return Stick::Press(m_State[padNum].current.Gamepad.sThumbLY, dir);
	if (dir == PadStick::RIGHT || dir == PadStick::LEFT)
		return Stick::Press(m_State[padNum].current.Gamepad.sThumbLX, dir);
	return false;
}
bool GamePad::Stick::Left::Trigger(int padNum, int dir)
{
	if (dir == PadStick::UP || dir == PadStick::DOWN)
		return Stick::Trigger(m_State[padNum].current.Gamepad.sThumbLY, m_State[padNum].last.Gamepad.sThumbLY, dir);
	if (dir == PadStick::RIGHT || dir == PadStick::LEFT)
		return Stick::Trigger(m_State[padNum].current.Gamepad.sThumbLX, m_State[padNum].last.Gamepad.sThumbLX, dir);
	return false;
}
bool GamePad::Stick::Left::Release(int padNum, int dir)
{
	if (dir == PadStick::UP || dir == PadStick::DOWN)
		return Stick::Release(m_State[padNum].current.Gamepad.sThumbLY, m_State[padNum].last.Gamepad.sThumbLY, dir);
	if (dir == PadStick::RIGHT || dir == PadStick::LEFT)
		return Stick::Release(m_State[padNum].current.Gamepad.sThumbLX, m_State[padNum].last.Gamepad.sThumbLX, dir);
	return false;
}

// �E�X�e�B�b�N�̓��͒l
std::array<float, 2> GamePad::Stick::Right::GetVal(int padNum)
{
	return Stick::GetStick(m_State[padNum].current.Gamepad.sThumbRX, m_State[padNum].current.Gamepad.sThumbRY);
}
bool GamePad::Stick::Right::Press(int padNum, int dir)
{
	if (dir == PadStick::UP || dir == PadStick::DOWN)
		return Stick::Press(m_State[padNum].current.Gamepad.sThumbRY, dir);
	if (dir == PadStick::RIGHT || dir == PadStick::LEFT)
		return Stick::Press(m_State[padNum].current.Gamepad.sThumbRX, dir);
	return false;
}
bool GamePad::Stick::Right::Trigger(int padNum, int dir)
{
	if (dir == PadStick::UP || dir == PadStick::DOWN)
		return Stick::Trigger(m_State[padNum].current.Gamepad.sThumbRY, m_State[padNum].last.Gamepad.sThumbRY, dir);
	if (dir == PadStick::RIGHT || dir == PadStick::LEFT)
		return Stick::Trigger(m_State[padNum].current.Gamepad.sThumbRX, m_State[padNum].last.Gamepad.sThumbRX, dir);
	return false;
}
bool GamePad::Stick::Right::Release(int padNum, int dir)
{
	if (dir == PadStick::UP || dir == PadStick::DOWN)
		return Stick::Release(m_State[padNum].current.Gamepad.sThumbRY, m_State[padNum].last.Gamepad.sThumbRY, dir);
	if (dir == PadStick::RIGHT || dir == PadStick::LEFT)
		return Stick::Release(m_State[padNum].current.Gamepad.sThumbRX, m_State[padNum].last.Gamepad.sThumbRX, dir);
	return false;
}

// ���g���K�[�̓��͒l
float GamePad::PadTrigger::Left::GetVal(int padNum)
{
	return PadTrigger::GetVal(m_State[padNum].current.Gamepad.bLeftTrigger);
}
bool GamePad::PadTrigger::Left::Press(int padNum)
{
	return PadTrigger::Press(m_State[padNum].current.Gamepad.bLeftTrigger);
}
bool GamePad::PadTrigger::Left::Trigger(int padNum)
{
	return PadTrigger::Trigger(m_State[padNum].current.Gamepad.bLeftTrigger, m_State[padNum].last.Gamepad.bLeftTrigger);
}
bool GamePad::PadTrigger::Left::Release(int padNum)
{
	return PadTrigger::Release(m_State[padNum].current.Gamepad.bLeftTrigger, m_State[padNum].last.Gamepad.bLeftTrigger);
}

// �E�g���K�[�̓��͒l
float GamePad::PadTrigger::Right::GetVal(int padNum)
{
	return PadTrigger::GetVal(m_State[padNum].current.Gamepad.bRightTrigger);
}
bool GamePad::PadTrigger::Right::Press(int padNum)
{
	return PadTrigger::Press(m_State[padNum].current.Gamepad.bRightTrigger);
}
bool GamePad::PadTrigger::Right::Trigger(int padNum)
{
	return PadTrigger::Trigger(m_State[padNum].current.Gamepad.bRightTrigger, m_State[padNum].last.Gamepad.bRightTrigger);
}
bool GamePad::PadTrigger::Right::Release(int padNum)
{
	return PadTrigger::Release(m_State[padNum].current.Gamepad.bRightTrigger, m_State[padNum].last.Gamepad.bRightTrigger);
}

/****************�����֐�******************/
// �X�e�B�b�N���̎擾
std::array<float, 2> GamePad::Stick::GetStick(const SHORT& stick_x, const SHORT& stick_y)
{
	std::array<float, 2> ret = { float(stick_x / 0x7FFF), float(stick_y / 0x7FFF) };

	for (int i = 0; i < 2; ++i)
	{
		ret[i] = std::max(ret[i], -1.0f);
		ret[i] = std::min(ret[i], 1.0f);
	}

	ret[1] *= -1;
	return ret;
}
bool GamePad::Stick::Press(const SHORT& current, int dir)
{
	switch (dir)
	{
	case 0:	// UP	
	case 3:	// RIGHT
		if (current > DEAD_ZONE)	return true;	break;
	case 1:	// DOWN
	case 2:	// LEFT
		if (current < -DEAD_ZONE)	return true;	break;
	}
	return false;
}
bool GamePad::Stick::Trigger(const SHORT& current, const SHORT& old, int dir)
{
	switch (dir)
	{
	case 0:	// UP	
	case 3:	// RIGHT
		if (current > DEAD_ZONE)
			if (old < DEAD_ZONE)
			return true;	
		break;
	case 1:	// DOWN
	case 2:	// LEFT
		if (current < -DEAD_ZONE)	
			if(old > -DEAD_ZONE)
				return true;	
		break;
	}
	return false;
}
bool GamePad::Stick::Release(const SHORT& current, const SHORT& old, int dir)
{
	switch (dir)
	{
	case 0:	// UP	
	case 3:	// RIGHT
		if (current < DEAD_ZONE)
			if (old > DEAD_ZONE)
				return true;
		break;
	case 1:	// DOWN
	case 2:	// LEFT
		if (current > -DEAD_ZONE)
			if (old < -DEAD_ZONE)
				return true;
		break;
	}
	return false;
}

// �o�C�u���[�V�����̐ݒ�
void GamePad::Vibration::Set(int padNum, WORD valL, WORD valR)
{
	valL = std::max(valL, (WORD)0);
	valL = std::min(valL, (WORD)65535);

	valR = std::max(valR, (WORD)0);
	valR = std::min(valR, (WORD)65535);

	m_State[padNum].vibration.wLeftMotorSpeed	= valL;
	m_State[padNum].vibration.wRightMotorSpeed	= valR;
}

// �g���K�[���̎擾
float GamePad::PadTrigger::GetVal(const BYTE& current)
{
	return current / 255.0f;
}
bool GamePad::PadTrigger::Press(const BYTE& current)
{
	if (GetVal(current) > 0.5f)
		return true;
	return false;
}
bool GamePad::PadTrigger::Trigger(const BYTE& current, const BYTE& old)
{
	if (GetVal(current) > 0.5f)
		if (GetVal(old) < 0.4f)
			return true;
	return false;
}
bool GamePad::PadTrigger::Release(const BYTE& current, const BYTE& old)
{
	if (GetVal(old) > 0.5f)
		if (GetVal(current) < 0.4f)
			return true;
	return false;
}
