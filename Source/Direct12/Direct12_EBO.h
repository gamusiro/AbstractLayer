/*===================================================================================
*    Date : 2022/12/04(��)
*        Author		: Gakuto.S
*        File		: Direct12_EBO.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Direct12_Graphics.h"
#include "../IF_ElementBufferObject.h"

class Direct12EBO : public IElementBufferObject
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \param[in] size
	/// \param[in] countIndex
	/// \param[in] format
	/// \param[in] graphics
	/// 
	/// \return �Ȃ�
	//**************************************************
	Direct12EBO(
		UINT size,
		unsigned int countIndex,
		DXGI_FORMAT format,
		Direct12Graphics* graphics
	);

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~Direct12EBO();

	////////		�p�����\�b�h�Q		////////
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
	/// \brief �o�b�t�@�̎擾
	/// 
	/// \return VkBuffer
	//**************************************************
	D3D12_INDEX_BUFFER_VIEW GetBufferView() { return m_view; }

private:
	ID3D12Resource*			m_buffer;
	D3D12_INDEX_BUFFER_VIEW	m_view;
};

