#include "core/ce_Buffer.hpp"

namespace ConfuseEngine {
    VkDeviceSize CE_Buffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
        if (minOffsetAlignment > 0) {
            return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
        }
        return instanceSize;
    }

    CE_Buffer::CE_Buffer(CE_Device &device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment) : m_rDevice{device}, m_instanceSize{instanceSize}, m_instanceCount{instanceCount}, m_usageFlags{usageFlags}, m_memoryPropertyFlags{memoryPropertyFlags} {
        m_alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
        m_bufferSize = m_alignmentSize * instanceCount;
        device.createBuffer(m_bufferSize, usageFlags, memoryPropertyFlags, m_buffer, m_memory);
    }

    CE_Buffer::~CE_Buffer() {
        unmap();
        vkDestroyBuffer(m_rDevice.device(), m_buffer, nullptr);
        vkFreeMemory(m_rDevice.device(), m_memory, nullptr);
    }

    VkResult CE_Buffer::map(VkDeviceSize size, VkDeviceSize offset) {
        assert(m_buffer && m_memory && "called map on buffer before create");
        return vkMapMemory(m_rDevice.device(), m_memory, offset, size, 0, &m_mapped);
    }

    void CE_Buffer::unmap() {
        if (m_mapped) {
            vkUnmapMemory(m_rDevice.device(), m_memory);
            m_mapped = nullptr;
        }
    }

    void CE_Buffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset) {
        assert(m_mapped && "cannot copy to unmapped buffer");

        if (size == VK_WHOLE_SIZE) {
            memcpy(m_mapped, data, m_bufferSize);
        }
        else {
            char *memOffset = (char *)m_mapped;
            memOffset += offset;
            memcpy(memOffset, data, size);
        }
    }

    VkResult CE_Buffer::flush(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = m_memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkFlushMappedMemoryRanges(m_rDevice.device(), 1, &mappedRange);
    }

    VkResult CE_Buffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = m_memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkInvalidateMappedMemoryRanges(m_rDevice.device(), 1, &mappedRange);
    }

    VkDescriptorBufferInfo CE_Buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
        return VkDescriptorBufferInfo{m_buffer, offset, size,};
    }

    void CE_Buffer::writeToIndex(void *data, int index) {
        writeToBuffer(data, m_instanceSize, index * m_alignmentSize);
    }

    VkResult CE_Buffer::flushIndex(int index) { return flush(m_alignmentSize, index * m_alignmentSize); }

    VkDescriptorBufferInfo CE_Buffer::descriptorInfoForIndex(int index) {
        return descriptorInfo(m_alignmentSize, index * m_alignmentSize);
    }

    VkResult CE_Buffer::invalidateIndex(int index) {
        return invalidate(m_alignmentSize, index * m_alignmentSize);
    }
}