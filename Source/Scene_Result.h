/*===================================================================================
*    Date : 2023/01/20(��)
*        Author		: Gakuto.S
*        File		: Object_Result.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Scene.h"

class ObjectTable;
class ObjectPin;
class ObjectPins;
class ISoundController;
class ObjectSprite;

class SceneResult : public IScene
{
private:
	enum class RESULT_STATE
	{
		NONE,
		WIN,
		LOSE
	};


public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	SceneResult(
		ObjectTable* table,
		ObjectPins* pins,
		IObject3D* plane,
		int finishColumn,
		int myTurn = -1,
		bool lobby = false
	);

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~SceneResult();

public:
	//**************************************************
	/// \brief ����������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Init() override;

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
	void Update() override;

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Draw() override;

private:
	//**************************************************
	/// \brief ���U���g�̉�������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void resultSound(int myTurn);

private:
	ObjectTable*		m_table;
	ObjectPins*			m_pins;
	IObject3D*			m_plane;
	ObjectSprite*		m_winSprite;
	ObjectSprite*		m_loseSprite;

	ISoundController*	m_correctSE;
	ISoundController*	m_successSE;
	ISoundController*	m_failedSE;
	int					m_finishColumn;
	int					m_rowIndex;
	int					m_correctCount;
	int					m_myTurn;
	unsigned int		m_frameCount;
	RESULT_STATE		m_result;

	const bool			k_Lobby;
	const unsigned int	k_UpdateFrame;
};

