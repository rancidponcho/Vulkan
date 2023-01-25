#pragma once

#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "global.hpp"
#include "swapChain.hpp"

const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

class tk_physicalDevice {
   public:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    void select(VkInstance instance, VkSurfaceKHR surface, tk_swapChain swapChain);

    //  Only returns true for now (on integrated GPU). Some handy ideas at
    //  https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Physical_devices_queue_families
    bool isSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, tk_swapChain swapChain);
    bool checkExtensionSupport(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkSurfaceKHR surface);

    VkPhysicalDevice get() { return physicalDevice; }
};