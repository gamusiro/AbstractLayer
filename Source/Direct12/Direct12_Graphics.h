/*===================================================================================
*    Date : 2022/12/03(土)
*        Author		: Gakuto.S
*        File		: Direct12_Graphics.h
*        Detail		:
===================================================================================*/
#pragma once

// マクロ定義
#define SAFE_RELEASE(p) if(p) p->Release(); p = nullptr;

// ウィンドウズプラットフォーム
#include "../Window_Win.h"

// DirectX12のインクルード
#include <d3d12.h>
#include <dxgi1_6.h>

// ライブラリのリンク
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include "../IF_Graphics.h"

class Direct12Graphics : public IGraphics
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	Direct12Graphics(int width, int height, WindowWin* window);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~Direct12Graphics();

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
	/// \param[in] binding
	/// 
	/// \return テクスチャリソース
	//**************************************************
	ITexture* CreateTextureFromFile(
		const char* fileName,
		unsigned int binding
	) override;

	//**************************************************
	/// \brief サンプラーの作成処理
	/// 
	/// \param[in] samplerDesc
	/// \param[in] binding
	/// 
	/// \return サンプラー
	//**************************************************
	ISampler* CreateSampler(
		sampler::SAMPLER_DESC samplerDesc,
		unsigned int binding
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


	////////		Direct12に依存するメソッド群		////////
public:
	//**************************************************
	/// \brief デバイスの取得
	/// 
	/// \return ID3D12Device
	//**************************************************
	ID3D12Device* getDevice() { return m_device; }

private:
	//**************************************************
	/// \brief デバイスとスワップチェインの生成
	/// 
	/// \param[in] width
	/// \param[in] height
	/// \param[in] hWnd
	/// 
	/// \return true なら成功
	//**************************************************
	HRESULT createDeviceAndSwapChain(
		const int width,
		const int height,
		const HWND hWnd
	);

	//**************************************************
	/// \brief レンダーターゲットの生成
	/// 
	/// \return true なら成功
	//**************************************************
	HRESULT createRenderTargetView();

	//**************************************************
	/// \brief 深度バッファの生成
	/// 
	/// \param[in] width
	/// \param[in] height
	/// 
	/// \return true なら成功
	//**************************************************
	HRESULT createDepthBuffer(
		const int width,
		const int height
	);

	//**************************************************
	/// \brief フェンスの生成
	/// 
	/// \return true なら成功
	//**************************************************
	HRESULT createFence();

	//**************************************************
	/// \brief GPUの処理終了待ち
	/// 
	/// \return なし
	//**************************************************
	void waitForIdleGPU();

	//**************************************************
	/// \brief リソースバリアの設定
	/// 
	/// \param[in] index
	/// \param[in] before
	/// \param[in] after
	/// 
	/// \return なし
	//**************************************************
	void setResourceBarrier(
		const UINT index,
		D3D12_RESOURCE_STATES before,
		D3D12_RESOURCE_STATES after
	);

	//**************************************************
	/// \brief ビューポートの設定
	/// 
	/// \param[in] width
	/// \param[in] height
	/// 
	/// \return なし
	//**************************************************
	void setViewport(
		const int width,
		const int height
	);

	//**************************************************
	/// \brief シザーレクトの設定
	/// 
	/// \param[in] width
	/// \param[in] height
	/// 
	/// \return なし
	//**************************************************
	void setScissorRect(
		const int width,
		const int height
	);

private:
	ID3D12Device*					m_device;
	ID3D12CommandAllocator*			m_commandAllocator;
	ID3D12GraphicsCommandList*		m_commandList;
	ID3D12CommandQueue*				m_commandQueue;
	IDXGISwapChain4*				m_swapChain;
	ID3D12DescriptorHeap*			m_renderTargetViewHeap;
	std::vector<ID3D12Resource*>	m_backBuffers;
	ID3D12Resource*					m_depthBuffer;
	ID3D12DescriptorHeap*			m_depthBufferHeap;
	ID3D12Fence*					m_fence;
	UINT							m_fenceValue;
	D3D12_VIEWPORT					m_viewport;
	D3D12_RECT						m_scissorRect;
};

