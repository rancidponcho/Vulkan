#include "../public/geometryBuffer.hpp"

#include <array>
#include <glm/glm.hpp>
#include <vector>

#include "../public/commandPool.hpp"
#include "../public/geometryData.hpp"
#include "../public/logicalDevice.hpp"
#include "../public/physicalDevice.hpp"

void tk_geometryBuffer::create(tk_logicalDevice &device, tk_physicalDevice &physicalDevice, tk_commandPool &commandPool) {
    size_t vertexDataSize = sizeof(vertices[0]) * vertices.size();
    size_t indexDataSize = sizeof(indices[0]) * indices.size();
    VkDeviceSize bufferSize = vertexDataSize + indexDataSize;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(device.get(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), vertexDataSize);
    memcpy(static_cast<char *>(data) + vertexDataSize, indices.data(), indexDataSize);
    vkUnmapMemory(device.get(), stagingBufferMemory);

    createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, geometryBuffer, geometryBufferMemory);

    copyBuffer(stagingBuffer, geometryBuffer, bufferSize, device, commandPool);

    vkDestroyBuffer(device.get(), stagingBuffer, nullptr);
    vkFreeMemory(device.get(), stagingBufferMemory, nullptr);
}

void tk_geometryBuffer::destroy(tk_logicalDevice &device) {
    vkDestroyBuffer(device.get(), geometryBuffer, nullptr);
    vkFreeMemory(device.get(), geometryBufferMemory, nullptr);
}

void tk_geometryBuffer::createBuffer(tk_logicalDevice &device, tk_physicalDevice &physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device.get(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device.get(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = physicalDevice.findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device.get(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(device.get(), buffer, bufferMemory, 0);
}

void tk_geometryBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, tk_logicalDevice &device, tk_commandPool &commandPool) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool.get();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device.get(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device.getGraphicsQueue());

    vkFreeCommandBuffers(device.get(), commandPool.get(), 1, &commandBuffer);
}
