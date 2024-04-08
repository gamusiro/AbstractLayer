/*===================================================================================
*    Date : 2022/12/03(�y)
*        Author		: Gakuto.S
*        File		: Direct12_Graphics.h
*        Detail		:
===================================================================================*/
#pragma once

// �}�N����`
#define SAFE_RELEASE(p) if(p) p->Release(); p = nullptr;

// �E�B���h�E�Y�v���b�g�t�H�[��
#include "../Window_Win.h"

// DirectX12�̃C���N���[�h
#include <d3d12.h>
#include <dxgi1_6.h>

// ���C�u�����̃����N
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include "../IF_Graphics.h"

class Direct12Graphics : public IGraphics
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	Direct12Graphics(int width, int height, WindowWin* window);

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~Direct12Graphics();

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
	/// \param[in] binding
	/// 
	/// \return �e�N�X�`�����\�[�X
	//**************************************************
	ITexture* CreateTextureFromFile(
		const char* fileName,
		unsigned int binding
	) override;

	//**************************************************
	/// \brief �T���v���[�̍쐬����
	/// 
	/// \param[in] samplerDesc
	/// \param[in] binding
	/// 
	/// \return �T���v���[
	//**************************************************
	ISampler* CreateSampler(
		sampler::SAMPLER_DESC samplerDesc,
		unsigned int binding
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


	////////		Direct12�Ɉˑ����郁�\�b�h�Q		////////
public:
	//**************************************************
	/// \brief �f�o�C�X�̎擾
	/// 
	/// \return ID3D12Device
	//**************************************************
	ID3D12Device* getDevice() { return m_device; }

private:
	//**************************************************
	/// \brief �f�o�C�X�ƃX���b�v�`�F�C���̐���
	/// 
	/// \param[in] width
	/// \param[in] height
	/// \param[in] hWnd
	/// 
	/// \return true �Ȃ琬��
	//**************************************************
	HRESULT createDeviceAndSwapChain(
		const int width,
		const int height,
		const HWND hWnd
	);

	//**************************************************
	/// \brief �����_�[�^�[�Q�b�g�̐���
	/// 
	/// \return true �Ȃ琬��
	//**************************************************
	HRESULT createRenderTargetView();

	//**************************************************
	/// \brief �[�x�o�b�t�@�̐���
	/// 
	/// \param[in] width
	/// \param[in] height
	/// 
	/// \return true �Ȃ琬��
	//**************************************************
	HRESULT createDepthBuffer(
		const int width,
		const int height
	);

	//**************************************************
	/// \brief �t�F���X�̐���
	/// 
	/// \return true �Ȃ琬��
	//**************************************************
	HRESULT createFence();

	//**************************************************
	/// \brief GPU�̏����I���҂�
	/// 
	/// \return �Ȃ�
	//**************************************************
	void waitForIdleGPU();

	//**************************************************
	/// \brief ���\�[�X�o���A�̐ݒ�
	/// 
	/// \param[in] index
	/// \param[in] before
	/// \param[in] after
	/// 
	/// \return �Ȃ�
	//**************************************************
	void setResourceBarrier(
		const UINT index,
		D3D12_RESOURCE_STATES before,
		D3D12_RESOURCE_STATES after
	);

	//**************************************************
	/// \brief �r���[�|�[�g�̐ݒ�
	/// 
	/// \param[in] width
	/// \param[in] height
	/// 
	/// \return �Ȃ�
	//**************************************************
	void setViewport(
		const int width,
		const int height
	);

	//**************************************************
	/// \brief �V�U�[���N�g�̐ݒ�
	/// 
	/// \param[in] width
	/// \param[in] height
	/// 
	/// \return �Ȃ�
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

