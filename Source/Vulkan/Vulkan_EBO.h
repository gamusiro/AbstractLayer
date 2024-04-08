/*===================================================================================
*    Date : 2022/11/28(月)
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
	/// \brief コンストラクタ
	/// 
	/// \param[in] size
	/// \param[in] countIndex
	/// \param[in] graphics
	/// 
	/// \return なし
	//**************************************************
	VulkanEBO(
		uint32_t size,
		unsigned int countIndex,
		VkIndexType type,
		VulkanGraphics* graphics
	);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~VulkanEBO();

	////////		継承メソッド群		////////
public:
	//**************************************************
	/// \brief マップ
	/// 
	/// \return true なら成功
	//**************************************************
	bool Map(void* data, size_t size) override;

	////////		Vulkanに依存するメソッド群		////////
public:
	//**************************************************
	/// \brief バッファの取得
	/// 
	/// \return VkBuffer
	//**************************************************
	VkBuffer GetBuffer() { return m_object.buffer; }

	//**************************************************
	/// \brief インデックスタイプの取得
	/// 
	/// \return VkIndexType
	//**************************************************
	VkIndexType GetType() { return m_type; }

private:
	graphics::BUFFER_OBJECT	m_object;
	VkIndexType				m_type;
	VulkanGraphics*			m_graphics;
};

