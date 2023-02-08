#pragma once

#include <vulkan/vulkan.h>

class tk_logicalDevice;
class tk_swapChain;

class tk_renderPass {
   public:
    void create(tk_swapChain &swapChain, tk_logicalDevice &device);
    void destroy(tk_logicalDevice &device);

   private:
    VkRenderPass renderPass;

   public:
    VkRenderPass get() { return renderPass; }
};