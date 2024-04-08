/*===================================================================================
*    Date : 2022/11/28(��)
*        Author		: Gakuto.S
*        File		: Vulkan_Graphics.cpp
*        Detail		:
===================================================================================*/
#include "Vulkan_Graphics.h"

// Vulkan �I�u�W�F�N�g�Q
#include "Vulkan_VBO.h"
#include "Vulkan_EBO.h"
#include "Vulkan_Pipeline.h"
#include "Vulkan_ShaderResource.h"
#include "Vulkan_Texture.h"
#include "Vulkan_Sampler.h"

using namespace DirectX;
using namespace std;

VulkanGraphics::VulkanGraphics(int width, int height, WindowWin* window)
    :m_instance(),
    m_physicalDevice(),
    m_physicalDeviceMemProps(),
    m_graphicsQueueIndex(),
    m_device(),
    m_deviceQueue(),
    m_commandPool(),
    m_surface(),
    m_surfaceCap(),
    m_surfaceFormat(),
    m_swapchain(),
    m_swapchainImages(),
    m_swapchainViews(),
    m_depthImage(),
    m_depthImageView(),
    m_depthImageMemory(),
    m_renderPass(),
    m_frameBuffers(),
    m_commands(),
    m_fences(),
    m_renderSem(),
    m_presentSem(),
    m_imageIndex()
{
    createInstance();
    createPhysicalDevice();
    createDevice();
    createSurface(window->GetHandle());
    createSwapchain(VK_NULL_HANDLE);
    createDepthBuffer();
    createRenderPass();
    createFrameBuffer();
    createFence();
}

VulkanGraphics::~VulkanGraphics()
{
    // �R�}���h�o�b�t�@�̉��
    vkFreeCommandBuffers(m_device, m_commandPool, uint32_t(m_commands.size()), m_commands.data());

    // �����_�[�p�X�̔j��
    vkDestroyRenderPass(m_device, m_renderPass, nullptr);

    // �t���[���o�b�t�@�̔j��
    for (auto& f : m_frameBuffers)
        vkDestroyFramebuffer(m_device, f, nullptr);

    // �[�x�o�b�t�@�̔j��
    vkFreeMemory(m_device, m_depthImageMemory, nullptr);
    vkDestroyImage(m_device, m_depthImage, nullptr);
    vkDestroyImageView(m_device, m_depthImageView, nullptr);

    // �C���[�W�r���[�̔j��
    for (auto& s : m_swapchainViews)
        vkDestroyImageView(m_device, s, nullptr);

    // �X���b�v�`�F�C���̔j��
    vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);

    // �T�[�t�F�[�X�̔j��
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

    // �R�}���h�v�[���̔j��
    vkDestroyCommandPool(m_device, m_commandPool, nullptr);

    // �f�o�C�X�̔j��
    vkDestroyDevice(m_device, nullptr);

    // �C���X�^���X�̔j��
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanGraphics::Clear()
{
    // ���݂̃X���b�v�`�F�[���̃o�b�t�@�C���f�b�N�X���擾
    vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_presentSem, VK_NULL_HANDLE, &m_imageIndex);
    vkWaitForFences(m_device, 1, &m_fences[m_imageIndex], VK_TRUE, UINT64_MAX);

    array<VkClearValue, 2> clearValue
    {
        {{0.5f, 0.0f, 0.0f, 1.0f},	// �N���A�J���[
        {1.0f, 0}}					// �[�x�o�b�t�@
    };

    VkRenderPassBeginInfo renderPassBI{};
    {// "RenderPassBeginInfo" �\���̂̐ݒ�
        renderPassBI.sType              = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBI.renderPass         = m_renderPass;
        renderPassBI.framebuffer        = m_frameBuffers[m_imageIndex];
        renderPassBI.renderArea.offset  = { 0, 0 };
        renderPassBI.renderArea.extent  = m_surfaceCap.currentExtent;
        renderPassBI.pClearValues       = clearValue.data();
        renderPassBI.clearValueCount    = uint32_t(clearValue.size());
    }

    VkCommandBufferBeginInfo commandBufferBI{};
    {// "CommandBufferBeginInfo" �\���̂̐ݒ�
        commandBufferBI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    }

    vkBeginCommandBuffer(m_commands[m_imageIndex], &commandBufferBI);
    vkCmdBeginRenderPass(m_commands[m_imageIndex], &renderPassBI, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdSetScissor(m_commands[m_imageIndex], 0, 1, &m_scissor);
    vkCmdSetViewport(m_commands[m_imageIndex], 0, 1, &m_viewport);
}

void VulkanGraphics::Present()
{
    vkCmdEndRenderPass(m_commands[m_imageIndex]);
	vkEndCommandBuffer(m_commands[m_imageIndex]);

    VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submitInfo{};
    {// "SubmitInfo" �\���̂̐ݒ�
        submitInfo.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
	    submitInfo.commandBufferCount	= 1;
	    submitInfo.pCommandBuffers		= &m_commands[m_imageIndex];
	    submitInfo.pWaitDstStageMask	= &waitStageMask;
	    submitInfo.waitSemaphoreCount	= 1;
	    submitInfo.pWaitSemaphores		= &m_presentSem;
	    submitInfo.signalSemaphoreCount = 1;
	    submitInfo.pSignalSemaphores	= &m_renderSem;
    }
	
	vkResetFences(m_device, 1, &m_fences[m_imageIndex]);
	vkQueueSubmit(m_deviceQueue, 1, &submitInfo, m_fences[m_imageIndex]);

	VkPresentInfoKHR presentInfo{};
    {// "PresentInfoKHR" �\���̂̐ݒ�
        presentInfo.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	    presentInfo.swapchainCount		= 1;
	    presentInfo.pSwapchains			= &m_swapchain;
	    presentInfo.pImageIndices		= &m_imageIndex;
	    presentInfo.waitSemaphoreCount	= 1;
	    presentInfo.pWaitSemaphores		= &m_renderSem;
    }

	vkQueuePresentKHR(m_deviceQueue, &presentInfo);
}

/* ���_�o�b�t�@�֘A */
IVertexBufferObject* VulkanGraphics::CreateVertexBuffer(size_t size, size_t stride)
{
    return new VulkanVBO(uint32_t(size), stride, this);
}
void VulkanGraphics::SetVertexBuffer(IVertexBufferObject* vertexBuffer)
{
    VulkanVBO* vbo = (VulkanVBO*)vertexBuffer;

    VkBuffer buffer     = vbo->GetBuffer();
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(m_commands[m_imageIndex], 0, 1, &buffer, &offset);
}

/* �C���f�b�N�X�o�b�t�@�֘A */
IElementBufferObject* VulkanGraphics::CreateIndexBuffer(size_t size, unsigned int countIndex, INDEX_FORMAT format)
{
    std::map<INDEX_FORMAT, VkIndexType> k_TYPE
    {
        {INDEX_FORMAT::UINT16, VkIndexType::VK_INDEX_TYPE_UINT16},
        {INDEX_FORMAT::UINT32, VkIndexType::VK_INDEX_TYPE_UINT32}
    };

    return new VulkanEBO(uint32_t(size), countIndex, k_TYPE[format], this);
}
void VulkanGraphics::SetIndexBuffer(IElementBufferObject* elementBuffer)
{
    VulkanEBO* buffer = (VulkanEBO*)elementBuffer;
    VkDeviceSize offset = 0;

    vkCmdBindIndexBuffer(m_commands[m_imageIndex], buffer->GetBuffer(), offset, buffer->GetType());
}

/* �p�C�v���C���֘A */
IPipeline* VulkanGraphics::CreatePipeline(
    size_t stride,
    std::vector<pipeline::vertexLayout::VERTEX_LAYOUT_DESC> layouts,
    pipeline::blend::BLEND_DESC blendDesc,
    pipeline::rasterizer::RASTERIZER_DESC rasterizerDesc,
    pipeline::primitive::PRIMITIVE_TOPOLOGY primitive,
    pipeline::shader::SHADER_DESC shaderDesc,
    std::vector<IShaderResource*> shaderResources,
    std::vector<ITexture*> textureResources,
    std::vector<ISampler*> samplerResources
)
{
#pragma region ���[�h�ϊ�
    //**************************************************
    /// \brief �u�����h���[�h
    //**************************************************
    std::map<pipeline::blend::BLEND, VkBlendFactor> k_BLEND
    {
        {pipeline::blend::BLEND::ZERO,          VkBlendFactor::VK_BLEND_FACTOR_ZERO },
        {pipeline::blend::BLEND::ONE,           VkBlendFactor::VK_BLEND_FACTOR_ONE },
        {pipeline::blend::BLEND::SRC_ALPHA,     VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA },
        {pipeline::blend::BLEND::INV_SRC_ALPHA, VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA },
    };

    //**************************************************
    /// \brief �u�����h�I�v�V�������[�h
    //**************************************************
    std::map<pipeline::blend::BLEND_OP, VkBlendOp> k_BLEND_OP
    {
        {pipeline::blend::BLEND_OP::ADD, VkBlendOp::VK_BLEND_OP_ADD },
    };

    //**************************************************
    /// \brief ���_�f�[�^�t�H�[�}�b�g
    //**************************************************
    std::map<pipeline::vertexLayout::FORMAT, VkFormat> k_VERTEX_FORMAT
    {
        {pipeline::vertexLayout::FORMAT::R32G32B32_FLOAT, VkFormat::VK_FORMAT_R32G32B32_SFLOAT},
        {pipeline::vertexLayout::FORMAT::R32G32_FLOAT, VkFormat::VK_FORMAT_R32G32_SFLOAT},
    };

    //**************************************************
    /// \brief �J�����O���[�h
    //**************************************************
    std::map<pipeline::rasterizer::CULL_MODE, VkCullModeFlagBits> k_CullMode
    {
        {pipeline::rasterizer::CULL_MODE::NONE,	VkCullModeFlagBits::VK_CULL_MODE_NONE},
        {pipeline::rasterizer::CULL_MODE::FRONT,	VkCullModeFlagBits::VK_CULL_MODE_FRONT_BIT},
        {pipeline::rasterizer::CULL_MODE::BACK,	VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT},
    };

    //**************************************************
    /// \brief �t�B�����[�h
    //**************************************************
    std::map<pipeline::rasterizer::FILL_MODE, VkPolygonMode> k_FillMode
    {
        {pipeline::rasterizer::FILL_MODE::SOLID,		VkPolygonMode::VK_POLYGON_MODE_FILL},
        {pipeline::rasterizer::FILL_MODE::WIREFRAME,	VkPolygonMode::VK_POLYGON_MODE_LINE}
    };

    //**************************************************
    /// \brief �v���~�e�B�u�g�|���W�[
    //**************************************************
    std::map<pipeline::primitive::PRIMITIVE_TOPOLOGY, VkPrimitiveTopology> k_PrimitiveMode
    {
        { pipeline::primitive::PRIMITIVE_TOPOLOGY::TRIANGLE_LIST,  VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST},
        { pipeline::primitive::PRIMITIVE_TOPOLOGY::TRIANGLE_STRIP, VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP }
    };
#pragma endregion

    VkResult ret{};

#pragma region �����f�[�^�g�p
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCI{};
    {// "PipelineInputAssemblyStateCreateInfo" �\���̂̐ݒ�
        inputAssemblyCI.sType       = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyCI.topology    = k_PrimitiveMode[primitive];
    }


    VkViewport viewport{};
	VkRect2D scissor{};

	VkPipelineViewportStateCreateInfo viewportCI{};
	{// "PipelineViewportStateCreateInfo" �\���̂̐ݒ�
		viewportCI.sType            = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportCI.viewportCount	= 1;
		viewportCI.pViewports		= &viewport;
		viewportCI.scissorCount		= 1;
		viewportCI.pScissors		= &scissor;
	}

    VkPipelineMultisampleStateCreateInfo multisampleCI{};
	{// "PipelineMultisampleStateCreateInfo" �\���̂̐ݒ�
		multisampleCI.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleCI.rasterizationSamples	= VK_SAMPLE_COUNT_1_BIT;
	}
#pragma endregion


#pragma region �V�F�[�_���\�[�X

    // �V�F�[�_���\�[�X�f�[�^���L���X�g����
    vector<VulkanShaderResource*> vshaderResources;
    for (auto& sr : shaderResources)
        vshaderResources.push_back((VulkanShaderResource*)sr);

    // ���C�A�E�g�ƃv�[���̍쐬
    VkDescriptorSetLayout shaderResourceSetLayout{};
    VkDescriptorPool shaderResourcePool{};
    this->pipelineCreateSetLayoutAndCreatePool(
        vshaderResources,
        shaderResourceSetLayout,
        shaderResourcePool
    );

    // �f�X�N���v�^�̃Z�b�g
    std::vector<VkDescriptorSet> shaderResourceSets;
    this->pipelineDescriptorSet(
        vshaderResources,
        shaderResourceSetLayout,
        shaderResourcePool,
        shaderResourceSets
    );

#pragma endregion

#pragma region �e�N�X�`�����\�[�X
    // �e�N�X�`�����\�[�X�f�[�^���L���X�g����
    vector<VulkanTexture*> vtextureResources;
    for (auto& tr : textureResources)
        vtextureResources.push_back((VulkanTexture*)tr);

    // ���C�A�E�g�ƃv�[���̍쐬
    VkDescriptorSetLayout textureResourceSetLayout{};
    VkDescriptorPool textureResourcePool{};
    this->pipelineCreateSetLayoutAndCreatePool(
        vtextureResources,
        textureResourceSetLayout,
        textureResourcePool
    );

    // �f�X�N���v�^�̃Z�b�g
    VkDescriptorSet textureResourceSet{};
    textureResourceSet = this->pipelineDescriptorSet(
        vtextureResources,
        textureResourceSetLayout,
        textureResourcePool
    );
#pragma endregion

#pragma region �T���v���[���\�[�X
    // �T���v���[���\�[�X�f�[�^���L���X�g����
    vector<VulkanSampler*> vsamplerResources;
    for (auto& sr : samplerResources)
        vsamplerResources.push_back((VulkanSampler*)sr);

    // ���C�A�E�g�ƃv�[���̍쐬
    VkDescriptorSetLayout samplerResourceSetLayout{};
    VkDescriptorPool samplerResourcePool{};
    this->pipelineCreateSetLayoutAndCreatePool(
        vsamplerResources,
        samplerResourceSetLayout,
        samplerResourcePool
    );

    // �f�X�N���v�^�̃Z�b�g
    VkDescriptorSet samplerResourceSet{};
    samplerResourceSet = this->pipelineDescriptorSet(
        vsamplerResources,
        samplerResourceSetLayout,
        samplerResourcePool
    );
#pragma endregion


    // �f�X�N���v�^���C�A�E�g���܂Ƃ߂Ē�`(shaderResource, Texture, Sampler)
    vector<VkDescriptorSetLayout> descriptorSetLayouts;
    if (shaderResourceSetLayout)  descriptorSetLayouts.push_back(shaderResourceSetLayout);
    if (textureResourceSetLayout) descriptorSetLayouts.push_back(textureResourceSetLayout);
    if (samplerResourceSetLayout) descriptorSetLayouts.push_back(samplerResourceSetLayout);

    VkPipelineLayoutCreateInfo pipelineLayoutCI{};
    {// "PipelineLayoutCreateInfo" �\���̂̐ݒ�
        pipelineLayoutCI.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCI.setLayoutCount = uint32_t(descriptorSetLayouts.size());
        pipelineLayoutCI.pSetLayouts    = descriptorSetLayouts.data();
    }

    // �p�C�v���C�����C�A�E�g�̍쐬
    VkPipelineLayout pipelineLayout{};
    ret = vkCreatePipelineLayout(m_device, &pipelineLayoutCI, nullptr, &pipelineLayout);
#pragma endregion


#pragma region �p�C�v���C���̕K�{����
    VkVertexInputBindingDescription	vertexInputBD{};
    {// "VertexInputBindingDescription" �\���̂̐ݒ�
		vertexInputBD.binding	= 0;
		vertexInputBD.stride	= uint32_t(stride);
		vertexInputBD.inputRate	= VK_VERTEX_INPUT_RATE_VERTEX;
	}

    std::vector<VkVertexInputAttributeDescription>	vertexInputAD(layouts.size());
    for (uint32_t i = 0; i < layouts.size(); ++i)
        vertexInputAD[i] = { i, layouts[i].binding, k_VERTEX_FORMAT[layouts[i].format], layouts[i].offset };

    VkPipelineVertexInputStateCreateInfo vertexInputStateCI{};
	{// "PipelineVertexInputStateCreateInfo" �\���̂̐ݒ�
		vertexInputStateCI.sType							= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCI.vertexBindingDescriptionCount	= 1;
		vertexInputStateCI.pVertexBindingDescriptions		= &vertexInputBD;
		vertexInputStateCI.vertexAttributeDescriptionCount	= uint32_t(vertexInputAD.size());
		vertexInputStateCI.pVertexAttributeDescriptions		= vertexInputAD.data();
	}

    // �V�F�[�_�̓ǂݍ��ݏ���
    vector<VkPipelineShaderStageCreateInfo> shaderStages
    {
        this->loadShaderModule((shaderDesc.vsName + ".spv").c_str(), VK_SHADER_STAGE_VERTEX_BIT, shaderDesc.vsEntryName.c_str()),
        this->loadShaderModule((shaderDesc.psName + ".spv").c_str(), VK_SHADER_STAGE_FRAGMENT_BIT, shaderDesc.psEntryName.c_str()),
    };

    // �u�����h�X�e�[�g�̐ݒ�
    VkPipelineColorBlendAttachmentState colorBlendAS{};
    {// "PipelineColorBlendAttachmentState" �\���̂̐ݒ�
        colorBlendAS.blendEnable          = blendDesc.blendEnable;
        colorBlendAS.srcColorBlendFactor  = k_BLEND[blendDesc.srcColorBlend];
        colorBlendAS.dstColorBlendFactor  = k_BLEND[blendDesc.dstColorBlend];
        colorBlendAS.srcAlphaBlendFactor  = k_BLEND[blendDesc.srcAlphaBlend];
        colorBlendAS.dstAlphaBlendFactor  = k_BLEND[blendDesc.dstAlphaBlend];
        colorBlendAS.colorBlendOp         = k_BLEND_OP[blendDesc.colorBlendOp];
        colorBlendAS.alphaBlendOp         = k_BLEND_OP[blendDesc.alphaBlendOp];
        colorBlendAS.colorWriteMask       = blendDesc.colorWriteMask;
    }

    VkPipelineColorBlendStateCreateInfo colorBlendCI{};
    {// "PipelineColorBlendStateCreateInfo" �\���̂̐ݒ�
        colorBlendCI.sType            = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendCI.attachmentCount  = 1;
        colorBlendCI.pAttachments     = &colorBlendAS;
    }

    // ���X�^���C�U�̐ݒ�
    VkPipelineRasterizationStateCreateInfo rasterizerStateCI{};
    {// "PipelineRasterizationStateCreateInfo" �\���̂̐ݒ�
		rasterizerStateCI.sType		    = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizerStateCI.polygonMode   = k_FillMode[rasterizerDesc.fMode];
		rasterizerStateCI.cullMode	    = k_CullMode[rasterizerDesc.cMode];
		rasterizerStateCI.frontFace	    = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizerStateCI.lineWidth	    = 1.0f;
	}

    // �[�x�X�e�[�g�̐ݒ�
    VkPipelineDepthStencilStateCreateInfo depthStencilCI{};
    {// "PipelineDepthStencilStateCreateInfo" �\���̂̐ݒ�
        depthStencilCI.sType              = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilCI.depthTestEnable    = VK_TRUE;
        depthStencilCI.depthCompareOp     = VK_COMPARE_OP_LESS_OR_EQUAL;
        depthStencilCI.depthWriteEnable   = VK_TRUE;
        depthStencilCI.stencilTestEnable  = VK_FALSE;
    }

    // ���I�Ƀr���[�|�[�g�ƃV�U�[���N�g��ݒ肷��
    vector<VkDynamicState> dynamicStates
    {
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_VIEWPORT,
    };
    
    VkPipelineDynamicStateCreateInfo dynamicStateCI{};
    {// "PipelineDynamicStateCreateInfo" �\���̂̐ݒ�
        dynamicStateCI.sType                = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateCI.dynamicStateCount    = uint32_t(dynamicStates.size());
        dynamicStateCI.pDynamicStates       = dynamicStates.data();
    }

    VkGraphicsPipelineCreateInfo graphicsPipelineCI{};
	{// "GraphicsPipelineCreateInfo" �\���̂̐ݒ�
		graphicsPipelineCI.sType				= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		graphicsPipelineCI.stageCount			= uint32_t(shaderStages.size());
		graphicsPipelineCI.pStages				= shaderStages.data();
		graphicsPipelineCI.pInputAssemblyState	= &inputAssemblyCI;
		graphicsPipelineCI.pVertexInputState	= &vertexInputStateCI;
		graphicsPipelineCI.pRasterizationState	= &rasterizerStateCI;
		graphicsPipelineCI.pDepthStencilState	= &depthStencilCI;
		graphicsPipelineCI.pMultisampleState	= &multisampleCI;
		graphicsPipelineCI.pViewportState		= &viewportCI;
		graphicsPipelineCI.pColorBlendState		= &colorBlendCI;
        graphicsPipelineCI.pDynamicState        = &dynamicStateCI;
		graphicsPipelineCI.layout				= pipelineLayout;
        graphicsPipelineCI.renderPass           = m_renderPass;
	}

    // �`��p�C�v���C���̐���
    VkPipeline pl{};
    ret = vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &graphicsPipelineCI, nullptr, &pl);

    for (auto& s : shaderStages)
        vkDestroyShaderModule(m_device, s.module, nullptr);
#pragma endregion

    
    return new VulkanPipeline(
        pl,
        pipelineLayout,
        shaderResourceSets,
        textureResourceSet,
        samplerResourceSet,
        { shaderResourceSetLayout, textureResourceSetLayout, samplerResourceSetLayout},
        { shaderResourcePool, textureResourcePool, samplerResourcePool },
        this
    );
}
void VulkanGraphics::SetPipeline(IPipeline* pipeline)
{
    VulkanPipeline* pl = (VulkanPipeline*)pipeline;
    vkCmdBindPipeline(m_commands[m_imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pl->GetPipeline());

    // �v���~�e�B�u�g�|���W�[�̐ݒ�
    //vkCmdSetPrimitiveTopology(m_commands[m_imageIndex], k_PrimitiveMode[primitive]);

    vector<VkDescriptorSet> descriptorSets = pl->GetResourceDescriptor(m_imageIndex);
    vkCmdBindDescriptorSets(
        m_commands[m_imageIndex],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pl->GetPipelineLayout(),
        0,
        uint32_t(descriptorSets.size()),
        descriptorSets.data(),
        0,
        nullptr
    );
}

/* �V�F�[�_���\�[�X�֘A */
IShaderResource* VulkanGraphics::CreateShaderReource(size_t size, unsigned int slotNum)
{
    return new VulkanShaderResource(uint32_t(size), uint32_t(slotNum), this);
}
ITexture* VulkanGraphics::CreateTextureFromFile(const char* fileName, unsigned int slotNum)
{
    return new VulkanTexture(fileName, uint32_t(slotNum), this);
}

ISampler* VulkanGraphics::CreateSampler(sampler::SAMPLER_DESC samplerDesc, unsigned int binding)
{
    return new VulkanSampler(samplerDesc, uint32_t(binding), this);
}

DirectX::XMMATRIX VulkanGraphics::CulculateWorldMatrix(const DirectX::XMFLOAT3 position, const DirectX::XMFLOAT3 rotate, const DirectX::XMFLOAT3 scale)
{
    DirectX::XMMATRIX trl, rot, scl;

    trl = DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&position));
    rot = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotate));
    scl = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&scale));

    return scl * rot * trl;
}

DirectX::XMMATRIX VulkanGraphics::CulculateViewMatrix()
{
    return DirectX::XMMatrixLookAtLH(
        XMLoadFloat3(&m_camera.position),
        XMLoadFloat3(&m_camera.focus),
        XMLoadFloat3(&m_camera.up)
    );
}

DirectX::XMMATRIX VulkanGraphics::CulculateProjectionMatrix(bool perspective)
{
    if (perspective)
    {
        return DirectX::XMMatrixPerspectiveFovLH(
            m_fov,
            m_aspect,
            m_nearZ,
            m_farZ
        );
    }

    return DirectX::XMMatrixOrthographicOffCenterRH(
        0.0f,
        k_SCREEN_UI_WIDTH,
        k_SCREEN_UI_HEIGHT,
        0.0f,
        0.0f,
        1.0f
    );
}

DirectX::XMMATRIX VulkanGraphics::Multiply(DirectX::XMMATRIX parentMat, DirectX::XMMATRIX localMat)
{
    return localMat * parentMat;
}

/* �`��R�[�� */
void VulkanGraphics::DrawIndexed(unsigned int numIndex, unsigned int numInstance)
{
    vkCmdDrawIndexed(m_commands[m_imageIndex], numIndex, numInstance, 0, 0, 0);
}

void VulkanGraphics::DrawInstanced(unsigned int numVertex, unsigned int numInstance)
{
    vkCmdDraw(m_commands[m_imageIndex], numVertex, numInstance, 0, 0);
}

void VulkanGraphics::Resize(int width, int height)
{
    // �T�C�Y���s�K��
    if (width <= 0 || height <= 0)
        return;

    // �����҂�
    vkDeviceWaitIdle(m_device);

    // �X���b�v�`�F�[���̍�蒼��
    createSurface(nullptr);
    createSwapchain(m_swapchain);

    // �[�x�o�b�t�@�̔j��
    vkFreeMemory(m_device, m_depthImageMemory, nullptr);
    vkDestroyImage(m_device, m_depthImage, nullptr);
    vkDestroyImageView(m_device, m_depthImageView, nullptr);

    // �t���[���o�b�t�@�̔j��
    for (auto& f : m_frameBuffers)
        vkDestroyFramebuffer(m_device, f, nullptr);


    // �[�x�o�b�t�@�̍Đ���
    createDepthBuffer();

    // �t���[���o�b�t�@�̍Đ���
    createFrameBuffer();
}

bool VulkanGraphics::map(VkDeviceMemory memory, void* data, size_t size)
{
    VkResult ret{};

    // �������ւ̏�������
    void* map;
    ret = vkMapMemory(m_device, memory, 0, VK_WHOLE_SIZE, 0, &map);
    if (VFAILED(ret))
        return false;

    memcpy(map, data, size);
    vkUnmapMemory(m_device, memory);

    return true;
}

graphics::BUFFER_OBJECT VulkanGraphics::createBufferObject(uint32_t size, VkBufferUsageFlagBits usageFlag, VkMemoryPropertyFlags memoryFlags)
{
    VkResult ret{};

    graphics::BUFFER_OBJECT object{};
    VkBufferCreateInfo bufferCI{};
    {// "BufferCreateInfo" �\���̂̐ݒ�
        bufferCI.sType  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCI.usage  = usageFlag;
        bufferCI.size   = size;
    }

    // �o�b�t�@�̍쐬
    ret = vkCreateBuffer(m_device, &bufferCI, nullptr, &object.buffer);
    if (VFAILED(ret))
        throw "�o�b�t�@�̐����Ɏ��s";

    VkMemoryRequirements request;
    vkGetBufferMemoryRequirements(m_device, object.buffer, &request);

    VkMemoryAllocateInfo memoryAI{};
    {// "MemoryAllocateInfo" �\���̂̐ݒ�
        memoryAI.sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAI.allocationSize     = request.size;
        memoryAI.memoryTypeIndex    = this->getMemoryType(request.memoryTypeBits, memoryFlags);
    }

    // �������̊m��
    ret = vkAllocateMemory(m_device, &memoryAI, nullptr, &object.memory);
    if (VFAILED(ret))
        throw "�������̊m�ۂɎ��s";

    // �������̃o�C���h
    ret = vkBindBufferMemory(m_device, object.buffer, object.memory, 0);
    if (VFAILED(ret))
        throw "�������̊��蓖�ĂɎ��s";

    return object;
}

void VulkanGraphics::destroyBufferObject(graphics::BUFFER_OBJECT& bufferObject)
{
    // �������̉��
    vkFreeMemory(m_device, bufferObject.memory, nullptr);

    // �o�b�t�@�̔j��
    vkDestroyBuffer(m_device, bufferObject.buffer, nullptr);
}

graphics::TEXTURE_OBJECT VulkanGraphics::createTextureObject(
    int width, int height, int channel, VkFormat format, unsigned char* meta
)
{
    using namespace graphics;
    
    VkResult ret{};

    BUFFER_OBJECT stagingBuffer{};
    TEXTURE_OBJECT object{};

    VkImageCreateInfo imageCI{};
    {// "ImageCreateInfo" �\���̂̐ݒ�
        imageCI.sType       = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCI.extent      = { uint32_t(width), uint32_t(height), 1 };
        imageCI.format      = format;
        imageCI.imageType   = VK_IMAGE_TYPE_2D;
        imageCI.arrayLayers = 1;
        imageCI.mipLevels   = 1;
        imageCI.samples     = VK_SAMPLE_COUNT_1_BIT;
        imageCI.usage       = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    }

    // �C���[�W�̍쐬
    ret = vkCreateImage(m_device, &imageCI, nullptr, &object.image);
    if (VFAILED(ret))
        return object;

    // �������^�C�v��T�C�Y�̏��擾
    VkMemoryRequirements request{};
    vkGetImageMemoryRequirements(m_device, object.image, &request);

    VkMemoryAllocateInfo memoryAI{};
    {// "MemoryAllocateInfo" �\���̂̐ݒ�
        memoryAI.sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAI.allocationSize     = request.size;
        memoryAI.memoryTypeIndex    = getMemoryType(request.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    }

    // �������̊m��
    ret = vkAllocateMemory(m_device, &memoryAI, nullptr, &object.memory);
    if (VFAILED(ret))
        return object;

    // �������̊��蓖��
    ret = vkBindImageMemory(m_device, object.image, object.memory, 0);
    if (VFAILED(ret))
        return object;

    {// �o�b�t�@�I�u�W�F�N�g���쐬(����CPU���珑�����߂Ȃ��̂ŁA�o�b�t�@��ʂ��ď�������)
        uint32_t imageSize = uint32_t(width) * uint32_t(height) * uint32_t(channel);
        stagingBuffer = this->createBufferObject(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        this->map(stagingBuffer.memory, meta, imageSize);
    }

    VkBufferImageCopy bufferImageCopy{};
    {// �e�N�X�`���̏���ݒ�
        bufferImageCopy.imageExtent         = { uint32_t(width), uint32_t(height) };
        bufferImageCopy.imageSubresource    = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
    }

    VkCommandBuffer command{};
    VkCommandBufferAllocateInfo commandBufferAI{};
    {// "CommandBufferAllocateInfo" �\���̂̐ݒ�
        commandBufferAI.sType               = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAI.commandBufferCount  = 1;
        commandBufferAI.commandPool         = m_commandPool;
        commandBufferAI.level               = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    }

    // �������̊��蓖��
    vkAllocateCommandBuffers(m_device, &commandBufferAI, &command);

    VkCommandBufferBeginInfo commandBI{};
    commandBI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(command, &commandBI);
    {// �������ւ̃R�s�[�����s
        setImageMemoryBarrier(
            command,
            object.image,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        );

        vkCmdCopyBufferToImage(
            command,
            stagingBuffer.buffer,
            object.image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &bufferImageCopy);

        
        setImageMemoryBarrier(
            command,
            object.image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );
    }
    vkEndCommandBuffer(command);

    VkSubmitInfo submitInfo{};
    {// "SubmitInfo" �\���̂̐ݒ�
        submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount   = 1;
        submitInfo.pCommandBuffers      = &command;
    }

    // �R�}���h�̎��s
    vkQueueSubmit(m_deviceQueue, 1, &submitInfo, VK_NULL_HANDLE);

    VkImageViewCreateInfo imageViewCI{};
    {// "ImageViewCreateInfo" �\���̂̐ݒ�
        imageViewCI.sType       = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCI.viewType    = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCI.image       = object.image;
        imageViewCI.format      = format;
        imageViewCI.components  = {
          VK_COMPONENT_SWIZZLE_R,
          VK_COMPONENT_SWIZZLE_G,
          VK_COMPONENT_SWIZZLE_B,
          VK_COMPONENT_SWIZZLE_A,
        };
        imageViewCI.subresourceRange = {
          VK_IMAGE_ASPECT_COLOR_BIT,0,1,0,1
        };
    }

    // �C���[�W�r���[�̍쐬
    vkCreateImageView(m_device, &imageViewCI, nullptr, &object.view);

    // �R�}���h�o�b�t�@�̉��
    vkDeviceWaitIdle(m_device);
    vkFreeCommandBuffers(m_device, m_commandPool, 1, &command);

    this->destroyBufferObject(stagingBuffer);

    return object;
}

void VulkanGraphics::destroyTextureObject(graphics::TEXTURE_OBJECT& textureObject)
{
    vkDestroyImage(m_device, textureObject.image, nullptr);
    vkDestroyImageView(m_device, textureObject.view, nullptr);
    vkFreeMemory(m_device, textureObject.memory, nullptr);
}

VkPipelineShaderStageCreateInfo VulkanGraphics::loadShaderModule(const char* fileName, VkShaderStageFlagBits stage, const char* entryPoint)
{
    common::FileIO bin(fileName);
    string buffer = bin.GetInfo();

    VkShaderModule shaderModule{};
    VkShaderModuleCreateInfo shaderModuleCI{};
    {// "ShaderModuleCreateInfo" �\���̂̐ݒ�
        shaderModuleCI.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCI.codeSize = buffer.size();
        shaderModuleCI.pCode    = (uint32_t*)buffer.data();
    }

    VkResult ret{};
    ret = vkCreateShaderModule(m_device, &shaderModuleCI, nullptr, &shaderModule);
    if (VFAILED(ret))
        return VkPipelineShaderStageCreateInfo();

    VkPipelineShaderStageCreateInfo pipelineShaderStageCI{};
    {// "PipelineShaderStageCreateInfo" �\���̂̐ݒ�
        pipelineShaderStageCI.sType     = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        pipelineShaderStageCI.stage     = stage;
        pipelineShaderStageCI.module    = shaderModule;
        pipelineShaderStageCI.pName     = entryPoint;
    }
    return pipelineShaderStageCI;
}

#pragma region Vulkan�v���C�x�[�g���\�b�h

uint32_t VulkanGraphics::getMemoryType(uint32_t requestBits, VkMemoryPropertyFlags requestProperty)
{
    uint32_t ret = ~0u;
    for (uint32_t i = 0; i < m_physicalDeviceMemProps.memoryTypeCount; ++i)
    {
        if (requestBits & 1)
        {
            const auto& types = m_physicalDeviceMemProps.memoryTypes[i];
            if ((types.propertyFlags & requestProperty) == requestProperty)
            {
                ret = i;
                break;
            }
        }
        requestBits >>= 1;
    }

    return ret;
}

VkResult VulkanGraphics::createInstance()
{
    VkResult ret{};

    std::vector<const char*> extensionNames;
    vector<VkExtensionProperties> properties;
    {// �g�p�\�Ȋg�������擾����      
        uint32_t extensionsCount{};
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);
        properties.resize(extensionsCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, properties.data());

        for (const auto& p : properties)
            extensionNames.push_back(p.extensionName);
    }

    VkApplicationInfo appInfo{};
    {// "ApplicationInfo" �\���̂̐ݒ�
        appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName    = k_APP_NAME;
        appInfo.apiVersion          = VK_API_VERSION_1_3;
        appInfo.applicationVersion  = VK_MAKE_VERSION(1, 0, 0);
    }

    VkInstanceCreateInfo instCI{};
    {// "InstanceCreateInfo"�@�\���̂̐ݒ�
        instCI.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instCI.pApplicationInfo         = &appInfo;
        instCI.enabledExtensionCount    = uint32_t(extensionNames.size());
        instCI.ppEnabledExtensionNames  = extensionNames.data();
    }

    // �C���X�^���X�̐�������
    ret = vkCreateInstance(&instCI, nullptr, &m_instance);
    return ret;
}

VkResult VulkanGraphics::createPhysicalDevice()
{
    VkResult ret{};

    vector<VkPhysicalDevice> device;
    {// �����f�o�C�X���̎擾
        uint32_t physicalDeviceCount{};
        vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, nullptr);
        device.resize(physicalDeviceCount);
        vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, device.data());
    }

    // �ŏ��Ɍ��������f�o�C�X���g�p����
    m_physicalDevice = device[0];

    // �g�p�ł��郁�����̏����擾����
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_physicalDeviceMemProps);

    vector<VkQueueFamilyProperties> queueFamilyProps;
    {// �����f�o�C�X�̃L���[ �t�@�~�����擾����
        uint32_t propCount{};
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &propCount, nullptr);
        queueFamilyProps.resize(propCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &propCount, queueFamilyProps.data());
    }

    // �`��p�̃L���[��T��
    m_graphicsQueueIndex = ~0u;
    for (uint32_t i = 0; i < queueFamilyProps.size(); ++i)
    {
        if (queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            m_graphicsQueueIndex = i;
            break;
        }
    }

    // �`��p�̃L���[��������Ȃ������ꍇ
    if (m_graphicsQueueIndex >= ~0u)
        return VK_ERROR_UNKNOWN;

    return VK_SUCCESS;
}

VkResult VulkanGraphics::createDevice()
{
    VkResult ret{};
    const float defaultQueuePriority(1.0f);
    VkDeviceQueueCreateInfo deviceQCI{};
    {// "DeviceQueueCreateInfo" �\���̂̐ݒ�
        deviceQCI.sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQCI.queueFamilyIndex  = m_graphicsQueueIndex;
        deviceQCI.pQueuePriorities  = &defaultQueuePriority;
        deviceQCI.queueCount        = 1;
    }

    vector<const char*>extensionNames;
    vector<VkExtensionProperties> deviceExtensionsProps{};
    {// �g�����̎擾
        uint32_t extensionCount{};
        vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, nullptr);
        deviceExtensionsProps.resize(extensionCount);
        vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, deviceExtensionsProps.data());
    }

    for (const auto& p : deviceExtensionsProps)
        extensionNames.push_back(p.extensionName);

    VkDeviceCreateInfo deviceCI{};
    {// "DeviceCreateInfo" �\���̂̐ݒ�
        deviceCI.sType                      = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCI.pQueueCreateInfos          = &deviceQCI;
        deviceCI.queueCreateInfoCount       = 1;
        deviceCI.ppEnabledExtensionNames    = extensionNames.data();
        deviceCI.enabledExtensionCount      = uint32_t(extensionNames.size());
    }

    ret = vkCreateDevice(m_physicalDevice, &deviceCI, nullptr, &m_device);
    if (VFAILED(ret))
        return ret;

    // �f�o�C�X�̃L���[�̎擾
    vkGetDeviceQueue(m_device, m_graphicsQueueIndex, 0, &m_deviceQueue);


    VkCommandPoolCreateInfo commandPoolCI{};
    {// "CommandPoolCreateInfo" �\���̂̐ݒ�
        commandPoolCI.sType             = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCI.queueFamilyIndex  = m_graphicsQueueIndex;
        commandPoolCI.flags             = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    }

    // �R�}���h�v�[���̍쐬
    ret = vkCreateCommandPool(m_device, &commandPoolCI, nullptr, &m_commandPool);

    return ret;
}

VkResult VulkanGraphics::createSurface(HWND hWnd)
{
    VkResult ret{};

    if (hWnd)
    {
        VkWin32SurfaceCreateInfoKHR surfaceCI{};
        {// "Win32SurfaceCreateInfoKHR" �\���̂̐ݒ�
            surfaceCI.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            surfaceCI.hwnd = hWnd;
            surfaceCI.hinstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
        }

        // �T�[�t�F�[�X�̍쐬
        ret = vkCreateWin32SurfaceKHR(m_instance, &surfaceCI, nullptr, &m_surface);
        if (VFAILED(ret))
            return ret;
    }

    // �T�[�t�F�[�X�̏����擾
    ret = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_surfaceCap);
    if (VFAILED(ret))
        return ret;

    vector<VkSurfaceFormatKHR> formats;
    {// �T�[�t�F�[�X�̃t�H�[�}�b�g�����擾����
        uint32_t formatCount{};
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, nullptr);
        formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, formats.data());
    }

    for (const auto& f : formats)
    {
        if (f.format == VkFormat::VK_FORMAT_B8G8R8A8_UNORM)
        {
            m_surfaceFormat = f;
            break;
        }
    }

    // �g����t�H�[�}�b�g���Ȃ������ꍇ
    if (m_surfaceFormat.format == VkFormat::VK_FORMAT_UNDEFINED)
        return VK_ERROR_UNKNOWN;

    return ret;
}

VkResult VulkanGraphics::createSwapchain(VkSwapchainKHR oldSwapchain)
{
    VkResult ret{};

    uint32_t BUFFER_NUM = std::max(k_BUFFER_NUM, m_surfaceCap.minImageCount);
    VkExtent2D extent   = m_surfaceCap.currentExtent;

    uint32_t presentModeCount{};
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr);
    vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, presentModes.data());

    uint32_t queueFamilyIndex[]{ m_graphicsQueueIndex };
    VkSwapchainCreateInfoKHR swapchainCI{};
    {// "SwapchainCreateInfoKHR" �\���̂̐ݒ�
        swapchainCI.sType                   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCI.surface                 = m_surface;
        swapchainCI.minImageCount           = BUFFER_NUM;
        swapchainCI.imageFormat             = m_surfaceFormat.format;
        swapchainCI.imageColorSpace         = m_surfaceFormat.colorSpace;
        swapchainCI.imageExtent             = extent;
        swapchainCI.imageUsage              = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        swapchainCI.preTransform            = m_surfaceCap.currentTransform;
        swapchainCI.imageArrayLayers        = 1;
        swapchainCI.imageSharingMode        = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCI.queueFamilyIndexCount   = _countof(queueFamilyIndex);
        swapchainCI.pQueueFamilyIndices     = queueFamilyIndex;
        swapchainCI.presentMode             = VK_PRESENT_MODE_IMMEDIATE_KHR;
        swapchainCI.oldSwapchain            = oldSwapchain;
        swapchainCI.clipped                 = VK_TRUE;
        swapchainCI.compositeAlpha          = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    }

    // �X���b�v�`�F�C���̍쐬
    ret = vkCreateSwapchainKHR(m_device, &swapchainCI, nullptr, &m_swapchain);
    if (VFAILED(ret))
        return ret;

    // �Â����\�[�X�����.
    if (oldSwapchain != VK_NULL_HANDLE)
    {
        for (auto& view : m_swapchainViews)
            vkDestroyImageView(m_device, view, nullptr);

        vkDestroySwapchainKHR(m_device, oldSwapchain, nullptr);
        m_swapchainImages.clear();
        m_swapchainViews.clear();
    }

    m_swapchainImages.resize(BUFFER_NUM);
    vkGetSwapchainImagesKHR(m_device, m_swapchain, &BUFFER_NUM, m_swapchainImages.data());

    // �C���[�W�r���[�̍쐬
    m_swapchainViews.resize(BUFFER_NUM);
    for (uint32_t i = 0; i < BUFFER_NUM; ++i)
    {
        VkImageViewCreateInfo imageViewCI{};
        imageViewCI.sType       = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCI.viewType    = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCI.format      = m_surfaceFormat.format;
        imageViewCI.components  = {
            VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
            VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G,
            VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B,
            VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A,
        };
        imageViewCI.subresourceRange    = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        imageViewCI.image               = m_swapchainImages[i];

        ret = vkCreateImageView(m_device, &imageViewCI, nullptr, &m_swapchainViews[i]);
        if (VFAILED(ret))
            return ret;
    }

    float width     = float(m_surfaceCap.currentExtent.width);
    float height    = float(m_surfaceCap.currentExtent.height);

    {// "Viewport" �\���̂̐ݒ�
		m_viewport.x	    = 0.0f;
        m_viewport.y        = height;
		m_viewport.width    = width;
		m_viewport.height   = -height;
		m_viewport.minDepth = 0.0f;
		m_viewport.maxDepth = 1.0f;
	}

    {// "VkRect2D" �\���̂̐ݒ�
        m_scissor.offset = {};
        m_scissor.extent = m_surfaceCap.currentExtent;
    }

    // �A�X�y�N�g��̌v�Z
    m_aspect = width / height;

    return ret;
}

VkResult VulkanGraphics::createDepthBuffer()
{
    VkResult ret{};

    VkImageCreateInfo imageCI{};
    {// "ImageCreateInfo" �\���̂̐ݒ�
        imageCI.sType           = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCI.imageType       = VK_IMAGE_TYPE_2D;
        imageCI.format          = VK_FORMAT_D32_SFLOAT;
        imageCI.extent.width    = m_surfaceCap.currentExtent.width;
        imageCI.extent.height   = m_surfaceCap.currentExtent.height;
        imageCI.extent.depth    = 1;
        imageCI.mipLevels       = 1;
        imageCI.usage           = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        imageCI.samples         = VK_SAMPLE_COUNT_1_BIT;
        imageCI.arrayLayers     = 1;
    }

    // �C���h�o�b�t�@�p�C���[�W�̍쐬
    ret = vkCreateImage(m_device, &imageCI, nullptr, &m_depthImage);
    if (VFAILED(ret))
        return ret;

    // �C���[�W�p�̃����������擾����
    VkMemoryRequirements request{};
    vkGetImageMemoryRequirements(m_device, m_depthImage, &request);

    VkMemoryAllocateInfo memoryAI{};
    {// "MemoryAllocateInfo" �\���̂̐ݒ�
        memoryAI.sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAI.allocationSize     = request.size;
        memoryAI.memoryTypeIndex    = getMemoryType(request.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    }

    // �������̊m��
    ret = vkAllocateMemory(m_device, &memoryAI, nullptr, &m_depthImageMemory);
    if (VFAILED(ret))
        return ret;

    // �������̊��蓖��
    ret = vkBindImageMemory(m_device, m_depthImage, m_depthImageMemory, 0);
    if (VFAILED(ret))
        return ret;

    VkImageViewCreateInfo viewCI{};
    {// "ImageViewCreateInfo" �\���̂̐ݒ�
        viewCI.sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewCI.viewType     = VK_IMAGE_VIEW_TYPE_2D;
        viewCI.format       = imageCI.format;
        viewCI.components   = {
            VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
            VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G,
            VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B,
            VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A,
        };
        viewCI.subresourceRange = { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1 };
        viewCI.image            = m_depthImage;
    }

    // �C���[�W�r���[�̍쐬
    ret = vkCreateImageView(m_device, &viewCI, nullptr, &m_depthImageView);
    if (VFAILED(ret))
        return ret;

    return ret;
}

VkResult VulkanGraphics::createRenderPass()
{
    VkResult ret{};

    array<VkAttachmentDescription, 2> attachments{};
    {// ��ʃN���A�����̐ݒ�
        attachments[0].format           = m_surfaceFormat.format;
        attachments[0].samples          = VK_SAMPLE_COUNT_1_BIT;
        attachments[0].loadOp           = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[0].storeOp          = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[0].stencilLoadOp    = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[0].stencilStoreOp   = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[0].initialLayout    = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[0].finalLayout      = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }
    {// �[�x�o�b�t�@�����̐ݒ�
        attachments[1].format           = VK_FORMAT_D32_SFLOAT;
        attachments[1].samples          = VK_SAMPLE_COUNT_1_BIT;
        attachments[1].loadOp           = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[1].storeOp          = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].stencilLoadOp    = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[1].stencilStoreOp   = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[1].initialLayout    = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[1].finalLayout      = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    }

    VkAttachmentReference colorRef{}, depthRef{};
    {// "AttachmentReference" �\���̂̐ݒ�
        colorRef.attachment = 0;
        colorRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        depthRef.attachment = 1;
        depthRef.layout     = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    }

    VkSubpassDescription subpassDesc{};
    {// "SubpassDescription" �\���̂̐ݒ�
        subpassDesc.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDesc.colorAttachmentCount    = 1;
        subpassDesc.pColorAttachments       = &colorRef;
        subpassDesc.pDepthStencilAttachment = &depthRef;
    }

    VkRenderPassCreateInfo renderPassCI{};
    {// "RenderPassCreateInfo" �\���̂̐ݒ�
        renderPassCI.sType              = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCI.attachmentCount    = uint32_t(attachments.size());
        renderPassCI.pAttachments       = attachments.data();
        renderPassCI.subpassCount       = 1;
        renderPassCI.pSubpasses         = &subpassDesc;
    }

    // �����_�[�p�X�̐���
    ret = vkCreateRenderPass(m_device, &renderPassCI, nullptr, &m_renderPass);
    return ret;
}

VkResult VulkanGraphics::createFrameBuffer()
{
    VkResult ret{};

    VkFramebufferCreateInfo framebufferCI{};
    {// "FramebufferCreateInfo" �\���̂̐ݒ�
        framebufferCI.sType         = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCI.renderPass    = m_renderPass;
        framebufferCI.width         = m_surfaceCap.currentExtent.width;
        framebufferCI.height        = m_surfaceCap.currentExtent.height;
        framebufferCI.layers        = 1;
    }

    m_frameBuffers.resize(m_swapchainImages.size());

    for (size_t i = 0; i < m_frameBuffers.size(); ++i)
    {// �t���[���o�b�t�@�̍쐬
        array<VkImageView, 2> attachments{};
        attachments[0]                  = m_swapchainViews[i];
        attachments[1]                  = m_depthImageView;
        framebufferCI.attachmentCount   = uint32_t(attachments.size());
        framebufferCI.pAttachments      = attachments.data();

        ret = vkCreateFramebuffer(m_device, &framebufferCI, nullptr, &m_frameBuffers[i]);
        if (VFAILED(ret))
            return ret;
    }

    return ret;
}

VkResult VulkanGraphics::createFence()
{
    VkResult ret{};

    VkCommandBufferAllocateInfo commandBufferAI{};
    {// "CommandBufferAllocateInfo" �\���̂̐ݒ�
        commandBufferAI.sType               = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAI.commandPool         = m_commandPool;
        commandBufferAI.commandBufferCount  = uint32_t(m_swapchainViews.size());
        commandBufferAI.level               = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    }

    m_commands.resize(commandBufferAI.commandBufferCount);

    // �������̎擾
    ret = vkAllocateCommandBuffers(m_device, &commandBufferAI, m_commands.data());
    if (VFAILED(ret))
        return ret;

    m_fences.resize(commandBufferAI.commandBufferCount);

    VkFenceCreateInfo fenceCI{};
    {// "FenceCreateInfo" �\���̂̐ݒ�
        fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        for (auto& f : m_fences)
        {
            // Create a fence
            ret = vkCreateFence(m_device, &fenceCI, nullptr, &f);
            if (VFAILED(ret))
                return ret;
        }
    }

    VkSemaphoreCreateInfo semaphoreCI{};
    {// "SemaphoreCreateInfo" �\���̂̐ݒ�
        semaphoreCI.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    }

    // �����_�����O�p�Z�}�t�H�̍쐬
    ret = vkCreateSemaphore(m_device, &semaphoreCI, nullptr, &m_renderSem);
    if (VFAILED(ret))
        return ret;

    // �o�b�t�@�؂�ւ��p�Z�}�t�H�̍쐬
    ret = vkCreateSemaphore(m_device, &semaphoreCI, nullptr, &m_presentSem);
    if (VFAILED(ret))
        return ret;

    return ret;
}

void VulkanGraphics::setImageMemoryBarrier(
    VkCommandBuffer command,
    VkImage image,
    VkImageLayout oldLayout,
    VkImageLayout newLayout
)
{
    VkImageMemoryBarrier imb{};
    {// "VkImageMemoryBarrier" �\���̂̐ݒ�
        imb.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imb.oldLayout           = oldLayout;
        imb.newLayout           = newLayout;
        imb.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imb.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imb.subresourceRange    = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        imb.image               = image;
    }
    
    VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

    switch (oldLayout)
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:
        imb.srcAccessMask = 0;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        imb.srcAccessMask   = VK_ACCESS_TRANSFER_WRITE_BIT;
        srcStage            = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;
    }

    switch (newLayout)
    {
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        imb.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        imb.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        imb.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        break;
    }

    vkCmdPipelineBarrier(
        command,
        srcStage,
        dstStage,
        0,
        0,          // Memory barrier count
        nullptr,
        0,          // Buffer memory barrier count
        nullptr,
        1,          // Image memory barrier count
        &imb);
}

void VulkanGraphics::pipelineCreateSetLayoutAndCreatePool(
    std::vector<VulkanShaderResource*> shaderResources,
    VkDescriptorSetLayout& setLayout,
    VkDescriptorPool& pool
)
{
    VkResult ret{};

    vector<VkDescriptorSetLayoutBinding>    bindings;
    vector<VkDescriptorPoolSize>            poolsSizes;
    for (auto& vsr : shaderResources)
    {
        bindings.push_back(vsr->GetDescriptorSetLayoutBinding());
        poolsSizes.push_back(vsr->GetDescriptorPoolSize());
    }

    VkDescriptorSetLayoutCreateInfo shaderResourceSetLayoutCI{};
    {// "DescriptorSetLayoutCreateInfo" �\���̂̐ݒ�
        shaderResourceSetLayoutCI.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        shaderResourceSetLayoutCI.bindingCount  = uint32_t(bindings.size());
        shaderResourceSetLayoutCI.pBindings     = bindings.data();
    }

    ret = vkCreateDescriptorSetLayout(m_device, &shaderResourceSetLayoutCI, nullptr, &setLayout);
    if (VFAILED(ret))
        return;

     VkDescriptorPoolCreateInfo descriptorPoolCI{};
    {// "DescriptorPoolCreateInfo" �\���̂̐ݒ�
        descriptorPoolCI.sType           = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCI.maxSets         = uint32_t(k_BUFFER_NUM);
        descriptorPoolCI.poolSizeCount   = uint32_t(poolsSizes.size());
        descriptorPoolCI.pPoolSizes      = poolsSizes.data();
    }

    ret = vkCreateDescriptorPool(m_device, &descriptorPoolCI, nullptr, &pool);
}

void VulkanGraphics::pipelineDescriptorSet(
    std::vector<VulkanShaderResource*> shaderResources,
    VkDescriptorSetLayout setLayout,
    VkDescriptorPool pool,
    std::vector<VkDescriptorSet>& sets
)
{
    VkResult ret{};
    vector<VkDescriptorSetLayout> shaderResourceSetlayouts;
    for (unsigned int i = 0; i < k_BUFFER_NUM; ++i)
        shaderResourceSetlayouts.push_back(setLayout);

    VkDescriptorSetAllocateInfo descriptorSetAI{};
    {// "DescriptorSetAllocateInfo" �\���̂̐ݒ�
        descriptorSetAI.sType               = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAI.descriptorPool      = pool;
        descriptorSetAI.descriptorSetCount  = uint32_t(k_BUFFER_NUM);
        descriptorSetAI.pSetLayouts         = shaderResourceSetlayouts.data();
    }

    sets.resize(k_BUFFER_NUM);
    ret = vkAllocateDescriptorSets(m_device, &descriptorSetAI, sets.data());

    for (unsigned int i = 0; i < k_BUFFER_NUM; ++i)
    {
        vector<VkDescriptorBufferInfo>  bufferInfos(shaderResources.size());
        vector<VkWriteDescriptorSet>    writeSets(shaderResources.size());
        for (size_t j = 0; j < shaderResources.size(); ++j)
        {
            bufferInfos[j] = shaderResources[j]->GetDescriptorBufferInfo(i);
            VkWriteDescriptorSet uniformWDS = shaderResources[j]->GetWriteDescriptorSet();
            {// "WriteDescriptorSet" �\���̂̐ݒ�
                uniformWDS.pBufferInfo  = &bufferInfos[j];
                uniformWDS.dstSet       = sets[i];
            }
            writeSets[j] = uniformWDS;
        }
        vkUpdateDescriptorSets(m_device, uint32_t(writeSets.size()), writeSets.data(), 0, nullptr);
    }
}

void VulkanGraphics::pipelineCreateSetLayoutAndCreatePool(
    std::vector<VulkanTexture*> textureResources,
    VkDescriptorSetLayout& setLayout,
    VkDescriptorPool& pool
)
{
    VkResult ret{};

    vector<VkDescriptorSetLayoutBinding>    bindings;
    vector<VkDescriptorPoolSize>            poolsSizes;
    for (auto& vtr : textureResources)
    {
        bindings.push_back(vtr->GetDescriptorSetLayoutBinding());
        poolsSizes.push_back(vtr->GetDescriptorPoolSize());
    }

    VkDescriptorSetLayoutCreateInfo textureResourceSetLayoutCI{};
    {// "DescriptorSetLayoutCreateInfo" �\���̂̐ݒ�
        textureResourceSetLayoutCI.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        textureResourceSetLayoutCI.bindingCount  = uint32_t(bindings.size());
        textureResourceSetLayoutCI.pBindings     = bindings.data();
    }

    ret = vkCreateDescriptorSetLayout(m_device, &textureResourceSetLayoutCI, nullptr, &setLayout);
    if (VFAILED(ret))
        return;

    VkDescriptorPoolCreateInfo descriptorPoolCI{};
    {// "DescriptorPoolCreateInfo" �\���̂̐ݒ�
        descriptorPoolCI.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCI.maxSets        = uint32_t(k_BUFFER_NUM);
        descriptorPoolCI.poolSizeCount  = uint32_t(poolsSizes.size());
        descriptorPoolCI.pPoolSizes     = poolsSizes.data();
    }

    ret = vkCreateDescriptorPool(m_device, &descriptorPoolCI, nullptr, &pool);
}

VkDescriptorSet VulkanGraphics::pipelineDescriptorSet(
    std::vector<VulkanTexture*> textureResources,
    VkDescriptorSetLayout setLayout,
    VkDescriptorPool pool
)
{
    VkResult ret{};
    vector<VkDescriptorSetLayout> textureResourceSetlayouts;
    for (unsigned int i = 0; i < textureResources.size(); ++i)
        textureResourceSetlayouts.push_back(setLayout);

    VkDescriptorSetAllocateInfo descriptorSetAI{};
    {// "DescriptorSetAllocateInfo" �\���̂̐ݒ�
        descriptorSetAI.sType               = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAI.descriptorPool      = pool;
        descriptorSetAI.descriptorSetCount  = uint32_t(textureResourceSetlayouts.size());
        descriptorSetAI.pSetLayouts         = textureResourceSetlayouts.data();
    }

    VkDescriptorSet descriptorSet{};
    ret = vkAllocateDescriptorSets(m_device, &descriptorSetAI, &descriptorSet);

    vector<VkWriteDescriptorSet>    writeSets(textureResources.size());
    for (size_t j = 0; j < textureResources.size(); ++j)
    {
        VkWriteDescriptorSet textureWDS = textureResources[j]->GetWriteDescriptorSet();
        {// "WriteDescriptorSet" �\���̂̐ݒ�
            textureWDS.dstSet = descriptorSet;
        }
        writeSets[j] = textureWDS;
    }
    vkUpdateDescriptorSets(m_device, uint32_t(writeSets.size()), writeSets.data(), 0, nullptr);

    return descriptorSet;
}

void VulkanGraphics::pipelineCreateSetLayoutAndCreatePool(
    std::vector<VulkanSampler*> samplerResources,
    VkDescriptorSetLayout& setLayout,
    VkDescriptorPool& pool
)
{
    VkResult ret{};

    vector<VkDescriptorSetLayoutBinding>    bindings;
    vector<VkDescriptorPoolSize>            poolsSizes;
    for (auto& vsr : samplerResources)
    {
        bindings.push_back(vsr->GetDescriptorSetLayoutBinding());
        poolsSizes.push_back(vsr->GetDescriptorPoolSize());
    }

    VkDescriptorSetLayoutCreateInfo samplerResourceSetLayoutCI{};
    {// "DescriptorSetLayoutCreateInfo" �\���̂̐ݒ�
        samplerResourceSetLayoutCI.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        samplerResourceSetLayoutCI.bindingCount  = uint32_t(bindings.size());
        samplerResourceSetLayoutCI.pBindings     = bindings.data();
    }

    ret = vkCreateDescriptorSetLayout(m_device, &samplerResourceSetLayoutCI, nullptr, &setLayout);
    if (VFAILED(ret))
        return;

    VkDescriptorPoolCreateInfo descriptorPoolCI{};
    {// "DescriptorPoolCreateInfo" �\���̂̐ݒ�
        descriptorPoolCI.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCI.maxSets        = uint32_t(k_BUFFER_NUM);
        descriptorPoolCI.poolSizeCount  = uint32_t(poolsSizes.size());
        descriptorPoolCI.pPoolSizes     = poolsSizes.data();
    }

    ret = vkCreateDescriptorPool(m_device, &descriptorPoolCI, nullptr, &pool);
}

VkDescriptorSet VulkanGraphics::pipelineDescriptorSet(
    std::vector<VulkanSampler*> samplerResources,
    VkDescriptorSetLayout setLayout,
    VkDescriptorPool pool
)
{
    VkResult ret{};
    vector<VkDescriptorSetLayout> samplerResourceSetlayouts;
    for (unsigned int i = 0; i < samplerResources.size(); ++i)
        samplerResourceSetlayouts.push_back(setLayout);

    VkDescriptorSetAllocateInfo descriptorSetAI{};
    {// "DescriptorSetAllocateInfo" �\���̂̐ݒ�
        descriptorSetAI.sType               = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAI.descriptorPool      = pool;
        descriptorSetAI.descriptorSetCount  = uint32_t(samplerResourceSetlayouts.size());
        descriptorSetAI.pSetLayouts         = samplerResourceSetlayouts.data();
    }

    VkDescriptorSet descriptorSet{};
    ret = vkAllocateDescriptorSets(m_device, &descriptorSetAI, &descriptorSet);

    vector<VkWriteDescriptorSet>    writeSets(samplerResources.size());
    for (size_t j = 0; j < samplerResources.size(); ++j)
    {
        VkWriteDescriptorSet samplerWDS = samplerResources[j]->GetWriteDescriptorSet();
        {// "WriteDescriptorSet" �\���̂̐ݒ�
            samplerWDS.dstSet = descriptorSet;
        }
        writeSets[j] = samplerWDS;
    }
    vkUpdateDescriptorSets(m_device, uint32_t(writeSets.size()), writeSets.data(), 0, nullptr);

    return descriptorSet;
}
#pragma endregion
