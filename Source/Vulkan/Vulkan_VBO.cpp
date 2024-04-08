/*===================================================================================
*    Date : 2022/11/28(ŒŽ)
*        Author		: Gakuto.S
*        File		: Vulkan_VBO.cpp
*        Detail		:
===================================================================================*/
#include "Vulkan_VBO.h"

VulkanVBO::VulkanVBO(uint32_t size, size_t stride, VulkanGraphics* graphics)
    :m_object(),
    m_graphics(graphics),
    IVertexBufferObject(stride)
{
    m_object = m_graphics->createBufferObject(
        size,
        VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
    );
}

VulkanVBO::~VulkanVBO()
{
    m_graphics->destroyBufferObject(m_object);
}

bool VulkanVBO::Map(void* data, size_t size)
{
    return m_graphics->map(m_object.memory, data, size);
}
