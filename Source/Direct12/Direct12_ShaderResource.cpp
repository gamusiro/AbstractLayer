/*===================================================================================
*    Date : 2022/12/04(日)
*        Author		: Gakuto.S
*        File		: Direct12_ShaderResource.cpp
*        Detail		:
===================================================================================*/
#include "Direct12_ShaderResource.h"

Direct12ShaderResource::Direct12ShaderResource(UINT size, UINT slotNum, Direct12Graphics* graphics)
	:m_buffer(),
	m_rootParam()
{
	HRESULT ret{};

	D3D12_HEAP_PROPERTIES heapProperties{};
	{// "HEAP_PROPERTIES" 構造体の設定
		heapProperties.Type					= D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
		heapProperties.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	}

	UINT bufferSize = (size + 0xff) & ~0xff;
	D3D12_RESOURCE_DESC resourceDesc{};
	{// "RESOURCE_DESC" 構造体の設定
		resourceDesc.Dimension			= D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Width				= bufferSize;
		resourceDesc.Height				= 1;
		resourceDesc.DepthOrArraySize	= 1;
		resourceDesc.MipLevels			= 1;
		resourceDesc.Format				= DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count	= 1;
		resourceDesc.Flags				= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
		resourceDesc.Layout				= D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}

	// 定数バッファの生成
	ret = graphics->getDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		__uuidof(ID3D12Resource),
		(void**)&m_buffer
	);
	if (FAILED(ret))
		throw "定数バッファの生成に失敗";

	{// "D3D12_ROOT_PARAMETER" 構造体の設定
		m_rootParam.ParameterType				= D3D12_ROOT_PARAMETER_TYPE_CBV;
		m_rootParam.ShaderVisibility			= D3D12_SHADER_VISIBILITY_ALL;
		m_rootParam.Descriptor.RegisterSpace	= 0;
		m_rootParam.Descriptor.ShaderRegister	= slotNum;
	}
}

Direct12ShaderResource::~Direct12ShaderResource()
{
	SAFE_RELEASE(m_buffer);
}

bool Direct12ShaderResource::Map(void* data, size_t size)
{
	HRESULT ret{};
	void* p;
	ret = m_buffer->Map(0, nullptr, &p);
	if (FAILED(ret))
		return false;

	memcpy(p, data, size);
	m_buffer->Unmap(0, nullptr);
	return true;
}
