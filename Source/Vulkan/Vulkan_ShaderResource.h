/*===================================================================================
*    Date : 2022/12/01(木)
*        Author		: Gakuto.S
*        File		: Vulkan_ShaderResource.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Vulkan_Graphics.h"
#include "../IF_ShaderResource.h"

class VulkanShaderResource: public IShaderResource
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	VulkanShaderResource(
		uint32_t size,
		uint32_t binding,
		VulkanGraphics* graphics
	);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~VulkanShaderResource();

	//**************************************************
	/// \brief マップ
	/// 
	/// \return なし
	//**************************************************
	bool Map(void* data, size_t size) override;

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
	/// \brief デスクリプターバッファインフォの取得
	/// 
	/// \return デスクリプターバッファインフォ
	//**************************************************
	VkDescriptorBufferInfo GetDescriptorBufferInfo(unsigned int index);

	//**************************************************
	/// \brief デスクリプターライトセットの取得
	/// 
	/// \return デスクリプターバッファインフォ
	//**************************************************
	VkWriteDescriptorSet GetWriteDescriptorSet() { return m_writeDescriptorSet; }

private:
	std::vector<graphics::BUFFER_OBJECT>	m_objects;
	VkDescriptorSetLayoutBinding			m_descriptorSetLayoutBinding;
	VkDescriptorPoolSize					m_descriptorPoolSize;
	VkDescriptorBufferInfo					m_descriptorBufferInfo;
	VkWriteDescriptorSet					m_writeDescriptorSet;

	VulkanGraphics*							m_graphics;
};

