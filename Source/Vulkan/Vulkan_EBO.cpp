/*===================================================================================
*    Date : 2022/11/28(ŒŽ)
*        Author		: Gakuto.S
*        File		: Vulkan_EBO.cpp
*        Detail		:
===================================================================================*/
#include "Vulkan_EBO.h"

VulkanEBO::VulkanEBO(uint32_t size, unsigned int countIndex, VkIndexType type, VulkanGraphics* graphics)
    :m_object(),
    m_graphics(graphics),
    m_type(type),
    IElementBufferObject(countIndex)
{
    m_object = m_graphics->createBufferObject(
        size,
        VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
    );
}

VulkanEBO::~VulkanEBO()
{
    m_graphics->destroyBufferObject(m_object);
}

bool VulkanEBO::Map(void* data, size_t size)
{
    return m_graphics->map(m_object.memory, data, size);
}
