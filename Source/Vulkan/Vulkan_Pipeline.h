/*===================================================================================
*    Date : 2022/11/28(月)
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
	/// \brief コンストラクタ
	/// 
	/// \return なし
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
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~VulkanPipeline()
	{
		// デスクリプタプールの破棄
		for(auto& pool : m_descriptorPools)
			vkDestroyDescriptorPool(m_graphics->getDevice(), pool, nullptr);

		// デスクリプタレイアウトの破棄
		for(auto& layout : m_descriptorSetLayouts)
			vkDestroyDescriptorSetLayout(m_graphics->getDevice(), layout, nullptr);

		// パイプラインレイアウトの破棄
		vkDestroyPipelineLayout(m_graphics->getDevice(), m_pipelineLayout, nullptr);

		// パイプラインの破棄
		vkDestroyPipeline(m_graphics->getDevice(), m_pipeline, nullptr);
	}

public:
	//**************************************************
	/// \brief パイプラインの取得
	/// 
	/// \return パイプライン
	//**************************************************
	VkPipeline GetPipeline() { return m_pipeline; }

	//**************************************************
	/// \brief パイプラインレイアウトの取得
	/// 
	/// \return パイプラインレイアウト
	//**************************************************
	VkPipelineLayout GetPipelineLayout() { return m_pipelineLayout; }

	//**************************************************
	/// \brief シェーダリソースデスクリプタの取得
	/// 
	/// \return シェーダリソースデスクリプタ
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

