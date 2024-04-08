/*===================================================================================
*    Date : 2022/11/28(月)
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
	/// \brief コンストラクタ
	/// 
	/// \param[in] size
	/// \param[in] stride
	/// \param[in] graphics
	/// 
	/// \return なし
	//**************************************************
	VulkanVBO(
		uint32_t size,
		size_t stride,
		VulkanGraphics* graphics
	);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~VulkanVBO();

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

private:
	graphics::BUFFER_OBJECT	m_object;
	VulkanGraphics*			m_graphics;
};

