/*===================================================================================
*    Date : 2022/12/02(��)
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
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	VulkanTexture(
		const char* fileName,
		uint32_t slotNum,
		VulkanGraphics* graphics,
		bool memory = false
	);

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~VulkanTexture();

	//**************************************************
	/// \brief �f�X�N���v�^�\���C�A�E�g�o�C���f�B���O���擾
	/// 
	/// \return �f�X�N���v�^�[���C�A�E�g�o�C���f�B���O
	//**************************************************
	VkDescriptorSetLayoutBinding GetDescriptorSetLayoutBinding() { return m_descriptorSetLayoutBinding; }

	//**************************************************
	/// \brief �f�X�N���v�^�[�v�[���T�C�Y�̎擾
	/// 
	/// \return �f�X�N���v�^�[�v�[���T�C�Y
	//**************************************************
	VkDescriptorPoolSize GetDescriptorPoolSize() { return m_descriptorPoolSize; }

	//**************************************************
	/// \brief �f�X�N���v�^�C���[�W�C���t�H�̎擾
	/// 
	/// \return �f�X�N���v�^�C���[�W�C���t�H
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

