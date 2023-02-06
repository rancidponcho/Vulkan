/*  Class desperately needs reordering  */
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

#include "debugMessenger.hpp"
#include "global.hpp"
#include "graphicsPipeline.hpp"
#include "logicalDevice.hpp"
#include "physicalDevice.hpp"
#include "swapChain.hpp"
#include "window.hpp"

class vkApplication {
   public:
    void run();

   private:
    const uint32_t WIDTH{800}, HEIGHT{600};

    VkInstance instance;
    tk_window window;
    VkSurfaceKHR surface;

    tk_debugMessenger debugMessenger;

    /*  Distinction between physical and logical device allows physical device extensions,
        queue counts, etc... whereas logical devices work as an interface  */
    tk_physicalDevice physicalDevice;
    tk_logicalDevice device;

    tk_swapChain swapChain;

    tk_graphicsPipeline graphicsPipeline;

    void initVulkan();
    void mainLoop();
    void cleanup();
    // Instance should go in it's own class with extensions and validation layers
    void createInstance();

    bool checkValidationLayerSupport();

    std::vector<const char *> getRequiredExtensions();

    // own class, or window class?
    void createSurface();
};
