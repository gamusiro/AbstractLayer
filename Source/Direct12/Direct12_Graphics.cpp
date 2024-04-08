/*===================================================================================
*    Date : 2022/12/03(土)
*        Author		: Gakuto.S
*        File		: Direct12_Graphics.cpp
*        Detail		:
===================================================================================*/
#include "Direct12_Graphics.h"

// Direct12 オブジェクト群
#include "Direct12_VBO.h"
#include "Direct12_EBO.h"
#include "Direct12_Pipeline.h"
#include "Direct12_ShaderResource.h"
#include "Direct12_Texture.h"
#include "Direct12_Sampler.h"

using namespace DirectX;
using namespace std;



Direct12Graphics::Direct12Graphics(int width, int height, WindowWin* window)
	:m_device(),
	m_commandAllocator(),
	m_commandList(),
	m_commandQueue(),
	m_swapChain(),
	m_renderTargetViewHeap(),
	m_backBuffers(),
	m_depthBuffer(),
	m_depthBufferHeap(),
	m_fence(),
	m_fenceValue(),
	m_viewport(),
	m_scissorRect()
{
    createDeviceAndSwapChain(width, height, window->GetHandle());

    createRenderTargetView();

    createDepthBuffer(width, height);

    createFence();

    setViewport(width, height);
    setScissorRect(width, height);
}

Direct12Graphics::~Direct12Graphics()
{
    SAFE_RELEASE(m_fence);
    SAFE_RELEASE(m_depthBufferHeap);
    SAFE_RELEASE(m_depthBuffer);

    for (size_t i = 0; i < m_backBuffers.size(); ++i)
    {
        SAFE_RELEASE(m_backBuffers[i]);
    }


    SAFE_RELEASE(m_renderTargetViewHeap);
    SAFE_RELEASE(m_swapChain);
    SAFE_RELEASE(m_commandQueue);
    SAFE_RELEASE(m_commandList);
    SAFE_RELEASE(m_commandAllocator);
    SAFE_RELEASE(m_device);
}

void Direct12Graphics::Clear()
{
    // 現在のバッファインデックスを取得
	UINT index = m_swapChain->GetCurrentBackBufferIndex();
	setResourceBarrier(
		index,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET
	);

	// レンダーターゲットの指定
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetHeapHandle = m_renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
	renderTargetHeapHandle.ptr += index * m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_CPU_DESCRIPTOR_HANDLE depthBufferHeapHandle = m_depthBufferHeap->GetCPUDescriptorHandleForHeapStart();
	m_commandList->OMSetRenderTargets(1, &renderTargetHeapHandle, false, &depthBufferHeapHandle);

	float clearColor[]{ 0.0f, 0.0f, 0.5f, 1.0f };
	m_commandList->ClearRenderTargetView(renderTargetHeapHandle, clearColor, 0, nullptr);	// Clear render target view command
	m_commandList->ClearDepthStencilView(													// Clear depth buffer command
		m_depthBufferHeap->GetCPUDescriptorHandleForHeapStart(),
		D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_DEPTH,
		D3D12_MAX_DEPTH,
		0,
		0,
		nullptr
	);

	m_commandList->RSSetViewports(1, &m_viewport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);
}

void Direct12Graphics::Present()
{
    // 現在のバッファのインデックス
    UINT index = m_swapChain->GetCurrentBackBufferIndex();

    setResourceBarrier(
        index,
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT
    );

    // コマンドの受付終了
    m_commandList->Close();

    // コマンドの実行
    std::vector<ID3D12CommandList*> commandLists{ m_commandList };
    m_commandQueue->ExecuteCommandLists(UINT(commandLists.size()), &commandLists[0]);

	waitForIdleGPU();

    // リセット
    m_commandAllocator->Reset();
    m_commandList->Reset(m_commandAllocator, nullptr);

    // バッファ切り替え
    m_swapChain->Present(1, 0);
}

IVertexBufferObject* Direct12Graphics::CreateVertexBuffer(size_t size, size_t stride)
{
    return new Direct12VBO(UINT(size), stride, this);
}

void Direct12Graphics::SetVertexBuffer(IVertexBufferObject* vertexBuffer)
{
	Direct12VBO* vbo = (Direct12VBO*)vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW bufferView = vbo->GetBufferView();
	m_commandList->IASetVertexBuffers(0, 1, &bufferView);
}

IElementBufferObject* Direct12Graphics::CreateIndexBuffer(size_t size, unsigned int countIndex, INDEX_FORMAT format)
{
	//**************************************************
	/// \brief インデックスデータフォーマット
	//**************************************************
	map<INDEX_FORMAT, DXGI_FORMAT> k_FORMAT
	{
		{INDEX_FORMAT::UINT16, DXGI_FORMAT::DXGI_FORMAT_R16_UINT},
		{INDEX_FORMAT::UINT32, DXGI_FORMAT::DXGI_FORMAT_R32_UINT},
	};

    return new Direct12EBO(UINT(size), countIndex, k_FORMAT[format], this);
}

void Direct12Graphics::SetIndexBuffer(IElementBufferObject* elementBuffer)
{
	Direct12EBO* ebo = (Direct12EBO*)elementBuffer;
	D3D12_INDEX_BUFFER_VIEW bufferView = ebo->GetBufferView();
	m_commandList->IASetIndexBuffer(&bufferView);
}

IPipeline* Direct12Graphics::CreatePipeline(
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
#pragma region モード変換
	//**************************************************
	/// \brief 頂点データフォーマット
	//**************************************************
	map<pipeline::vertexLayout::FORMAT, DXGI_FORMAT> k_FORMAT
	{
		{pipeline::vertexLayout::FORMAT::R32G32B32_FLOAT,	DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT },
		{pipeline::vertexLayout::FORMAT::R32G32_FLOAT,		DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT }
	};

	//**************************************************
	/// \brief ブレンドモード
	//**************************************************
	std::map<pipeline::blend::BLEND, D3D12_BLEND> k_BLEND
	{
		{pipeline::blend::BLEND::ZERO,			D3D12_BLEND::D3D12_BLEND_ZERO },
		{pipeline::blend::BLEND::ONE,			D3D12_BLEND::D3D12_BLEND_ONE },
		{pipeline::blend::BLEND::SRC_ALPHA,		D3D12_BLEND::D3D12_BLEND_SRC_ALPHA },
		{pipeline::blend::BLEND::INV_SRC_ALPHA,	D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA },
	};

	//**************************************************
	/// \brief ブレンドオプションモード
	//**************************************************
	std::map<pipeline::blend::BLEND_OP, D3D12_BLEND_OP> k_BLEND_OP
	{
		{pipeline::blend::BLEND_OP::ADD, D3D12_BLEND_OP::D3D12_BLEND_OP_ADD },
	};

	//**************************************************
	/// \brief カリングモード
	//**************************************************
	std::map<pipeline::rasterizer::CULL_MODE, D3D12_CULL_MODE> k_CullMode
	{
		{pipeline::rasterizer::CULL_MODE::NONE,		D3D12_CULL_MODE::D3D12_CULL_MODE_NONE},
		{pipeline::rasterizer::CULL_MODE::FRONT,	D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT},
		{pipeline::rasterizer::CULL_MODE::BACK,		D3D12_CULL_MODE::D3D12_CULL_MODE_BACK},
	};

	//**************************************************
	/// \brief フィルモード
	//**************************************************
	std::map<pipeline::rasterizer::FILL_MODE, D3D12_FILL_MODE> k_FillMode
	{
		{pipeline::rasterizer::FILL_MODE::SOLID,		D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID},
		{pipeline::rasterizer::FILL_MODE::WIREFRAME,	D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME}
	};

	//**************************************************
	/// \brief プリミティブトポロジー
	//**************************************************
	std::map<pipeline::primitive::PRIMITIVE_TOPOLOGY, D3D_PRIMITIVE_TOPOLOGY> k_PrimitiveMode
	{
		{ pipeline::primitive::PRIMITIVE_TOPOLOGY::TRIANGLE_LIST,  D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST},
		{ pipeline::primitive::PRIMITIVE_TOPOLOGY::TRIANGLE_STRIP, D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP }
	};
#pragma endregion

	


	HRESULT ret{};

	// シェーダの読み込み
	common::FileIO fileVS((shaderDesc.vsName + ".cso").c_str());
	common::FileIO filePS((shaderDesc.psName + ".cso").c_str());

	string vsBuffer = fileVS.GetInfo();
	string psBuffer = filePS.GetInfo();

	// シェーダに転送するデータを構成する
	vector<D3D12_ROOT_PARAMETER> parameters;

	// 定数バッファ
	map<size_t, ID3D12Resource*> dShaderResources;
	for (auto& sr : shaderResources)
	{
		size_t index = parameters.size();
		Direct12ShaderResource* dsr = (Direct12ShaderResource*)sr;
		dShaderResources[index] = dsr->GetBuffer();
		parameters.push_back(dsr->GetRootParameter());
	}

	// テクスチャ
	map<size_t, ID3D12DescriptorHeap*> dtextureResources;
	for (auto& tr : textureResources)
	{
		size_t index = parameters.size();
		Direct12Texture* dsr = (Direct12Texture*)tr;
		dtextureResources[index] = dsr->GetHeap();
		parameters.push_back(dsr->GetRootParameter());
	}

	// サンプラー
	vector<D3D12_STATIC_SAMPLER_DESC> samplers;
	for (auto& sr : samplerResources)
	{
		size_t index = parameters.size();
		Direct12Sampler* dsr = (Direct12Sampler*)sr;
		samplers.push_back(dsr->GetSamplerDesc());
	}
	


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	{// "D3D12_ROOT_SIGNATURE_DESC" 構造体の設定
		rootSignatureDesc.Flags				= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.NumParameters		= UINT(parameters.size());
		rootSignatureDesc.pParameters		= parameters.data();
		rootSignatureDesc.NumStaticSamplers = UINT(samplers.size());
		rootSignatureDesc.pStaticSamplers	= samplers.data();
	}

	// ルートシグネチャの構築
	ID3DBlob* signatureBlob{};
	ret = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&signatureBlob,
		nullptr
	);
	if (FAILED(ret))
		return nullptr;

	// ルートシグネチャの生成
	ID3D12RootSignature* rootSignature{};
	ret = m_device->CreateRootSignature(
		0,
		signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(),
		__uuidof(ID3D12RootSignature),
		(void**)&rootSignature
	);
	if (FAILED(ret))
		throw "ルートシグネチャの作成に失敗";

	// 頂点レイアウトの設定
	vector<D3D12_INPUT_ELEMENT_DESC> inputElementDesc(layouts.size());
	for (size_t i = 0; i < inputElementDesc.size(); ++i)
	{
		inputElementDesc[i] = {
		layouts[i].semanticName,
		0,
		k_FORMAT[layouts[i].format],
		0,
		layouts[i].offset,
		D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
		};
	}
		

	// ブレンドステートの設定
	D3D12_BLEND_DESC blendStateDesc{};
	{// "D3D12_BLEND_DESC" 構造体の設定
		blendStateDesc.AlphaToCoverageEnable					= false;
		blendStateDesc.IndependentBlendEnable					= false;
		blendStateDesc.RenderTarget[0].BlendEnable				= blendDesc.blendEnable;
		blendStateDesc.RenderTarget[0].LogicOpEnable			= blendDesc.logicOpEnable;
		blendStateDesc.RenderTarget[0].SrcBlend					= k_BLEND[blendDesc.srcColorBlend];
		blendStateDesc.RenderTarget[0].DestBlend				= k_BLEND[blendDesc.dstColorBlend];
		blendStateDesc.RenderTarget[0].BlendOp					= k_BLEND_OP[blendDesc.colorBlendOp];
		blendStateDesc.RenderTarget[0].SrcBlendAlpha			= k_BLEND[blendDesc.srcAlphaBlend];
		blendStateDesc.RenderTarget[0].DestBlendAlpha			= k_BLEND[blendDesc.dstAlphaBlend];
		blendStateDesc.RenderTarget[0].BlendOpAlpha				= k_BLEND_OP[blendDesc.alphaBlendOp];
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask	= blendDesc.colorWriteMask;
	}

	// ラスタライザステートの設定
	D3D12_RASTERIZER_DESC rasterizerStateDesc{};
	{// "D3D12_RASTERIZER_DESC" 構造体の設定
		rasterizerStateDesc.MultisampleEnable		= false;
		rasterizerStateDesc.CullMode				= k_CullMode[rasterizerDesc.cMode];
		rasterizerStateDesc.FillMode				= k_FillMode[rasterizerDesc.fMode];
		rasterizerStateDesc.DepthClipEnable			= true;
		rasterizerStateDesc.FrontCounterClockwise	= false;
		rasterizerStateDesc.DepthBias				= D3D12_DEFAULT_DEPTH_BIAS;
		rasterizerStateDesc.DepthBiasClamp			= D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizerStateDesc.SlopeScaledDepthBias	= D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizerStateDesc.AntialiasedLineEnable	= false;
		rasterizerStateDesc.ForcedSampleCount		= 0;
		rasterizerStateDesc.ConservativeRaster		= D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}

	// デプスステンシルステートの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilStateDesc{};
	{
		depthStencilStateDesc.DepthEnable					= true;
		depthStencilStateDesc.StencilEnable					= true;
		depthStencilStateDesc.DepthFunc						= D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS_EQUAL;
		depthStencilStateDesc.DepthWriteMask				= D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.StencilReadMask				= 0xff;
		depthStencilStateDesc.StencilWriteMask				= 0xff;
		depthStencilStateDesc.FrontFace.StencilDepthFailOp	= D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilFailOp		= D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilPassOp		= D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilFunc			= D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
		depthStencilStateDesc.BackFace.StencilDepthFailOp	= D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilFailOp		= D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilPassOp		= D3D12_STENCIL_OP::D3D12_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilFunc			= D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
	}


	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};
	{// "D3D12_GRAPHICS_PIPELINE_STATE_DESC" 構造体の設定
		pipelineStateDesc.SampleMask						= D3D12_DEFAULT_SAMPLE_MASK;
		pipelineStateDesc.VS.BytecodeLength					= vsBuffer.size();
		pipelineStateDesc.VS.pShaderBytecode				= vsBuffer.data();
		pipelineStateDesc.PS.BytecodeLength					= psBuffer.size();
		pipelineStateDesc.PS.pShaderBytecode				= psBuffer.data();
		pipelineStateDesc.InputLayout.NumElements			= UINT(inputElementDesc.size());
		pipelineStateDesc.InputLayout.pInputElementDescs	= inputElementDesc.data();
		pipelineStateDesc.BlendState						= blendStateDesc;
		pipelineStateDesc.NumRenderTargets					= 1;
		pipelineStateDesc.RTVFormats[0]						= DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineStateDesc.DepthStencilState					= depthStencilStateDesc;
		pipelineStateDesc.DSVFormat							= DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		pipelineStateDesc.RasterizerState					= rasterizerStateDesc;
		pipelineStateDesc.IBStripCutValue					= D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineStateDesc.PrimitiveTopologyType				= D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateDesc.SampleDesc.Count					= 1;
		pipelineStateDesc.SampleDesc.Quality				= 0;
		pipelineStateDesc.pRootSignature					= rootSignature;
	}

	// パイプラインの作成
	ID3D12PipelineState* pipelineState{};
	ret = m_device->CreateGraphicsPipelineState(
		&pipelineStateDesc,
		__uuidof(ID3D12PipelineState),
		(void**)&pipelineState
	);
	if (FAILED(ret))
		throw "パイプラインの作成に失敗";

	SAFE_RELEASE(signatureBlob);

	return new Direct12Pipeline(
		rootSignature,
		pipelineState,
		k_PrimitiveMode[primitive],
		dShaderResources,
		dtextureResources
	);
}

void Direct12Graphics::SetPipeline(IPipeline* pipeline)
{
	Direct12Pipeline* pl = (Direct12Pipeline*)pipeline;
	m_commandList->SetPipelineState(pl->GetPipelineState());
	m_commandList->SetGraphicsRootSignature(pl->GetRootSignature());

	// 定数
	for (auto& resource : pl->GetShaderResources())
		m_commandList->SetGraphicsRootConstantBufferView(UINT(resource.first), resource.second->GetGPUVirtualAddress());

	m_commandList->RSSetViewports(1, &m_viewport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);
	m_commandList->IASetPrimitiveTopology(pl->GetPrimitiveTopology());

	// テクスチャ
	auto heaps = pl->GetSetHeaps();
	if (!heaps.empty())
	{
		m_commandList->SetDescriptorHeaps(UINT(heaps.size()), heaps.data());

		for (auto& heap : pl->GetTextureHeaps())
			m_commandList->SetGraphicsRootDescriptorTable(UINT(heap.first), heap.second->GetGPUDescriptorHandleForHeapStart());
	}
}

IShaderResource* Direct12Graphics::CreateShaderReource(size_t size, unsigned int slotNum)
{
    return new Direct12ShaderResource(UINT(size), UINT(slotNum), this);
}

ITexture* Direct12Graphics::CreateTextureFromFile(const char* fileName, unsigned int slotNum)
{
    return new Direct12Texture(fileName, UINT(slotNum), this);
}

ISampler* Direct12Graphics::CreateSampler(sampler::SAMPLER_DESC samplerDesc, unsigned int slotNum)
{
    return new Direct12Sampler(samplerDesc, UINT(slotNum));
}

void Direct12Graphics::DrawIndexed(unsigned int numIndex, unsigned int numInstance)
{
	m_commandList->DrawIndexedInstanced(numIndex, numInstance, 0, 0, 0);
}

void Direct12Graphics::DrawInstanced(unsigned int numVertex, unsigned int numInstance)
{
	m_commandList->DrawInstanced(numVertex, numInstance, 0, 0);
}

void Direct12Graphics::Resize(int width, int height)
{
	waitForIdleGPU();
	if (width <= 0 || height <= 0)
		return;

	// スワップチェーンのバッファリサイズ
	for (auto& buffer : m_backBuffers)
		buffer->Release();

	m_swapChain->ResizeBuffers(
		k_BUFFER_NUM,
		UINT(width), UINT(height),
		DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
		DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	);

	HRESULT ret{};
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();	// heap address
	m_backBuffers.resize(k_BUFFER_NUM);
	for (UINT i = 0; i < k_BUFFER_NUM; ++i)
	{// レンダーターゲットビューの作成
		ret = m_swapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&m_backBuffers[i]);
		if (FAILED(ret))
			return;

		m_device->CreateRenderTargetView(m_backBuffers[i], nullptr, handle);
		handle.ptr += m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// 深度バッファの再生成
	SAFE_RELEASE(m_depthBufferHeap);
	SAFE_RELEASE(m_depthBuffer);

	createDepthBuffer(width, height);

	setViewport(width, height);
	setScissorRect(width, height);
}

#pragma region プライベートメソッド

HRESULT Direct12Graphics::createDeviceAndSwapChain(const int width, const int height, const HWND hWnd)
{
    HRESULT ret{};

	// デバイスの作成処理
	ret = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
		__uuidof(ID3D12Device),
		(void**)&m_device
	);
	if (FAILED(ret))
		return ret;

	// コマンドアロケータの作成
	ret = m_device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		__uuidof(ID3D12CommandAllocator),
		(void**)&m_commandAllocator
	);
	if (FAILED(ret))
		return ret;

	// グラフィックスコマンドリストの作成
	ret = m_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_commandAllocator,
		nullptr,
		__uuidof(ID3D12GraphicsCommandList),
		(void**)&m_commandList
	);
	if (FAILED(ret))
		return ret;

	D3D12_COMMAND_QUEUE_DESC queueDesc{};
	{// "COMMAND_QUEUE_DESC" 構造体の設定
		queueDesc.Flags		= D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.NodeMask	= 0;
		queueDesc.Priority	= D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		queueDesc.Type		= D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	}
	
	// コマンドキューの作成
	ret = m_device->CreateCommandQueue(
		&queueDesc,
		__uuidof(ID3D12CommandQueue),
		(void**)&m_commandQueue
	);
	if (FAILED(ret))
		return ret;

	// ファクトリの作成
	IDXGIFactory6* factory{};
	ret = CreateDXGIFactory2(
		0,
		__uuidof(IDXGIFactory6),
		(void**)&factory
	);
	if (FAILED(ret) || !factory)
		return ret;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	{// "SWAP_CHAIN_DESC1" 構造体の設定
		swapChainDesc.Width					= width;
		swapChainDesc.Height				= height;
		swapChainDesc.Format				= DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.Stereo				= false;
		swapChainDesc.SampleDesc.Count		= 1;
		swapChainDesc.SampleDesc.Quality	= 0;
		swapChainDesc.BufferUsage			= DXGI_USAGE_BACK_BUFFER;
		swapChainDesc.BufferCount			= k_BUFFER_NUM;
		swapChainDesc.Scaling				= DXGI_SCALING::DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect			= DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode				= DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Flags					= DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	}
	
	// スワップチェインの作成
	ret = factory->CreateSwapChainForHwnd(
		m_commandQueue,
		hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&m_swapChain
	);

	SAFE_RELEASE(factory);
	return ret;
}

HRESULT Direct12Graphics::createRenderTargetView()
{
    HRESULT ret{};

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	{// "DESCRIPTOR_HEAP_DESC" 構造体の設定
		heapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.NodeMask		= 0;
		heapDesc.NumDescriptors = k_BUFFER_NUM;
		heapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	}
	
	// レンダーターゲット用のヒープを作成
	ret = m_device->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_renderTargetViewHeap);
	if (FAILED(ret))
		return ret;

	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	ret = m_swapChain->GetDesc(&swapChainDesc);
	if (FAILED(ret))
		return ret;

	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();	// heap address
	m_backBuffers.resize(k_BUFFER_NUM);
	for (UINT i = 0; i < k_BUFFER_NUM; ++i)
	{// レンダーターゲットビューの作成
		ret = m_swapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&m_backBuffers[i]);
		if (FAILED(ret))
			return ret;

		m_device->CreateRenderTargetView(m_backBuffers[i], nullptr, handle);
		handle.ptr += m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	return ret;
}

HRESULT Direct12Graphics::createDepthBuffer(const int width, const int height)
{
    HRESULT ret{};
	D3D12_RESOURCE_DESC resourceDesc{};
	{// "RESOURCE_DESC" 構造体の設定
		resourceDesc.Dimension			= D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resourceDesc.Width				= width;
		resourceDesc.Height				= height;
		resourceDesc.DepthOrArraySize	= 1;
		resourceDesc.Format				= DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		resourceDesc.SampleDesc.Count	= 1;
		resourceDesc.Flags				= D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		resourceDesc.MipLevels			= 1;
		resourceDesc.Layout				= D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resourceDesc.Alignment			= 0;
	}
	
	D3D12_HEAP_PROPERTIES heapProperties{};
	{// "HEAP_PROPERTIES" 構造体の設定
		heapProperties.Type					= D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		heapProperties.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	}
	
	D3D12_CLEAR_VALUE clearValue{};
	{// "CLEAR_VALUE" 構造体の設定
		clearValue.DepthStencil.Depth	= D3D12_MAX_DEPTH;
		clearValue.Format				= DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	}
	
	// 深度バッファの作成
	ret = m_device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		__uuidof(ID3D12Resource),
		(void**)&m_depthBuffer
	);
	if (FAILED(ret))
		return ret;

	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	{// DESCRIPTOR_HEAP_DESC 構造体の設定
		descriptorHeapDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		descriptorHeapDesc.NumDescriptors	= 1;
	}
	
	// 深度バッファのヒープ作成
	ret = m_device->CreateDescriptorHeap(
		&descriptorHeapDesc,
		__uuidof(ID3D12DescriptorHeap),
		(void**)&m_depthBufferHeap
	);
	if (FAILED(ret))
		return ret;

	D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc{};
	{// "DEPTH_STENCIL_VIEW_DESC" 構造体の設定
		viewDesc.Format			= DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		viewDesc.ViewDimension	= D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;
		viewDesc.Flags			= D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	}
	
	// 深度バッファビューの作成
	m_device->CreateDepthStencilView(m_depthBuffer, &viewDesc, m_depthBufferHeap->GetCPUDescriptorHandleForHeapStart());

	return ret;
}

HRESULT Direct12Graphics::createFence()
{
    HRESULT ret{};
	
	// フェンスの生成
	ret = m_device->CreateFence(
		m_fenceValue,
		D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE,
		__uuidof(ID3D12Fence),
		(void**)&m_fence
	);

	return ret;
}

void Direct12Graphics::waitForIdleGPU()
{
	// 待ち
	m_commandQueue->Signal(m_fence, ++m_fenceValue);

	if (m_fence->GetCompletedValue() != m_fenceValue)
	{
		HANDLE fenceEvent = CreateEvent(nullptr, false, false, nullptr);
		if (fenceEvent)
		{
			m_fence->SetEventOnCompletion(m_fenceValue, fenceEvent);
			WaitForSingleObject(fenceEvent, INFINITE);
			CloseHandle(fenceEvent);
		}
	}
}

void Direct12Graphics::setResourceBarrier(const UINT index, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrierDesc{};
	{// "RESOURCE_BARRIER" 構造体の設定
		barrierDesc.Type					= D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrierDesc.Flags					= D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrierDesc.Transition.pResource	= m_backBuffers[index];
		barrierDesc.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrierDesc.Transition.StateBefore	= before;
		barrierDesc.Transition.StateAfter	= after;
	}
	m_commandList->ResourceBarrier(1, &barrierDesc);
}

void Direct12Graphics::setViewport(const int width, const int height)
{
	// アスペクト比の設定
	m_aspect = float(width) / float(height);

	m_viewport.Width	= FLOAT(width);
	m_viewport.Height	= FLOAT(height);
	m_viewport.MaxDepth = D3D12_MAX_DEPTH;
}

void Direct12Graphics::setScissorRect(const int width, const int height)
{
	m_scissorRect.left		= 0;
	m_scissorRect.top		= 0;
	m_scissorRect.right		= m_scissorRect.left + width;
	m_scissorRect.bottom	= m_scissorRect.top + height;
}

#pragma endregion