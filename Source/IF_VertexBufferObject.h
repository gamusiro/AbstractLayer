/*===================================================================================
*    Date : 2022/11/28(月)
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
	/// \brief コンストラクタ
	/// 
	/// \param[in] stride
	/// 
	/// \return なし
	//**************************************************
	IVertexBufferObject(size_t stride)
		:k_STRIDE(stride)
	{}

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	virtual ~IVertexBufferObject() {}

public:
	const size_t k_STRIDE;
};

