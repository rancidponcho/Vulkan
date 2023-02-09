#pragma once

#include <vulkan/vulkan.hpp>

class tk_physicalDevice;
class tk_logicalDevice;
class tk_surface;

class tk_commandPool {
   public:
    void create(tk_physicalDevice &physicalDevice, tk_surface &surface, tk_logicalDevice &device);
    void destroy(tk_logicalDevice &device);

   private:
    VkCommandPool commandPool;

   public:
    VkCommandPool get() { return commandPool; }
};