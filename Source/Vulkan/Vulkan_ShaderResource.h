/*===================================================================================
*    Date : 2022/12/01(��)
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
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	VulkanShaderResource(
		uint32_t size,
		uint32_t binding,
		VulkanGraphics* graphics
	);

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~VulkanShaderResource();

	//**************************************************
	/// \brief �}�b�v
	/// 
	/// \return �Ȃ�
	//**************************************************
	bool Map(void* data, size_t size) override;

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
	/// \brief �f�X�N���v�^�[�o�b�t�@�C���t�H�̎擾
	/// 
	/// \return �f�X�N���v�^�[�o�b�t�@�C���t�H
	//**************************************************
	VkDescriptorBufferInfo GetDescriptorBufferInfo(unsigned int index);

	//**************************************************
	/// \brief �f�X�N���v�^�[���C�g�Z�b�g�̎擾
	/// 
	/// \return �f�X�N���v�^�[�o�b�t�@�C���t�H
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

