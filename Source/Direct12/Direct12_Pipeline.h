/*===================================================================================
*    Date : 2022/12/04(��)
*        Author		: Gakuto.S
*        File		: Direct12_Pipeline.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Direct12_Graphics.h"
#include "../IF_Pipeline.h"

class Direct12Pipeline : public IPipeline
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	Direct12Pipeline(
		ID3D12RootSignature* rootSignature,
		ID3D12PipelineState* pipeline,
		D3D12_PRIMITIVE_TOPOLOGY primitive,
		std::map<size_t, ID3D12Resource*> shaderResources,
		std::map<size_t, ID3D12DescriptorHeap*> textureHeaps
	)
		:m_rootSignature(rootSignature),
		m_pipeline(pipeline),
		m_primitive(primitive),
		m_shaderResources(shaderResources),
		m_setHeaps(),
		m_textureHeaps(textureHeaps)
	{
		for (auto& heap : m_textureHeaps)
			m_setHeaps.push_back(heap.second);
	}

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~Direct12Pipeline()
	{
		m_shaderResources.clear();
		SAFE_RELEASE(m_rootSignature);
		SAFE_RELEASE(m_pipeline);
	}

	////////		Direct12�Ɉˑ����郁�\�b�h�Q		////////
public:
	//**************************************************
	/// \brief ���[�g�V�O�l�`���̎擾
	/// 
	/// \return ���[�g�V�O�l�`��
	//**************************************************
	ID3D12RootSignature* GetRootSignature() { return m_rootSignature; }

	//**************************************************
	/// \brief �p�C�v���C���X�e�[�g�̎擾
	/// 
	/// \return �p�C�v���C���X�e�[�g
	//**************************************************
	ID3D12PipelineState* GetPipelineState() { return m_pipeline; }

	//**************************************************
	/// \brief �v���~�e�B�u�g�|���W�[�̎擾
	/// 
	/// \return �v���~�e�B�u�g�|���W�[
	//**************************************************
	D3D12_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() { return m_primitive; }

	//**************************************************
	/// \brief �V�F�[�_���\�[�X�̎擾
	/// 
	/// \return �p�C�v���C���X�e�[�g
	//**************************************************
	std::map<size_t, ID3D12Resource*> GetShaderResources() { return m_shaderResources; }

	//**************************************************
	/// \brief �Z�b�g����q�[�v�̎擾
	/// 
	/// \return �q�[�v
	//**************************************************
	std::vector<ID3D12DescriptorHeap*> GetSetHeaps() { return m_setHeaps; }

	//**************************************************
	/// \brief �e�N�X�`���q�[�v�̎擾
	/// 
	/// \return �e�N�X�`���q�[�v
	//**************************************************
	std::map<size_t, ID3D12DescriptorHeap*> GetTextureHeaps() { return m_textureHeaps; }

private:
	ID3D12RootSignature*				m_rootSignature;
	ID3D12PipelineState*				m_pipeline;
	D3D12_PRIMITIVE_TOPOLOGY			m_primitive;

	std::map<size_t, ID3D12Resource*>	m_shaderResources;
	
	std::vector<ID3D12DescriptorHeap*>		m_setHeaps;
	std::map<size_t, ID3D12DescriptorHeap*>	m_textureHeaps;
};

