/*===================================================================================
*    Date : 2022/01/30(��)
*        Author		: Gakuto.S
*        File		: Object_Fade.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Object_Sprite.h"

class IScene;

class ObjectFade : public ObjectSprite
{
public:
	enum class MODE
	{
		NONE,
		FADE_IN,
		FADE_OUT,
	};

public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	ObjectFade(const char* fileName);

public:
	//**************************************************
	/// \brief �X�V����
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Update() override;

	//**************************************************
	/// \brief �V�[���̃Z�b�g
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetNextScene(IScene* scene);

	//**************************************************
	/// \brief �t�F�[�h��Ԃ̎擾
	/// 
	/// \return true �Ȃ�t�F�[�h���Ă��Ȃ�
	//**************************************************
	bool GetFadeState();

private:
	IScene* m_nextScene;
	MODE	m_mode;
	float	k_ADD_VALUE;
};

