/*===================================================================================
*    Date : 2022/11/28(月)
*        Author		: Gakuto.S
*        File		: Vulkan_Graphics.h
*        Detail		:
===================================================================================*/
#pragma once

// マクロ定義
#define VFAILED(r) (r != VK_SUCCESS)

// ウィンドウズプラットフォーム
#include "../Window_Win.h"

// Vulkanのインクルード
#include <vulkan/vk_layer.h>
#include <vulkan/vulkan_win32.h>

// ライブラリのリンク
#pragma comment(lib, "vulkan-1.lib")

#include "../IF_Graphics.h"

class VulkanShaderResource;
class VulkanTexture;
class VulkanSampler;

namespace graphics
{
	//**************************************************
	/// \brief バッファ構造体
	//**************************************************
	struct BUFFER_OBJECT
	{
		VkBuffer		buffer;
		VkDeviceMemory	memory;
	};

	//**************************************************
	/// \brief テクスチャ構造体
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
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	VulkanGraphics(int width, int height, WindowWin* window);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~VulkanGraphics();


	////////	継承メソッド	////////
public:
	//**************************************************
	/// \brief 画面クリア処理
	/// 
	/// \return なし
	//**************************************************
	void Clear() override;

	//**************************************************
	/// \brief バッファ切り替え処理
	/// 
	/// \return なし
	//**************************************************
	void Present() override;

	//**************************************************
	/// \brief 頂点バッファの生成処理
	/// 
	/// \param[in] size
	/// \param[in] stride
	/// 
	/// \return 頂点バッファポインタ
	//**************************************************
	IVertexBufferObject* CreateVertexBuffer(
		size_t size,
		size_t stride
	) override;

	//**************************************************
	/// \brief 頂点バッファの設定処理
	/// 
	/// \param[in] vertexBuffer
	/// 
	/// \return なし
	//**************************************************
	void SetVertexBuffer(IVertexBufferObject* vertexBuffer) override;

	//**************************************************
	/// \brief インデックスバッファの生成処理
	/// 
	/// \param[in] size
	/// \param[in] countIndex
	/// \param[in] format
	/// 
	/// \return インデックスバッファポインタ
	//**************************************************
	IElementBufferObject* CreateIndexBuffer(
		size_t size,
		unsigned int countIndex,
		INDEX_FORMAT format
	) override;

	//**************************************************
	/// \brief 頂点バッファの設定処理
	/// 
	/// \param[in] elementBuffer
	/// 
	/// \return なし
	//**************************************************
	void SetIndexBuffer(IElementBufferObject* elementBuffer) override;

	//**************************************************
	/// \brief パイプラインの作成処理
	/// 
	/// \param[in] layouts
	/// \param[in] blendDesc
	/// \param[in] rasterizerDesc
	/// \param[in] shaderDesc
	/// 
	/// \return パイプライン
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
	/// \brief パイプラインの設定処理
	/// 
	/// \param[in] pipeline
	/// 
	/// \return なし
	//**************************************************
	void SetPipeline(IPipeline* pipeline) override;

	//**************************************************
	/// \brief 定数バッファの作成処理
	/// 
	/// \param[in] size
	/// \param[in] slotNum
	/// 
	/// \return シェーダリソース
	//**************************************************
	IShaderResource* CreateShaderReource(
		size_t size,
		unsigned int slotNum
	) override;

	//**************************************************
	/// \brief テクスチャの作成処理
	/// 
	/// \param[in] fileName
	/// 
	/// \return テクスチャリソース
	//**************************************************
	ITexture* CreateTextureFromFile(
		const char* fileName,
		unsigned int slotNum
	) override;

	//**************************************************
	/// \brief サンプラーの作成処理
	/// 
	/// \param[in] samplerDesc
	/// \param[in] slotNum
	/// 
	/// \return サンプラー
	//**************************************************
	ISampler* CreateSampler(
		sampler::SAMPLER_DESC samplerDesc,
		unsigned int binding
	) override;

	//**************************************************
	/// \brief モデルマトリックスの作成
	/// 
	/// \param[in] position
	/// \param[in] rotate
	/// \param[in] scale
	/// 
	/// \return モデルマトリックス
	//**************************************************
	DirectX::XMMATRIX CulculateWorldMatrix(
		const DirectX::XMFLOAT3 position,
		const DirectX::XMFLOAT3 rotate,
		const DirectX::XMFLOAT3 scale
	) override;

	//**************************************************
	/// \brief カメラマトリックスの作成
	/// 
	/// \return カメラマトリックス
	//**************************************************
	DirectX::XMMATRIX CulculateViewMatrix() override;

	//**************************************************
	/// \brief プロジェクションマトリックスの作成
	/// 
	/// \param[in] fov
	/// \param[in] nearZ
	/// \param[in] farZ
	/// 
	/// \return プロジェクションマトリックス
	//**************************************************
	DirectX::XMMATRIX CulculateProjectionMatrix(bool perspective = true) override;


	//**************************************************
	/// \brief 親子行列の乗算の作成
	/// 
	/// \param[in] parentMat
	/// \param[in] localMat
	/// 
	/// \return 行列
	//**************************************************
	virtual DirectX::XMMATRIX Multiply(
		DirectX::XMMATRIX parentMat,
		DirectX::XMMATRIX localMat
	) override;

	//**************************************************
	/// \brief 描画処理
	/// 
	/// \param[in] numIndex
	/// \param[in] numInstance
	/// 
	/// \return なし
	//**************************************************
	void DrawIndexed(
		unsigned int numIndex,
		unsigned int numInstance
	) override;

	//**************************************************
	/// \brief 描画処理
	/// 
	/// \param[in] numVertex
	/// \param[in] numInstance
	/// 
	/// \return なし
	//**************************************************
	void DrawInstanced(
		unsigned int numVertex,
		unsigned int numInstance
	) override;


	//**************************************************
	/// \brief 画面リサイズ処理
	/// 
	/// \param[in] width
	/// \param[in] height
	/// 
	/// \return なし
	//**************************************************
	void Resize(int width, int height) override;


	////////		Vulkanに依存するメソッド群		////////
public:
	//**************************************************
	/// \brief デバイスの取得
	/// 
	/// \return VK_SUCCESS なら成功
	//**************************************************
	VkDevice getDevice() { return m_device; }

	//**************************************************
	/// \brief 現在のバッファインデックスを取得
	/// 
	/// \return VK_SUCCESS なら成功
	//**************************************************
	uint32_t getCurrentlyIndex() { return m_imageIndex; }

	//**************************************************
	/// \brief マップ
	/// 
	/// \param[in] memory
	/// \param[in] data
	/// \param[in] size
	/// 
	/// \return true なら成功
	//**************************************************
	bool map(VkDeviceMemory memory, void* data, size_t size);

	//**************************************************
	/// \brief バッファ生成
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
	/// \brief バッファ破棄
	/// 
	/// \param[in] bufferObject
	/// 
	/// \return なし
	//**************************************************
	void destroyBufferObject(graphics::BUFFER_OBJECT& bufferObject);

	//**************************************************
	/// \brief テクスチャバッファの作成
	/// 
	/// \param[in] width
	/// 
	/// \return なし
	//**************************************************
	graphics::TEXTURE_OBJECT createTextureObject(
		int width,
		int height,
		int channe,
		VkFormat format,
		unsigned char* meta
	);

	//**************************************************
	/// \brief テクスチャバッファ破棄
	/// 
	/// \param[in] textureBufferObject
	/// 
	/// \return なし
	//**************************************************
	void destroyTextureObject(graphics::TEXTURE_OBJECT& textureObject);

	//**************************************************
	/// \brief シェーダモジュールの作成
	/// 
	/// \param[in] fileName
	/// \param[in] stage
	/// \param[in] entryPoint
	/// 
	/// \return シェーダモジュール
	//**************************************************
	VkPipelineShaderStageCreateInfo loadShaderModule(
		const char* fileName,
		VkShaderStageFlagBits stage,
		const char* entrypoint = "main"
	);

private:
	//**************************************************
	/// \brief メモリタイプの取得
	/// 
	/// \param[in] requestBits
	/// \param[in] requestProperty
	/// 
	/// \return メモリタイプインデックス
	//**************************************************
	uint32_t getMemoryType(
		uint32_t requestBits,
		VkMemoryPropertyFlags requestProperty
	);

	//**************************************************
	/// \brief インスタンスの生成
	/// 
	/// \return VK_SUCCESS なら成功
	//**************************************************
	VkResult createInstance();

	//**************************************************
	/// \brief 物理デバイスの生成
	/// 
	/// \return Success is VK_SUCCESS
	//**************************************************
	VkResult createPhysicalDevice();

	//**************************************************
	/// \brief 論理デバイスの生成
	/// 
	/// \return VK_SUCCESS なら成功
	//**************************************************
	VkResult createDevice();

	//**************************************************
	/// \brief サーフェースの生成
	/// 
	/// \param[in] hWnd
	/// 
	/// \return VK_SUCCESS なら成功
	//**************************************************
	VkResult createSurface(HWND hWnd);

	//**************************************************
	/// \brief スワップチェインの生成
	/// 
	/// \param[in] oldSwapChain
	/// 
	/// \return VK_SUCCESS なら成功
	//**************************************************
	VkResult createSwapchain(
		VkSwapchainKHR oldSwapChain
	);

	//**************************************************
	/// \brief 深度バッファの生成
	/// 
	/// \return VK_SUCCESS なら成功
	//**************************************************
	VkResult createDepthBuffer();

	//**************************************************
	/// \brief レンダーパスの生成
	/// 
	/// \return VK_SUCCESS なら成功
	//**************************************************
	VkResult createRenderPass();

	//**************************************************
	/// \brief フレームバッファの生成
	/// 
	/// \return VK_SUCCESS なら成功
	//**************************************************
	VkResult createFrameBuffer();

	//**************************************************
	/// \brief フェンスの生成
	/// 
	/// \return VK_SUCCESS なら成功
	//**************************************************
	VkResult createFence();

	//**************************************************
	/// \brief イメージメモリバリア
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
	/// \brief シェーダリソースのレイアウトとプールの作成
	/// 
	/// \param[in] shaderResources
	/// \param[out] setLayout
	/// \param[out] pool
	/// 
	/// \return なし
	//**************************************************
	void pipelineCreateSetLayoutAndCreatePool(
		std::vector<VulkanShaderResource*> shaderResources,
		VkDescriptorSetLayout& setLayout,
		VkDescriptorPool& pool
	);

	//**************************************************
	/// \brief シェーダリソースのデスクリプタを設定
	/// 
	/// \param[in] shaderResources
	/// \param[in] setLayout
	/// \param[in] pool
	/// \param[out] sets
	/// 
	/// \return なし
	//**************************************************
	void pipelineDescriptorSet(
		std::vector<VulkanShaderResource*> shaderResources,
		VkDescriptorSetLayout setLayout,
		VkDescriptorPool pool,
		std::vector<VkDescriptorSet>& sets
	);

	//**************************************************
	/// \brief テクスチャリソースのバインディングを取得
	/// 
	/// \param[in] textureResources
	/// \param[out] setLayout
	/// \param[out] pool
	/// 
	/// \return なし
	//**************************************************
	void pipelineCreateSetLayoutAndCreatePool(
		std::vector<VulkanTexture*> textureResources,
		VkDescriptorSetLayout& setLayout,
		VkDescriptorPool& pool
	);

	//**************************************************
	/// \brief テクスチャリソースのデスクリプタを設定
	/// 
	/// \param[in] textureResources
	/// \param[in] setLayout
	/// \param[in] pool
	/// \param[out] set
	/// 
	/// \return なし
	//**************************************************
	VkDescriptorSet pipelineDescriptorSet(
		std::vector<VulkanTexture*> textureResources,
		VkDescriptorSetLayout setLayout,
		VkDescriptorPool pool
	);

	//**************************************************
	/// \brief サンプラーリソースのバインディングを取得
	/// 
	/// \param[in] shaderResources
	/// \param[out] setLayout
	/// \param[out] pool
	/// 
	/// \return なし
	//**************************************************
	void pipelineCreateSetLayoutAndCreatePool(
		std::vector<VulkanSampler*> samplerResources,
		VkDescriptorSetLayout& setLayout,
		VkDescriptorPool& pool
	);

	//**************************************************
	/// \brief サンプラーリソースのデスクリプタを設定
	/// 
	/// \param[in] samplerResources
	/// \param[in] setLayout
	/// \param[in] pool
	/// \param[out] set
	/// 
	/// \return なし
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

	// スワップチェイン
	VkSwapchainKHR                      m_swapchain;
	std::vector<VkImage>                m_swapchainImages;
	std::vector<VkImageView>            m_swapchainViews;

	// 深度バッファ
	VkImage                             m_depthImage;
	VkImageView                         m_depthImageView;
	VkDeviceMemory                      m_depthImageMemory;

	VkRenderPass                        m_renderPass;
	std::vector<VkFramebuffer>          m_frameBuffers;

	std::vector<VkCommandBuffer>        m_commands;

	// フェンス
	std::vector<VkFence>                m_fences;
	VkSemaphore                         m_renderSem;
	VkSemaphore                         m_presentSem;

	// ビューポート・シザーレクト
	VkViewport							m_viewport;
	VkRect2D							m_scissor;

	// 現在のスワップチェインイメージの取得
	uint32_t                            m_imageIndex;

private:
	const char* k_APP_NAME = "Vulkan";

};

