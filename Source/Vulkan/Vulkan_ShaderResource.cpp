/*===================================================================================
*    Date : 2022/12/01(木)
*        Author		: Gakuto.S
*        File		: Vulkan_ShaderResource.cpp
*        Detail		:
===================================================================================*/
#include "Vulkan_ShaderResource.h"

VulkanShaderResource::VulkanShaderResource(uint32_t size, uint32_t binding, VulkanGraphics* graphics)
	:m_graphics(graphics),
	m_objects(),
	m_descriptorPoolSize(),
	m_descriptorSetLayoutBinding(),
	m_descriptorBufferInfo(),
	m_writeDescriptorSet()
{
	// バッファの作成
	m_objects.resize(graphics->k_BUFFER_NUM);
	for (auto& u : m_objects)
	{
		VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
		u = graphics->createBufferObject(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, memoryFlags);
	}

	{// レイアウトの定義
		m_descriptorSetLayoutBinding.binding			= binding;
		m_descriptorSetLayoutBinding.descriptorCount	= 1;
		m_descriptorSetLayoutBinding.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		m_descriptorSetLayoutBinding.stageFlags			= VK_SHADER_STAGE_ALL;
	}

	{// プールサイズの指定
		m_descriptorPoolSize.type				= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		m_descriptorPoolSize.descriptorCount	= uint32_t(m_objects.size());
	}

	{// バッファ情報の設定
		m_descriptorBufferInfo.buffer	= nullptr;
		m_descriptorBufferInfo.offset	= 0;
		m_descriptorBufferInfo.range	= size;
	}

	{// ライトデスクリプタセット
		m_writeDescriptorSet.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		m_writeDescriptorSet.dstBinding       = binding;
		m_writeDescriptorSet.descriptorCount  = 1;
		m_writeDescriptorSet.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	}
}

VulkanShaderResource::~VulkanShaderResource()
{
	// バッファの廃棄
	for (auto& u : m_objects)
		m_graphics->destroyBufferObject(u);
}

bool VulkanShaderResource::Map(void* data, size_t size)
{
	return m_graphics->map(m_objects[m_graphics->getCurrentlyIndex()].memory, data, size);
}

VkDescriptorBufferInfo VulkanShaderResource::GetDescriptorBufferInfo(unsigned int index)
{
	m_descriptorBufferInfo.buffer = m_objects[index].buffer;
	return m_descriptorBufferInfo;
}
