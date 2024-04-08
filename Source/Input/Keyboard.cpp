/*===================================================================================
*    Date : 2023/01/18(ìy)
*        Author		: Gakuto.S
*        File		: Keyboard.cpp
*        Detail		:
===================================================================================*/
#include "Keyboard.h"

// ÉLÅ[èÛë‘äiî[óp
BYTE Keyboard::m_curretnState[Keyboard::k_KEY_NUM];
BYTE Keyboard::m_oldState[Keyboard::k_KEY_NUM];



void Keyboard::SetKeyState(BYTE* state)
{
	memcpy(m_oldState, m_curretnState, k_KEY_NUM);
	memcpy(m_curretnState, state, k_KEY_NUM);
}

bool Keyboard::Press(int key)
{
	return m_curretnState[key] & 0x80;
}

bool Keyboard::Trigger(int key)
{
	if (m_curretnState[key] & 0x80)
	{
		if (!(m_oldState[key] & 0x80))
			return true;
	}

	return false;
}

bool Keyboard::Release(int key)
{
	if (!(m_curretnState[key] & 0x80))
	{
		if (m_oldState[key] & 0x80)
			return true;
	}

	return false;
}
