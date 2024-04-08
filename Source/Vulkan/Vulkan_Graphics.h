/*===================================================================================
*    Date : 2022/11/28(��)
*        Author		: Gakuto.S
*        File		: Vulkan_Graphics.h
*        Detail		:
===================================================================================*/
#pragma once

// �}�N����`
#define VFAILED(r) (r != VK_SUCCESS)

// �E�B���h�E�Y�v���b�g�t�H�[��
#include "../Window_Win.h"

// Vulkan�̃C���N���[�h
#include <vulkan/vk_layer.h>
#include <vulkan/vulkan_win32.h>

// ���C�u�����̃����N
#pragma comment(lib, "vulkan-1.lib")

#include "../IF_Graphics.h"

class VulkanShaderResource;
class VulkanTexture;
class VulkanSampler;

namespace graphics
{
	//**************************************************
	/// \brief �o�b�t�@�\����
	//**************************************************
	struct BUFFER_OBJECT
	{
		VkBuffer		buffer;
		VkDeviceMemory	memory;
	};

	//**************************************************
	/// \brief �e�N�X�`���\����
	//**************************************************
	struct TEXTURE_OBJECT
	{
		VkImage         image;
		VkDeviceMemory  memory;
		VkImageView     view;
	};
}

class VulkanGraphics : public IGraphics
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	VulkanGraphics(int width, int height, WindowWin* window);

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~VulkanGraphics();


	////////	�p�����\�b�h	////////
public:
	//**************************************************
	/// \brief ��ʃN���A����
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Clear() override;

	//**************************************************
	/// \brief �o�b�t�@�؂�ւ�����
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Present() override;

	//**************************************************
	/// \brief ���_�o�b�t�@�̐�������
	/// 
	/// \param[in] size
	/// \param[in] stride
	/// 
	/// \return ���_�o�b�t�@�|�C���^
	//**************************************************
	IVertexBufferObject* CreateVertexBuffer(
		size_t size,
		size_t stride
	) override;

	//**************************************************
	/// \brief ���_�o�b�t�@�̐ݒ菈��
	/// 
	/// \param[in] vertexBuffer
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetVertexBuffer(IVertexBufferObject* vertexBuffer) override;

	//**************************************************
	/// \brief �C���f�b�N�X�o�b�t�@�̐�������
	/// 
	/// \param[in] size
	/// \param[in] countIndex
	/// \param[in] format
	/// 
	/// \return �C���f�b�N�X�o�b�t�@�|�C���^
	//**************************************************
	IElementBufferObject* CreateIndexBuffer(
		size_t size,
		unsigned int countIndex,
		INDEX_FORMAT format
	) override;

	//**************************************************
	/// \brief ���_�o�b�t�@�̐ݒ菈��
	/// 
	/// \param[in] elementBuffer
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetIndexBuffer(IElementBufferObject* elementBuffer) override;

	//**************************************************
	/// \brief �p�C�v���C���̍쐬����
	/// 
	/// \param[in] layouts
	/// \param[in] blendDesc
	/// \param[in] rasterizerDesc
	/// \param[in] shaderDesc
	/// 
	/// \return �p�C�v���C��
	//**************************************************
	IPipeline* CreatePipeline(
		size_t stride,
		std::vector<pipeline::vertexLayout::VERTEX_LAYOUT_DESC> layouts,
		pipeline::blend::BLEND_DESC blendDesc,
		pipeline::rasterizer::RASTERIZER_DESC rasterizerDesc,
		pipeline::primitive::PRIMITIVE_TOPOLOGY primitive,
		pipeline::shader::SHADER_DESC shaderDesc,
		std::vector<IShaderResource*> shaderResources,
		std::vector<ITexture*>  textureResources,
		std::vector<ISampler*>	samplerResources
	) override;

	//**************************************************
	/// \brief �p�C�v���C���̐ݒ菈��
	/// 
	/// \param[in] pipeline
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetPipeline(IPipeline* pipeline) override;

	//**************************************************
	/// \brief �萔�o�b�t�@�̍쐬����
	/// 
	/// \param[in] size
	/// \param[in] slotNum
	/// 
	/// \return �V�F�[�_���\�[�X
	//**************************************************
	IShaderResource* CreateShaderReource(
		size_t size,
		unsigned int slotNum
	) override;

	//**************************************************
	/// \brief �e�N�X�`���̍쐬����
	/// 
	/// \param[in] fileName
	/// 
	/// \return �e�N�X�`�����\�[�X
	//**************************************************
	ITexture* CreateTextureFromFile(
		const char* fileName,
		unsigned int slotNum
	) override;

	//**************************************************
	/// \brief �T���v���[�̍쐬����
	/// 
	/// \param[in] samplerDesc
	/// \param[in] slotNum
	/// 
	/// \return �T���v���[
	//**************************************************
	ISampler* CreateSampler(
		sampler::SAMPLER_DESC samplerDesc,
		unsigned int binding
	) override;

	//**************************************************
	/// \brief ���f���}�g���b�N�X�̍쐬
	/// 
	/// \param[in] position
	/// \param[in] rotate
	/// \param[in] scale
	/// 
	/// \return ���f���}�g���b�N�X
	//**************************************************
	DirectX::XMMATRIX CulculateWorldMatrix(
		const DirectX::XMFLOAT3 position,
		const DirectX::XMFLOAT3 rotate,
		const DirectX::XMFLOAT3 scale
	) override;

	//**************************************************
	/// \brief �J�����}�g���b�N�X�̍쐬
	/// 
	/// \return �J�����}�g���b�N�X
	//**************************************************
	DirectX::XMMATRIX CulculateViewMatrix() override;

	//**************************************************
	/// \brief �v���W�F�N�V�����}�g���b�N�X�̍쐬
	/// 
	/// \param[in] fov
	/// \param[in] nearZ
	/// \param[in] farZ
	/// 
	/// \return �v���W�F�N�V�����}�g���b�N�X
	//**************************************************
	DirectX::XMMATRIX CulculateProjectionMatrix(bool perspective = true) override;


	//**************************************************
	/// \brief �e�q�s��̏�Z�̍쐬
	/// 
	/// \param[in] parentMat
	/// \param[in] localMat
	/// 
	/// \return �s��
	//**************************************************
	virtual DirectX::XMMATRIX Multiply(
		DirectX::XMMATRIX parentMat,
		DirectX::XMMATRIX localMat
	) override;

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \param[in] numIndex
	/// \param[in] numInstance
	/// 
	/// \return �Ȃ�
	//**************************************************
	void DrawIndexed(
		unsigned int numIndex,
		unsigned int numInstance
	) override;

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \param[in] numVertex
	/// \param[in] numInstance
	/// 
	/// \return �Ȃ�
	//**************************************************
	void DrawInstanced(
		unsigned int numVertex,
		unsigned int numInstance
	) override;


	//**************************************************
	/// \brief ��ʃ��T�C�Y����
	/// 
	/// \param[in] width
	/// \param[in] height
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Resize(int width, int height) override;


	////////		Vulkan�Ɉˑ����郁�\�b�h�Q		////////
public:
	//**************************************************
	/// \brief �f�o�C�X�̎擾
	/// 
	/// \return VK_SUCCESS �Ȃ琬��
	//**************************************************
	VkDevice getDevice() { return m_device; }

	//**************************************************
	/// \brief ���݂̃o�b�t�@�C���f�b�N�X���擾
	/// 
	/// \return VK_SUCCESS �Ȃ琬��
	//**************************************************
	uint32_t getCurrentlyIndex() { return m_imageIndex; }

	//**************************************************
	/// \brief �}�b�v
	/// 
	/// \param[in] memory
	/// \param[in] data
	/// \param[in] size
	/// 
	/// \return true �Ȃ琬��
	//**************************************************
	bool map(VkDeviceMemory memory, void* data, size_t size);

	//**************************************************
	/// \brief �o�b�t�@����
	/// 
	/// \param[in] size
	/// \param[in] usageFlag
	/// \param[in] memoryFlags
	/// 
	/// \return BUFFER_OBJECT
	//**************************************************
	graphics::BUFFER_OBJECT createBufferObject(
		uint32_t size,
		VkBufferUsageFlagBits usageFlag,
		VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
	);

	//**************************************************
	/// \brief �o�b�t�@�j��
	/// 
	/// \param[in] bufferObject
	/// 
	/// \return �Ȃ�
	//**************************************************
	void destroyBufferObject(graphics::BUFFER_OBJECT& bufferObject);

	//**************************************************
	/// \brief �e�N�X�`���o�b�t�@�̍쐬
	/// 
	/// \param[in] width
	/// 
	/// \return �Ȃ�
	//**************************************************
	graphics::TEXTURE_OBJECT createTextureObject(
		int width,
		int height,
		int channe,
		VkFormat format,
		unsigned char* meta
	);

	//**************************************************
	/// \brief �e�N�X�`���o�b�t�@�j��
	/// 
	/// \param[in] textureBufferObject
	/// 
	/// \return �Ȃ�
	//**************************************************
	void destroyTextureObject(graphics::TEXTURE_OBJECT& textureObject);

	//**************************************************
	/// \brief �V�F�[�_���W���[���̍쐬
	/// 
	/// \param[in] fileName
	/// \param[in] stage
	/// \param[in] entryPoint
	/// 
	/// \return �V�F�[�_���W���[��
	//**************************************************
	VkPipelineShaderStageCreateInfo loadShaderModule(
		const char* fileName,
		VkShaderStageFlagBits stage,
		const char* entrypoint = "main"
	);

private:
	//**************************************************
	/// \brief �������^�C�v�̎擾
	/// 
	/// \param[in] requestBits
	/// \param[in] requestProperty
	/// 
	/// \return �������^�C�v�C���f�b�N�X
	//**************************************************
	uint32_t getMemoryType(
		uint32_t requestBits,
		VkMemoryPropertyFlags requestProperty
	);

	//**************************************************
	/// \brief �C���X�^���X�̐���
	/// 
	/// \return VK_SUCCESS �Ȃ琬��
	//**************************************************
	VkResult createInstance();

	//**************************************************
	/// \brief �����f�o�C�X�̐���
	/// 
	/// \return Success is VK_SUCCESS
	//**************************************************
	VkResult createPhysicalDevice();

	//**************************************************
	/// \brief �_���f�o�C�X�̐���
	/// 
	/// \return VK_SUCCESS �Ȃ琬��
	//**************************************************
	VkResult createDevice();

	//**************************************************
	/// \brief �T�[�t�F�[�X�̐���
	/// 
	/// \param[in] hWnd
	/// 
	/// \return VK_SUCCESS �Ȃ琬��
	//**************************************************
	VkResult createSurface(HWND hWnd);

	//**************************************************
	/// \brief �X���b�v�`�F�C���̐���
	/// 
	/// \param[in] oldSwapChain
	/// 
	/// \return VK_SUCCESS �Ȃ琬��
	//**************************************************
	VkResult createSwapchain(
		VkSwapchainKHR oldSwapChain
	);

	//**************************************************
	/// \brief �[�x�o�b�t�@�̐���
	/// 
	/// \return VK_SUCCESS �Ȃ琬��
	//**************************************************
	VkResult createDepthBuffer();

	//**************************************************
	/// \brief �����_�[�p�X�̐���
	/// 
	/// \return VK_SUCCESS �Ȃ琬��
	//**************************************************
	VkResult createRenderPass();

	//**************************************************
	/// \brief �t���[���o�b�t�@�̐���
	/// 
	/// \return VK_SUCCESS �Ȃ琬��
	//**************************************************
	VkResult createFrameBuffer();

	//**************************************************
	/// \brief �t�F���X�̐���
	/// 
	/// \return VK_SUCCESS �Ȃ琬��
	//**************************************************
	VkResult createFence();

	//**************************************************
	/// \brief �C���[�W�������o���A
	/// 
	/// \param[in] command
	/// \param[in] image
	/// \param[in] oldLayout
	/// \param[in] newLayout
	/// 
	/// \return none
	//**************************************************
	void setImageMemoryBarrier(
		VkCommandBuffer command,
		VkImage image,
		VkImageLayout oldLayout,
		VkImageLayout newLayout
	);

	//**************************************************
	/// \brief �V�F�[�_���\�[�X�̃��C�A�E�g�ƃv�[���̍쐬
	/// 
	/// \param[in] shaderResources
	/// \param[out] setLayout
	/// \param[out] pool
	/// 
	/// \return �Ȃ�
	//**************************************************
	void pipelineCreateSetLayoutAndCreatePool(
		std::vector<VulkanShaderResource*> shaderResources,
		VkDescriptorSetLayout& setLayout,
		VkDescriptorPool& pool
	);

	//**************************************************
	/// \brief �V�F�[�_���\�[�X�̃f�X�N���v�^��ݒ�
	/// 
	/// \param[in] shaderResources
	/// \param[in] setLayout
	/// \param[in] pool
	/// \param[out] sets
	/// 
	/// \return �Ȃ�
	//**************************************************
	void pipelineDescriptorSet(
		std::vector<VulkanShaderResource*> shaderResources,
		VkDescriptorSetLayout setLayout,
		VkDescriptorPool pool,
		std::vector<VkDescriptorSet>& sets
	);

	//**************************************************
	/// \brief �e�N�X�`�����\�[�X�̃o�C���f�B���O���擾
	/// 
	/// \param[in] textureResources
	/// \param[out] setLayout
	/// \param[out] pool
	/// 
	/// \return �Ȃ�
	//**************************************************
	void pipelineCreateSetLayoutAndCreatePool(
		std::vector<VulkanTexture*> textureResources,
		VkDescriptorSetLayout& setLayout,
		VkDescriptorPool& pool
	);

	//**************************************************
	/// \brief �e�N�X�`�����\�[�X�̃f�X�N���v�^��ݒ�
	/// 
	/// \param[in] textureResources
	/// \param[in] setLayout
	/// \param[in] pool
	/// \param[out] set
	/// 
	/// \return �Ȃ�
	//**************************************************
	VkDescriptorSet pipelineDescriptorSet(
		std::vector<VulkanTexture*> textureResources,
		VkDescriptorSetLayout setLayout,
		VkDescriptorPool pool
	);

	//**************************************************
	/// \brief �T���v���[���\�[�X�̃o�C���f�B���O���擾
	/// 
	/// \param[in] shaderResources
	/// \param[out] setLayout
	/// \param[out] pool
	/// 
	/// \return �Ȃ�
	//**************************************************
	void pipelineCreateSetLayoutAndCreatePool(
		std::vector<VulkanSampler*> samplerResources,
		VkDescriptorSetLayout& setLayout,
		VkDescriptorPool& pool
	);

	//**************************************************
	/// \brief �T���v���[���\�[�X�̃f�X�N���v�^��ݒ�
	/// 
	/// \param[in] samplerResources
	/// \param[in] setLayout
	/// \param[in] pool
	/// \param[out] set
	/// 
	/// \return �Ȃ�
	//**************************************************
	VkDescriptorSet pipelineDescriptorSet(
		std::vector<VulkanSampler*> samplerResources,
		VkDescriptorSetLayout setLayout,
		VkDescriptorPool pool
	);

private:
	VkInstance                          m_instance;
	VkPhysicalDevice                    m_physicalDevice;
	VkPhysicalDeviceMemoryProperties    m_physicalDeviceMemProps;
	uint32_t                            m_graphicsQueueIndex;
	VkDevice                            m_device;
	VkQueue                             m_deviceQueue;
	VkCommandPool                       m_commandPool;

	VkSurfaceKHR                        m_surface;
	VkSurfaceCapabilitiesKHR            m_surfaceCap;
	VkSurfaceFormatKHR                  m_surfaceFormat;

	// �X���b�v�`�F�C��
	VkSwapchainKHR                      m_swapchain;
	std::vector<VkImage>                m_swapchainImages;
	std::vector<VkImageView>            m_swapchainViews;

	// �[�x�o�b�t�@
	VkImage                             m_depthImage;
	VkImageView                         m_depthImageView;
	VkDeviceMemory                      m_depthImageMemory;

	VkRenderPass                        m_renderPass;
	std::vector<VkFramebuffer>          m_frameBuffers;

	std::vector<VkCommandBuffer>        m_commands;

	// �t�F���X
	std::vector<VkFence>                m_fences;
	VkSemaphore                         m_renderSem;
	VkSemaphore                         m_presentSem;

	// �r���[�|�[�g�E�V�U�[���N�g
	VkViewport							m_viewport;
	VkRect2D							m_scissor;

	// ���݂̃X���b�v�`�F�C���C���[�W�̎擾
	uint32_t                            m_imageIndex;

private:
	const char* k_APP_NAME = "Vulkan";

};

