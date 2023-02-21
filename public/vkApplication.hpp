#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

#include "commandBuffers.hpp"
#include "commandPool.hpp"
#include "debugMessenger.hpp"
#include "frameBuffer.hpp"
#include "geometryBuffer.hpp"
#include "global.hpp"
#include "graphicsPipeline.hpp"
#include "instance.hpp"
#include "logicalDevice.hpp"
#include "physicalDevice.hpp"
#include "renderPass.hpp"
#include "surface.hpp"
#include "swapChain.hpp"
#include "window.hpp"

class vkApplication {
   public:
    void run();

    bool framebufferResized = false;

   private:
    const uint32_t WIDTH{800}, HEIGHT{600};
    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t currentFrame = 0;

    tk_instance instance;
    tk_window window;
    tk_surface surface;
    tk_debugMessenger debugMessenger;
    tk_physicalDevice physicalDevice;
    tk_logicalDevice device;
    tk_swapChain swapChain;
    tk_renderPass renderPass;
    tk_graphicsPipeline graphicsPipeline;
    tk_frameBuffer frameBuffer;
    tk_commandPool commandPool;
    tk_commandBuffers commandBuffers;
    tk_geometryBuffer geometryBuffer;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    void initVulkan();
    void mainLoop();
    void cleanup();

    void drawFrame();
    void createSyncObjects();

    void recreateSwapChain();
};