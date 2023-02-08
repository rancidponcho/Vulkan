#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

#include "../public/logicalDevice.hpp"
#include "../public/renderPass.hpp"
#include "../public/swapChain.hpp"

class tk_frameBuffer {
   public:
    void create(tk_swapChain swapChain, tk_renderPass renderPass, tk_logicalDevice device);
    void destroy(tk_logicalDevice device);

   private:
    std::vector<VkFramebuffer> swapChainFramebuffers;
};