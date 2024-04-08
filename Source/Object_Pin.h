/*===================================================================================
*    Date : 2023/01/11(��)
*        Author		: Gakuto.S
*        File		: Object_Pin_White.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_ObjectGLTF.h"

class ObjectPin : public IObjectGLTF
{
public:
	enum class COLOR
	{
		BLUE,
		RED,
		YELLOW,
		GREEN,
		PINK,
		WHITE
	};


public:
	//**************************************************
	/// \brief ����������
	/// 
	/// \return �Ȃ�
	//**************************************************
	ObjectPin(COLOR color, bool useMaterial = true)
		:m_type(color),
		m_useMaterial(useMaterial),
		IObjectGLTF()
	{}

	//**************************************************
	/// \brief ����������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Init() override;

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Draw() override;

	//**************************************************
	/// \brief �J���[�̎擾
	/// 
	/// \return �s���̃J���[
	//**************************************************
	COLOR GetColor() { return m_type; }

	//**************************************************
	/// \brief �}�e���A���̏������݂�����
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetUseMaterial(bool use) { m_useMaterial = use; }

private:
	//**************************************************
	/// \brief �F��Ԃ�
	/// 
	/// \return �Ȃ�
	//**************************************************
	DirectX::XMFLOAT4 getColor();

private:
	const COLOR m_type;
	bool m_useMaterial;
};

