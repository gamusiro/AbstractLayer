/*===================================================================================
*    Date : 2022/01/30(ŒŽ)
*        Author		: Gakuto.S
*        File		: Object_Fade.h
*        Detail		:
===================================================================================*/
#include "Object_Fade.h"

#include "Application.h"

ObjectFade::ObjectFade(const char* fileName)
	:ObjectSprite(fileName),
	m_mode(MODE::FADE_IN),
	m_nextScene(nullptr),
	k_ADD_VALUE(1.0f / float(WindowWin::s_FRAME_COUNT))
{
}

void ObjectFade::Update()
{
	switch (m_mode)
	{
	case ObjectFade::MODE::FADE_IN:
		m_alpha -= k_ADD_VALUE;
		if (m_alpha < 0.0f)
		{
			m_alpha = 0.0f;
			m_mode = MODE::NONE;
		}
		break;
	case ObjectFade::MODE::FADE_OUT:
		m_alpha += k_ADD_VALUE;
		if (m_alpha > 1.0f)
		{
			m_alpha = 1.0f;
			m_mode = MODE::FADE_IN;

			Application::Get()->SetScene(m_nextScene);
			m_nextScene = nullptr;
		}
		break;
	default:
		break;
	}
}

void ObjectFade::SetNextScene(IScene* scene)
{
	m_mode = MODE::FADE_OUT;
	m_nextScene = scene;
}

bool ObjectFade::GetFadeState()
{
	if (m_mode == MODE::NONE)
		return true;

	return false;
}
