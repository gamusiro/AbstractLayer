/*===================================================================================
*    Date : 2023/01/26(��)
*        Author		: Gakuto.S
*        File		: Scene_Lobby.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Scene.h"

class ObjectSprite;

class SceneLobby : public IScene
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	SceneLobby(bool join);

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~SceneLobby();

public:
	//**************************************************
	/// \brief �X�V����
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Init() override;

	//**************************************************
	/// \brief �X�V����
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Uninit() override;

	//**************************************************
	/// \brief �X�V����
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Update() override;

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Draw() override;

public:
	//**************************************************
	/// \brief �N���C�A���g��������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Join(bool host);

	//**************************************************
	/// \brief �ǂ��炩�������𗣂ꂽ
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Leave()		{ m_joinClient = false;  m_joinHost = false; }

private:
	ObjectSprite*	m_host;
	ObjectSprite*	m_client;
	ObjectSprite*	m_hostDummy;
	ObjectSprite*	m_clientDummy;
	ObjectSprite*	m_hostOption;
	ObjectSprite*	m_clientOption;
	ObjectSprite*	m_backTitle;

	bool			m_joinHost;
	bool			m_joinClient;
};

