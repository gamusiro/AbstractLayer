/*===================================================================================
*    Date : 2022/12/04(��)
*        Author		: Gakuto.S
*        File		: Direct12_ShaderResource.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Direct12_Graphics.h"
#include "../IF_ShaderResource.h"

class Direct12ShaderResource : public IShaderResource
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \param[in] size
	/// \param[in] slotNum
	/// \param[in] graphics
	/// 
	/// \return �Ȃ�
	//**************************************************
	Direct12ShaderResource(
		UINT size,
		UINT slotNum,
		Direct12Graphics* graphics
	);

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~Direct12ShaderResource();

	////////	�p�����\�b�h	////////
public:
	//**************************************************
	/// \brief �}�b�v
	/// 
	/// \return true �Ȃ琬��
	//**************************************************
	bool Map(void* data, size_t size) override;


	////////		Direct12�Ɉˑ����郁�\�b�h�Q		////////
public:
	//**************************************************
	/// \brief �}�b�v
	/// 
	/// \return true �Ȃ琬��
	//**************************************************
	D3D12_ROOT_PARAMETER GetRootParameter() { return m_rootParam; }

	//**************************************************
	/// \brief �o�b�t�@�̎擾
	/// 
	/// \return true �Ȃ琬��
	//**************************************************
	ID3D12Resource* GetBuffer() { return m_buffer; }

private:
	D3D12_ROOT_PARAMETER	m_rootParam;
	ID3D12Resource*			m_buffer;
};

