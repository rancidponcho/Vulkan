#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

class tk_commandPool;
class tk_logicalDevice;
class tk_renderPass;
class tk_frameBuffer;
class tk_swapChain;
class tk_graphicsPipeline;
class tk_geometryBuffer;

class tk_commandBuffers {
   public:
    void create(tk_commandPool &commandPool, tk_logicalDevice &device);
    // automatically destroyed when command pool is destroyed
    void record(uint32_t currentFrame, tk_swapChain &swapChain, tk_renderPass &renderPass, tk_frameBuffer &frameBuffer, tk_graphicsPipeline &graphicsPipeline, uint32_t imageIndex, tk_geometryBuffer &geometryBuffer);

   private:
    std::vector<VkCommandBuffer> commandBuffers;
    const int MAX_FRAMES_IN_FLIGHT = 2;

   public:
    VkCommandBuffer get(uint32_t currentFrame) { return commandBuffers[currentFrame]; }
    VkCommandBuffer *getPtr(uint32_t currentFrame) { return &commandBuffers[currentFrame]; }
};