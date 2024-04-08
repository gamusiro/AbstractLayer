/*===================================================================================
*    Date : 2022/12/04(日)
*        Author		: Gakuto.S
*        File		: Direct12_Texture.cpp
*        Detail		:
===================================================================================*/
#include "Direct12_Texture.h"

Direct12Texture::Direct12Texture(const char* fileName, UINT slotNum, Direct12Graphics* graphics, bool memory)
	:m_buffer(),
	m_heap(),
	m_rootParam(),
	m_range()
{
	common::LoadTexture texture(fileName);
	int width					= texture.GetWidth();
	int height					= texture.GetHeight();
	unsigned char* metaImage	= texture.GetImage();

	// WriteToSubResourceで転送するための設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type					= D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	heapProperties.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;	//CPUから直接行う

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Format				= DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	resourceDesc.Width				= width;
	resourceDesc.Height				= height;
	resourceDesc.DepthOrArraySize	= 1;
	resourceDesc.SampleDesc.Count	= 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.MipLevels			= 1;
	resourceDesc.Dimension			= D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Layout				= D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags				= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	// テクスチャリソースの設定
	HRESULT ret{};
	ret = graphics->getDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		__uuidof(ID3D12Resource),
		(void**)&m_buffer
	);

	// テクスチャを転送する
	ret = m_buffer->WriteToSubresource(
		0,
		nullptr,
		metaImage,
		common::LoadTexture::k_RGBA * width,
		common::LoadTexture::k_RGBA * width * height
	);

	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDesc.NodeMask			= 0;
	descriptorHeapDesc.NumDescriptors	= 1;
	descriptorHeapDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	ret = graphics->getDevice()->CreateDescriptorHeap(
		&descriptorHeapDesc,
		__uuidof(ID3D12DescriptorHeap),
		(void**)&m_heap
	);

	D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceView{};
	shaderResourceView.Format					= DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	shaderResourceView.Shader4ComponentMapping	= D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	shaderResourceView.ViewDimension			= D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	shaderResourceView.Texture2D.MipLevels		= 1;

	graphics->getDevice()->CreateShaderResourceView(
		m_buffer,
		&shaderResourceView,
		m_heap->GetCPUDescriptorHandleForHeapStart()
	);

	{// "D3D12_DESCRIPTOR_RANGE" 構造体の設定
		m_range.NumDescriptors						= 1;
		m_range.RangeType							= D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		m_range.BaseShaderRegister					= slotNum;
		m_range.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	{// "D3D12_ROOT_PARAMETER" 構造体の設定
		m_rootParam.ParameterType						= D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		m_rootParam.DescriptorTable.pDescriptorRanges	= &m_range;
		m_rootParam.DescriptorTable.NumDescriptorRanges	= 1;
		m_rootParam.ShaderVisibility					= D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
	}
}

Direct12Texture::~Direct12Texture()
{
	SAFE_RELEASE(m_heap);
	SAFE_RELEASE(m_buffer);
}
