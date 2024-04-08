/*===================================================================================
*    Date : 2022/11/28(��)
*        Author		: Gakuto.S
*        File		: IF_ElementBufferObject.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_BufferObject.h"

//**************************************************
/// \brief �C���f�b�N�X�^�C�v
//**************************************************
enum class INDEX_FORMAT
{
	UINT16,
	UINT32
};

class IElementBufferObject : public IBufferObject
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \param[in] countIndex
	/// 
	/// \return �Ȃ�
	//**************************************************
	IElementBufferObject(unsigned int countIndex)
		:k_INDEX_NUM(countIndex)
	{}

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual ~IElementBufferObject() {}

public:
	const unsigned int k_INDEX_NUM;
};

