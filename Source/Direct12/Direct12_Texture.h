/*===================================================================================
*    Date : 2022/12/04(日)
*        Author		: Gakuto.S
*        File		: Direct12_Texture.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Direct12_Graphics.h"
#include "../IF_Texture.h"

class Direct12Texture : public ITexture
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	Direct12Texture(
		const char* fileName,
		UINT slotNum,
		Direct12Graphics* graphics,
		bool memory = false
	);


	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~Direct12Texture();


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
	ID3D12DescriptorHeap* GetHeap() { return m_heap; }

private:
	D3D12_ROOT_PARAMETER	m_rootParam;
	D3D12_DESCRIPTOR_RANGE	m_range;

	ID3D12Resource*			m_buffer;
	ID3D12DescriptorHeap*	m_heap;
};

