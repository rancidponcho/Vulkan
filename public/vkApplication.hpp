#pragma once

#include "commandBuffer.hpp"
#include "commandPool.hpp"
#include "debugMessenger.hpp"
#include "frameBuffer.hpp"
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

   private:
    const uint32_t WIDTH{800}, HEIGHT{600};

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
    tk_commandBuffer commandBuffer;

    void initVulkan();
    void mainLoop();
    void cleanup();
};