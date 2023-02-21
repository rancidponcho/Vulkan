#pragma once

#include <vulkan/vulkan.hpp>

class tk_logicalDevice;
class tk_physicalDevice;
class tk_commandPool;

class tk_geometryBuffer {
   public:
    void create(tk_logicalDevice &device, tk_physicalDevice &physicalDevice, tk_commandPool &commandPool);
    void destroy(tk_logicalDevice &device);

   private:
    VkBuffer geometryBuffer;
    VkDeviceMemory geometryBufferMemory;

    void createBuffer(tk_logicalDevice &device, tk_physicalDevice &physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, tk_logicalDevice &device, tk_commandPool &commandPool);

   public:
    VkBuffer get() { return geometryBuffer; }
};