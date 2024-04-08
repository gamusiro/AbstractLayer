/*===================================================================================
*    Date : 2022/12/03(土)
*        Author		: Gakuto.S
*        File		: Vulkan_Sampler.cpp
*        Detail		:
===================================================================================*/
#include "Vulkan_Sampler.h"

VulkanSampler::VulkanSampler(
	sampler::SAMPLER_DESC desc,
	uint32_t binding,
	VulkanGraphics* graphics)
	:m_descriptorSetLayoutBinding(),
	m_descriptorPoolSize(),
	m_descriptorImageInfo(),
	m_writeDescriptorSet(),
	m_graphics(graphics)
{
	VkSamplerCreateInfo samplerCI{};
    {// "SamplerCreateInfo" 構造体の設定
        samplerCI.sType         = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCI.minFilter     = VK_FILTER_LINEAR;
        samplerCI.magFilter     = VK_FILTER_LINEAR;
        samplerCI.addressModeU  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCI.addressModeV  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCI.maxAnisotropy = 1.0f;
        samplerCI.borderColor   = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    }

    VkResult ret{};
    ret = vkCreateSampler(m_graphics->getDevice(), &samplerCI, nullptr, &m_sampler);
    if (VFAILED(ret))
        throw "サンプラーの設定に失敗";

    {// レイアウトの定義
		m_descriptorSetLayoutBinding.binding			= binding;
		m_descriptorSetLayoutBinding.descriptorCount	= 1;
		m_descriptorSetLayoutBinding.descriptorType		= VK_DESCRIPTOR_TYPE_SAMPLER;
		m_descriptorSetLayoutBinding.stageFlags			= VK_SHADER_STAGE_FRAGMENT_BIT;
	}

	{// プールサイズの指定
		m_descriptorPoolSize.type				= VK_DESCRIPTOR_TYPE_SAMPLER;
		m_descriptorPoolSize.descriptorCount	= 1;
	}

	{// "VkDescriptorImageInfo" 構造体の設定
		m_descriptorImageInfo.sampler		= m_sampler;
		m_descriptorImageInfo.imageView		= VK_NULL_HANDLE;
		m_descriptorImageInfo.imageLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
	}
	
	{// "VkWriteDescriptorSet" 構造体の設定
		m_writeDescriptorSet.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		m_writeDescriptorSet.dstBinding			= binding;
		m_writeDescriptorSet.dstArrayElement	= 0;
		m_writeDescriptorSet.descriptorCount	= 1;
		m_writeDescriptorSet.descriptorType		= VK_DESCRIPTOR_TYPE_SAMPLER;
		m_writeDescriptorSet.pImageInfo			= &m_descriptorImageInfo;
	}
}

VulkanSampler::~VulkanSampler()
{
	vkDestroySampler(m_graphics->getDevice(), m_sampler, nullptr);
}
