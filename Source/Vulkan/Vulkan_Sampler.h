/*===================================================================================
*    Date : 2022/12/03(�y)
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
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	VulkanSampler(
		sampler::SAMPLER_DESC desc,
		uint32_t binding,
		VulkanGraphics* graphics
	);

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~VulkanSampler();

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
	VkSampler						m_sampler;
	VkDescriptorSetLayoutBinding	m_descriptorSetLayoutBinding;
	VkDescriptorPoolSize			m_descriptorPoolSize;
	VkDescriptorImageInfo			m_descriptorImageInfo;
	VkWriteDescriptorSet			m_writeDescriptorSet;

	VulkanGraphics* m_graphics;
};

