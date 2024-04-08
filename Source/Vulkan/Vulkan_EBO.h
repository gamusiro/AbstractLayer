/*===================================================================================
*    Date : 2022/11/28(��)
*        Author		: Gakuto.S
*        File		: Vulkan_EBO.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Vulkan_Graphics.h"
#include "../IF_ElementBufferObject.h"

class VulkanEBO : public IElementBufferObject
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \param[in] size
	/// \param[in] countIndex
	/// \param[in] graphics
	/// 
	/// \return �Ȃ�
	//**************************************************
	VulkanEBO(
		uint32_t size,
		unsigned int countIndex,
		VkIndexType type,
		VulkanGraphics* graphics
	);

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	~VulkanEBO();

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

	//**************************************************
	/// \brief �C���f�b�N�X�^�C�v�̎擾
	/// 
	/// \return VkIndexType
	//**************************************************
	VkIndexType GetType() { return m_type; }

private:
	graphics::BUFFER_OBJECT	m_object;
	VkIndexType				m_type;
	VulkanGraphics*			m_graphics;
};

