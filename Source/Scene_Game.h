/*===================================================================================
*    Date : 2023/01/05(��)
*        Author		: Gakuto.S
*        File		: SceneGame.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Scene.h"

class ObjectTable;
class ObjectPin;
class ObjectPins;
class ObjectCursors;
class ISoundController;
class ObjectSprite;

class SceneGame : public IScene
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	SceneGame();

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~SceneGame();

public:
	//**************************************************
	/// \brief ����������
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void Init() override;

	//**************************************************
	/// \brief �I������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Uninit() override;

	//**************************************************
	/// \brief �X�V����
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void Update() override;

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Draw() override;

protected:
	//**************************************************
	/// \brief �Q�[���I�u�W�F�N�g�̏���������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void gameObjectInit();

	//**************************************************
	/// \brief �s���̐ݒ菈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void updatePinControl(int column);

	//**************************************************
	/// \brief �\������UI��ǉ�
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void drawUI() {};

protected:
	ObjectTable*		m_table;
	IObject3D*			m_plane;
	ObjectPins*			m_pins;
	ObjectCursors*		m_cursors;
	IObject3D*			m_comitButton;
	ObjectSprite*		m_hitSprite;
	ObjectSprite*		m_browSprite;
	ObjectSprite*		m_hitRefSprite;
	ObjectSprite*		m_browRefSprite;

	ISoundController*	m_gameBGM;

	unsigned int		m_frameCounter;
	bool				m_drawButton;
	bool				m_finish;

protected:
	int					m_hitCount;
	int					m_browCount;

private:
	bool				m_changeColumn;
};

