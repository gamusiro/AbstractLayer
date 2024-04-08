/*===================================================================================
*    Date : 2022/12/02(金)
*        Author		: Gakuto.S
*        File		: Vulkan_Texture.cpp
*        Detail		:
===================================================================================*/
#include "Vulkan_Texture.h"

VulkanTexture::VulkanTexture(const char* fileName, uint32_t slotNum, VulkanGraphics* graphics, bool memory)
	:m_object(),
	m_descriptorSetLayoutBinding(),
	m_descriptorPoolSize(),
	m_descriptorImageInfo(),
	m_writeDescriptorSet(),
	m_graphics(graphics)
{
	VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;

	common::LoadTexture texture(fileName);
	int width					= texture.GetWidth();
	int height					= texture.GetHeight();
	unsigned char* metaImage	= texture.GetImage();

	// テクスチャ用のオブジェクト作成
	m_object = m_graphics->createTextureObject(width, height, common::LoadTexture::k_RGBA, format, metaImage);

	{// レイアウトの定義
		m_descriptorSetLayoutBinding.binding			= slotNum;
		m_descriptorSetLayoutBinding.descriptorCount	= 1;
		m_descriptorSetLayoutBinding.descriptorType		= VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		m_descriptorSetLayoutBinding.stageFlags			= VK_SHADER_STAGE_FRAGMENT_BIT;
	}

	{// プールサイズの指定
		m_descriptorPoolSize.type				= VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		m_descriptorPoolSize.descriptorCount	= 1;
	}

	{// "VkDescriptorImageInfo" 構造体の設定
		m_descriptorImageInfo.sampler		= VK_NULL_HANDLE;
		m_descriptorImageInfo.imageView		= m_object.view;
		m_descriptorImageInfo.imageLayout	= VK_IMAGE_LAYOUT_GENERAL;
	}
	
	{// "VkWriteDescriptorSet" 構造体の設定
		m_writeDescriptorSet.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		m_writeDescriptorSet.dstBinding			= slotNum;
		m_writeDescriptorSet.dstArrayElement	= 0;
		m_writeDescriptorSet.descriptorCount	= 1;
		m_writeDescriptorSet.descriptorType		= VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		m_writeDescriptorSet.pImageInfo			= &m_descriptorImageInfo;
	}
}

VulkanTexture::~VulkanTexture()
{
	m_graphics->destroyTextureObject(m_object);
}
