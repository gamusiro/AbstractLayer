/*===================================================================================
*    Date : 2023/01/15(��)
*        Author		: Gakuto.S
*        File		: Object_Cursor.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Object.h"

class ObjectTable;
class ISoundController;

class ObjectCursors : public IObject3D
{
public:
	enum class COMIT_TYPE
	{
		ERASE,
		PUTON,
		CHECK,
		NONE
	};


public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	ObjectCursors(ObjectTable* table)
		:m_table(table),
		m_colorIndex(0),
		m_rowIndex(0),
		m_colorCursor(),
		m_columnCursor(),
		m_degree(),
		m_currentlyColumn(),
		m_currentlyColumnFull(false),
		m_comitCursor(),
		m_cursorMoveSE(),
		m_comitSE()
	{
	}

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
	void Update(bool key = true);

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Draw() override;

public:
	//**************************************************
	/// \brief �J���[�C���f�b�N�X�̎擾����
	/// 
	/// \return �J���[�C���f�b�N�X
	//**************************************************
	int GetColorIndex() { return m_colorIndex; }

	//**************************************************
	/// \brief �s���z�[���C���f�b�N�X�̎擾����
	/// 
	/// \return �s���z�[���C���f�b�N�X
	//**************************************************
	int GetPinWholeIndex();

	//**************************************************
	/// \brief ��C���f�b�N�X�̎擾����
	/// 
	/// \return ��C���f�b�N�X
	//**************************************************
	int GetCurrentlyColumn() { return m_currentlyColumn; }

	//**************************************************
	/// \brief �s�C���f�b�N�X�̎擾����
	/// 
	/// \return �s�C���f�b�N�X
	//**************************************************
	int GetCurrentlyRow() { return m_rowIndex; }

	//**************************************************
	/// \brief �s�C���f�b�N�X�̐ݒ菈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetCurrentlyRow(int row, bool sound);

	//**************************************************
	/// \brief �s�C���f�b�N�X�̐ݒ菈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetColorIndex(int color, bool sound);

	//**************************************************
	/// \brief ��C���f�b�N�X�̐ݒ菈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetCurrentlyColumn(int column);

	//**************************************************
	/// \brief 4�s�����h�����Ă��邩�ݒ肷��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetCurrentlyColumnFull(bool full) { m_currentlyColumnFull = full; }

	//**************************************************
	/// \brief ����{�^���������ꂽ�Ƃ��̏���
	/// 
	/// \return true ����
	//**************************************************
	COMIT_TYPE GetComitButton(bool increment = true);

	//**************************************************
	/// \brief �J�[�\���̈ړ����삪�s��ꂽ
	/// 
	/// VK_LEFT	 : ��
	/// VK_RIGHT : �E
	/// VK_DOWN  : ��
	/// VK_UP    : ��
	/// 
	/// \return -1�͕s�� 
	//**************************************************
	int GetKeyDir() { return m_keyCode; }

private:
	IObject3D*			m_columnCursor;
	IObject3D*			m_colorCursor;
	IObject3D*			m_comitCursor;
	ObjectTable*		m_table;
	ISoundController*	m_cursorMoveSE;
	ISoundController*	m_comitSE;
	
	int			m_currentlyColumn;		// 0 ~ 8
	int			m_rowIndex;				// 0 ~ 5
	int			m_colorIndex;			// 0 ~ 6
	bool		m_currentlyColumnFull;
	int			m_keyCode;				// �I�����C���p�ɉ����ꂽ�����������f�[�^���i�[����ϐ�

	float		m_degree;

public:
	const DirectX::XMFLOAT3 k_SCALE{2.0f, 2.0f, 2.0f};
};

