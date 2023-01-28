#pragma once

#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "global.hpp"
#include "swapChain.hpp"

class tk_physicalDevice {
   public:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    void select(VkInstance instance, VkSurfaceKHR surface, tk_swapChain swapChain);

    //  Only returns true for now (on integrated GPU). Some handy ideas at
    //  https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Physical_devices_queue_families
    bool isSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, tk_swapChain swapChain);
    bool checkExtensionSupport(VkPhysicalDevice device);

    // task > overload this to only take a surface
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    VkPhysicalDevice get() { return physicalDevice; }
};