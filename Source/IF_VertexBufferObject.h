/*===================================================================================
*    Date : 2022/11/28(��)
*        Author		: Gakuto.S
*        File		: IF_VertexBufferObject.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_BufferObject.h"

class IVertexBufferObject : public IBufferObject
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \param[in] stride
	/// 
	/// \return �Ȃ�
	//**************************************************
	IVertexBufferObject(size_t stride)
		:k_STRIDE(stride)
	{}

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual ~IVertexBufferObject() {}

public:
	const size_t k_STRIDE;
};

