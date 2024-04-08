/*===================================================================================
*    Date : 2022/12/04(日)
*        Author		: Gakuto.S
*        File		: Direct12_ShaderResource.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Direct12_Graphics.h"
#include "../IF_ShaderResource.h"

class Direct12ShaderResource : public IShaderResource
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \param[in] size
	/// \param[in] slotNum
	/// \param[in] graphics
	/// 
	/// \return なし
	//**************************************************
	Direct12ShaderResource(
		UINT size,
		UINT slotNum,
		Direct12Graphics* graphics
	);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~Direct12ShaderResource();

	////////	継承メソッド	////////
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
	/// \brief マップ
	/// 
	/// \return true なら成功
	//**************************************************
	D3D12_ROOT_PARAMETER GetRootParameter() { return m_rootParam; }

	//**************************************************
	/// \brief バッファの取得
	/// 
	/// \return true なら成功
	//**************************************************
	ID3D12Resource* GetBuffer() { return m_buffer; }

private:
	D3D12_ROOT_PARAMETER	m_rootParam;
	ID3D12Resource*			m_buffer;
};

