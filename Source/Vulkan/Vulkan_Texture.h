/*===================================================================================
*    Date : 2022/12/02(金)
*        Author		: Gakuto.S
*        File		: Vulkan_Texture.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Vulkan_Graphics.h"
#include "../IF_Texture.h"

class VulkanTexture : public ITexture
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	VulkanTexture(
		const char* fileName,
		uint32_t slotNum,
		VulkanGraphics* graphics,
		bool memory = false
	);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~VulkanTexture();

	//**************************************************
	/// \brief デスクリプタ―レイアウトバインディングを取得
	/// 
	/// \return デスクリプターレイアウトバインディング
	//**************************************************
	VkDescriptorSetLayoutBinding GetDescriptorSetLayoutBinding() { return m_descriptorSetLayoutBinding; }

	//**************************************************
	/// \brief デスクリプタープールサイズの取得
	/// 
	/// \return デスクリプタープールサイズ
	//**************************************************
	VkDescriptorPoolSize GetDescriptorPoolSize() { return m_descriptorPoolSize; }

	//**************************************************
	/// \brief デスクリプタイメージインフォの取得
	/// 
	/// \return デスクリプタイメージインフォ
	//**************************************************
	VkWriteDescriptorSet GetWriteDescriptorSet() { return m_writeDescriptorSet; }

private:
	graphics::TEXTURE_OBJECT		m_object;
	VkDescriptorSetLayoutBinding	m_descriptorSetLayoutBinding;
	VkDescriptorPoolSize			m_descriptorPoolSize;
	VkDescriptorImageInfo			m_descriptorImageInfo;
	VkWriteDescriptorSet			m_writeDescriptorSet;

	VulkanGraphics* m_graphics;
};

