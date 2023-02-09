#pragma once

#include <vulkan/vulkan.hpp>

class tk_physicalDevice;
class tk_surface;

class tk_logicalDevice {
   public:
    void create(tk_physicalDevice &physicalDevice, tk_surface &surface);
    void destroy();

   private:
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

   public:
    VkDevice get() { return device; }
};