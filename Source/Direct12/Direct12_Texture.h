/*===================================================================================
*    Date : 2022/12/04(��)
*        Author		: Gakuto.S
*        File		: Direct12_Texture.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Direct12_Graphics.h"
#include "../IF_Texture.h"

class Direct12Texture : public ITexture
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	Direct12Texture(
		const char* fileName,
		UINT slotNum,
		Direct12Graphics* graphics,
		bool memory = false
	);


	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~Direct12Texture();


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
	ID3D12DescriptorHeap* GetHeap() { return m_heap; }

private:
	D3D12_ROOT_PARAMETER	m_rootParam;
	D3D12_DESCRIPTOR_RANGE	m_range;

	ID3D12Resource*			m_buffer;
	ID3D12DescriptorHeap*	m_heap;
};

