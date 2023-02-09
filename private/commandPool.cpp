#include "../public/commandPool.hpp"

#include "../public/logicalDevice.hpp"
#include "../public/physicalDevice.hpp"
#include "../public/surface.hpp"

void tk_commandPool::create(tk_physicalDevice &physicalDevice, tk_surface &surface, tk_logicalDevice &device) {
    QueueFamilyIndices queueFamilyIndices = physicalDevice.findQueueFamilies(surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(device.get(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void tk_commandPool::destroy(tk_logicalDevice &device) {
    vkDestroyCommandPool(device.get(), commandPool, nullptr);
}