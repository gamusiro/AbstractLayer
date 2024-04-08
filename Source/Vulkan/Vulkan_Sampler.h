/*===================================================================================
*    Date : 2022/12/03(土)
*        Author		: Gakuto.S
*        File		: Vulkan_Sampler.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Vulkan_Graphics.h"
#include "../IF_Sampler.h"

class VulkanSampler : public ISampler
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	VulkanSampler(
		sampler::SAMPLER_DESC desc,
		uint32_t binding,
		VulkanGraphics* graphics
	);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~VulkanSampler();

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
	VkSampler						m_sampler;
	VkDescriptorSetLayoutBinding	m_descriptorSetLayoutBinding;
	VkDescriptorPoolSize			m_descriptorPoolSize;
	VkDescriptorImageInfo			m_descriptorImageInfo;
	VkWriteDescriptorSet			m_writeDescriptorSet;

	VulkanGraphics* m_graphics;
};

