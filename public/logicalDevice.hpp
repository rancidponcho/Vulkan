#pragma once

#include <vulkan/vulkan.hpp>

#include "physicalDevice.hpp"

class tk_logicalDevice {
   public:
    void create(tk_physicalDevice physicalDevice, VkSurfaceKHR surface);
    void destroy();

   private:
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

   public:
    VkDevice get() { return device; }
};