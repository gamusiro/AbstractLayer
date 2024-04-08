/*===================================================================================
*    Date : 2022/12/05(月)
*        Author		: Gakuto.S
*        File		: Direct12_Sampler.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Direct12_Graphics.h"
#include "../IF_Sampler.h"

class Direct12Sampler : public ISampler
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	Direct12Sampler(
		sampler::SAMPLER_DESC desc,
		UINT binding
	);

	////////		Direct12に依存するメソッド群		////////
public:
	//**************************************************
	/// \brief バッファビューの取得
	/// 
	/// \return バッファビュー
	//**************************************************
	D3D12_STATIC_SAMPLER_DESC GetSamplerDesc() { return m_samplerDesc; }

private:
	D3D12_STATIC_SAMPLER_DESC m_samplerDesc;
};

