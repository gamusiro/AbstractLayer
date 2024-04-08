/*===================================================================================
*    Date : 2022/12/05(月)
*        Author		: Gakuto.S
*        File		: Direct12_Sampler.cpp
*        Detail		:
===================================================================================*/
#include "Direct12_Sampler.h"

Direct12Sampler::Direct12Sampler(sampler::SAMPLER_DESC desc, UINT slotNum)
	:m_samplerDesc()
{
	// サンプラーの設定
	{// "D3D12_STATIC_SAMPLER_DESC" 構造体の設定
		m_samplerDesc.AddressU			= D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		m_samplerDesc.AddressV			= D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		m_samplerDesc.AddressW			= D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		m_samplerDesc.BorderColor		= D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		m_samplerDesc.Filter			= D3D12_FILTER::D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		m_samplerDesc.MaxLOD			= D3D12_FLOAT32_MAX;
		m_samplerDesc.MinLOD			= 0.0f;
		m_samplerDesc.ShaderRegister	= slotNum;
		m_samplerDesc.ComparisonFunc	= D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
		m_samplerDesc.ShaderVisibility	= D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
	}
}
