#pragma once

#include <vulkan/vulkan.hpp>

#include "global.hpp"

class tk_swapChain;

class tk_physicalDevice {
   public:
    void select(VkInstance instance, VkSurfaceKHR surface, tk_swapChain &swapChain);

    QueueFamilyIndices findQueueFamilies(VkSurfaceKHR surface);

   private:
    bool isSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, tk_swapChain &swapChain);

    bool checkExtensionSupport(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

   public:
    VkPhysicalDevice get() { return physicalDevice; }
};