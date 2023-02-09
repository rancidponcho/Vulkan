#pragma once

#include <vulkan/vulkan.hpp>

class tk_commandPool;
class tk_logicalDevice;
class tk_renderPass;
class tk_frameBuffer;
class tk_swapChain;
class tk_graphicsPipeline;

class tk_commandBuffer {
   public:
    void create(tk_commandPool &commandPool, tk_logicalDevice &device);
    // automatically destroyed when command pool is destroyed
    void record(tk_swapChain &swapChain, tk_renderPass &renderPass, tk_frameBuffer &frameBuffer, tk_graphicsPipeline &graphicsPipeline, uint32_t imageIndex);

   private:
    VkCommandBuffer commandBuffer;
};