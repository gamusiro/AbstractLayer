/*===================================================================================
*    Date : 2022/12/04(日)
*        Author		: Gakuto.S
*        File		: Direct12_EBO.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Direct12_Graphics.h"
#include "../IF_ElementBufferObject.h"

class Direct12EBO : public IElementBufferObject
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \param[in] size
	/// \param[in] countIndex
	/// \param[in] format
	/// \param[in] graphics
	/// 
	/// \return なし
	//**************************************************
	Direct12EBO(
		UINT size,
		unsigned int countIndex,
		DXGI_FORMAT format,
		Direct12Graphics* graphics
	);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~Direct12EBO();

	////////		継承メソッド群		////////
public:
	//**************************************************
	/// \brief マップ
	/// 
	/// \return true なら成功
	//**************************************************
	bool Map(void* data, size_t size) override;

	////////		Direct12に依存するメソッド群		////////
public:
	//**************************************************
	/// \brief バッファの取得
	/// 
	/// \return VkBuffer
	//**************************************************
	D3D12_INDEX_BUFFER_VIEW GetBufferView() { return m_view; }

private:
	ID3D12Resource*			m_buffer;
	D3D12_INDEX_BUFFER_VIEW	m_view;
};

