/*===================================================================================
*    Date : 2022/11/28(��)
*        Author		: Gakuto.S
*        File		: Vulkan_Pipeline.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Vulkan_Graphics.h"
#include "../IF_Pipeline.h"

class VulkanPipeline : public IPipeline
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	VulkanPipeline(
		VkPipeline pipeline,
		VkPipelineLayout pipelineLayout,
		std::vector<VkDescriptorSet> shaderResourceSets,
		VkDescriptorSet textureResourceSet,
		VkDescriptorSet samplerResourceSet,
		std::vector<VkDescriptorSetLayout> descriptorLayouts,
		std::vector<VkDescriptorPool> descriptorPools,
		VulkanGraphics* graphics
	)
		:m_pipeline(pipeline),
		m_pipelineLayout(pipelineLayout),
		m_descriptorSetLayouts(descriptorLayouts),
		m_descriptorPools(descriptorPools),
		m_graphics(graphics)
	{
		if (!shaderResourceSets.empty())
		{
			m_resourceSets[0].push_back(shaderResourceSets[0]);
			m_resourceSets[1].push_back(shaderResourceSets[0]);
		}
		if (textureResourceSet)
		{
			m_resourceSets[0].push_back(textureResourceSet);
			m_resourceSets[1].push_back(textureResourceSet);
		}
		if (samplerResourceSet)
		{
			m_resourceSets[0].push_back(samplerResourceSet);
			m_resourceSets[1].push_back(samplerResourceSet);
		}

	}

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~VulkanPipeline()
	{
		// �f�X�N���v�^�v�[���̔j��
		for(auto& pool : m_descriptorPools)
			vkDestroyDescriptorPool(m_graphics->getDevice(), pool, nullptr);

		// �f�X�N���v�^���C�A�E�g�̔j��
		for(auto& layout : m_descriptorSetLayouts)
			vkDestroyDescriptorSetLayout(m_graphics->getDevice(), layout, nullptr);

		// �p�C�v���C�����C�A�E�g�̔j��
		vkDestroyPipelineLayout(m_graphics->getDevice(), m_pipelineLayout, nullptr);

		// �p�C�v���C���̔j��
		vkDestroyPipeline(m_graphics->getDevice(), m_pipeline, nullptr);
	}

public:
	//**************************************************
	/// \brief �p�C�v���C���̎擾
	/// 
	/// \return �p�C�v���C��
	//**************************************************
	VkPipeline GetPipeline() { return m_pipeline; }

	//**************************************************
	/// \brief �p�C�v���C�����C�A�E�g�̎擾
	/// 
	/// \return �p�C�v���C�����C�A�E�g
	//**************************************************
	VkPipelineLayout GetPipelineLayout() { return m_pipelineLayout; }

	//**************************************************
	/// \brief �V�F�[�_���\�[�X�f�X�N���v�^�̎擾
	/// 
	/// \return �V�F�[�_���\�[�X�f�X�N���v�^
	//**************************************************
	std::vector<VkDescriptorSet> GetResourceDescriptor(uint32_t index) { return m_resourceSets[index]; }

private:
	VkPipeline				m_pipeline;
	VkPipelineLayout		m_pipelineLayout;


	std::vector<VkDescriptorSetLayout>	m_descriptorSetLayouts;
	std::vector<VkDescriptorPool>		m_descriptorPools;

	std::map<uint32_t, std::vector<VkDescriptorSet>> m_resourceSets;

	VulkanGraphics*			m_graphics;
};

