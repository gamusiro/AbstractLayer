/*===================================================================================
*    Date : 2023/01/11(��)
*        Author		: Gakuto.S
*        File		: Object_Table.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Object.h"

class IObjectGLTF;

class ObjectTable : public IObject3D
{
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
	/// \brief �`�揈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Draw() override;

	//**************************************************
	/// \brief �s���z�[���̃s�����Z�b�g����ʒu���擾
	/// 
	/// \return �ʒu
	//**************************************************
	DirectX::XMFLOAT3 GetPinWholePinSetPosition(int index);

	//**************************************************
	/// \brief �񓚗p�s���z�[���̃s�����Z�b�g����ʒu���擾
	/// 
	/// \return �ʒu
	//**************************************************
	DirectX::XMFLOAT3 GetHitBrowPinWholePinSetPosition(int index);

	//**************************************************
	/// \brief �J���[�s���z�[���̃s�����Z�b�g����ʒu���擾
	/// 
	/// \return �ʒu
	//**************************************************
	DirectX::XMFLOAT3 GetColorPinWholePinSetPosition(int index);

	//**************************************************
	/// \brief �R�~�b�g�{�^����J�[�\���̕\���ʒu���擾
	/// 
	/// \return �ʒu
	//**************************************************
	DirectX::XMFLOAT3 GetComitButtonPosition(int currentlyColumnIndex);

private:
	std::map<int, IObjectGLTF*> m_pinWholes;
	std::map<int, IObjectGLTF*> m_ansPinWholes;
	std::map<int, IObjectGLTF*>	m_colorPinWholes;
	IObject3D*					m_comitButton;

public:
	static const int k_ROW = 4;
	static const int k_COL = 8;
	static const int k_COLOR_NUM = 6;
};

