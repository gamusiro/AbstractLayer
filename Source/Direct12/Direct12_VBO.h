/*===================================================================================
*    Date : 2022/12/04(日)
*        Author		: Gakuto.S
*        File		: Direct12_VBO.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Direct12_Graphics.h"
#include "../IF_VertexBufferObject.h"

class Direct12VBO : public IVertexBufferObject
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \param[in] size
	/// \param[in] stride
	/// \param[in] graphics
	/// 
	/// \return なし
	//**************************************************
	Direct12VBO(
		UINT size,
		size_t stride,
		Direct12Graphics* graphics
	);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~Direct12VBO();

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
	/// \brief バッファビューの取得
	/// 
	/// \return バッファビュー
	//**************************************************
	D3D12_VERTEX_BUFFER_VIEW GetBufferView() { return m_view; }

private:
	ID3D12Resource*				m_buffer;
	D3D12_VERTEX_BUFFER_VIEW	m_view;
};

