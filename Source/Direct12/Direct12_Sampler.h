/*===================================================================================
*    Date : 2022/12/05(��)
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
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	Direct12Sampler(
		sampler::SAMPLER_DESC desc,
		UINT binding
	);

	////////		Direct12�Ɉˑ����郁�\�b�h�Q		////////
public:
	//**************************************************
	/// \brief �o�b�t�@�r���[�̎擾
	/// 
	/// \return �o�b�t�@�r���[
	//**************************************************
	D3D12_STATIC_SAMPLER_DESC GetSamplerDesc() { return m_samplerDesc; }

private:
	D3D12_STATIC_SAMPLER_DESC m_samplerDesc;
};

