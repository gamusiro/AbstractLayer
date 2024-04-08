/*===================================================================================
*    Date : 2022/11/28(月)
*        Author		: Gakuto.S
*        File		: IF_ElementBufferObject.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_BufferObject.h"

//**************************************************
/// \brief インデックスタイプ
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
	/// \brief コンストラクタ
	/// 
	/// \param[in] countIndex
	/// 
	/// \return なし
	//**************************************************
	IElementBufferObject(unsigned int countIndex)
		:k_INDEX_NUM(countIndex)
	{}

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	virtual ~IElementBufferObject() {}

public:
	const unsigned int k_INDEX_NUM;
};

