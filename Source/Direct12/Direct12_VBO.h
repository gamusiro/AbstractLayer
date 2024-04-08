/*===================================================================================
*    Date : 2022/12/04(��)
*        Author		: Gakuto.S
*        File		: Direct12_VBO.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Direct12_Graphics.h"
#include "../IF_VertexBufferObject.h"

class Direct12VBO : public IVertexBufferObject
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \param[in] size
	/// \param[in] stride
	/// \param[in] graphics
	/// 
	/// \return �Ȃ�
	//**************************************************
	Direct12VBO(
		UINT size,
		size_t stride,
		Direct12Graphics* graphics
	);

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~Direct12VBO();

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
	/// \brief �o�b�t�@�r���[�̎擾
	/// 
	/// \return �o�b�t�@�r���[
	//**************************************************
	D3D12_VERTEX_BUFFER_VIEW GetBufferView() { return m_view; }

private:
	ID3D12Resource*				m_buffer;
	D3D12_VERTEX_BUFFER_VIEW	m_view;
};

