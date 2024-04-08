/*===================================================================================
*    Date : 2022/11/28(��)
*        Author		: Gakuto.S
*        File		: Vulkan_VBO.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Vulkan_Graphics.h"
#include "../IF_BufferObject.h"

class VulkanVBO : public IVertexBufferObject
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
	VulkanVBO(
		uint32_t size,
		size_t stride,
		VulkanGraphics* graphics
	);

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~VulkanVBO();

	////////		�p�����\�b�h�Q		////////
public:
	//**************************************************
	/// \brief �}�b�v
	/// 
	/// \return true �Ȃ琬��
	//**************************************************
	bool Map(void* data, size_t size) override;

	////////		Vulkan�Ɉˑ����郁�\�b�h�Q		////////
public:
	//**************************************************
	/// \brief �o�b�t�@�̎擾
	/// 
	/// \return VkBuffer
	//**************************************************
	VkBuffer GetBuffer() { return m_object.buffer; }

private:
	graphics::BUFFER_OBJECT	m_object;
	VulkanGraphics*			m_graphics;
};

