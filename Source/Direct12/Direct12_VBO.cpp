/*===================================================================================
*    Date : 2022/12/04(��)
*        Author		: Gakuto.S
*        File		: Direct12_VBO.h
*        Detail		:
===================================================================================*/
#include "Direct12_VBO.h"

Direct12VBO::Direct12VBO(UINT size, size_t stride, Direct12Graphics* graphics)
    :m_buffer(),
    m_view(),
    IVertexBufferObject(stride)
{
    HRESULT ret{};

	D3D12_HEAP_PROPERTIES heapProperties{};
	{// "HEAP_PROPERTIES" �\���̂̐ݒ�
		heapProperties.Type					= D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
		heapProperties.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN; 
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;				
	}
	

	D3D12_RESOURCE_DESC resourceDesc{};
	{// "RESOURCE_DESC" �\���̂̐ݒ�
		resourceDesc.Dimension			= D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Width				= UINT64(size);
		resourceDesc.Height				= 1;
		resourceDesc.DepthOrArraySize	= 1;
		resourceDesc.MipLevels			= 1;
		resourceDesc.Format				= DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count	= 1;
		resourceDesc.Flags				= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
		resourceDesc.Layout				= D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}

	// ���_�o�b�t�@�̐���
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
		throw "���_�o�b�t�@�̐����Ɏ��s";

	// �o�b�t�@�r���[
	m_view.BufferLocation	= m_buffer->GetGPUVirtualAddress();
	m_view.SizeInBytes		= size;
	m_view.StrideInBytes	= UINT(k_STRIDE);
}

Direct12VBO::~Direct12VBO()
{
    SAFE_RELEASE(m_buffer);
}

bool Direct12VBO::Map(void* data, size_t size)
{
	HRESULT ret{};
	void* map;
	ret = m_buffer->Map(0, nullptr, (void**)&map);
	if (FAILED(ret))
		return false;

	// �������ɃR�s�[
	memcpy(map, data, size);
	m_buffer->Unmap(0, nullptr);
	return true;
}
