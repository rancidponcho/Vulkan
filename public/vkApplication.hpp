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
#include "physicalDevice.hpp"
#include "swapChain.hpp"
#include "window.hpp"

//  lists of all vulkan validation layers & extensions
const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

class vkApplication {
   public:
    void run();

   private:
    VkInstance instance;
    tk_window window;
    VkSurfaceKHR surface;

    tk_debugMessenger debugMessenger;

    /*  Distinction between physical and logical device allows physical device extensions,
        queue counts, etc... whereas logical devices work as an interface  */
    tk_physicalDevice physicalDevice;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    tk_swapChain swapChain;

    // ===========================================================

    // ===========================================================

    void initVulkan();
    void mainLoop();
    void cleanup();
    void createInstance();

    bool checkValidationLayerSupport();

    std::vector<const char *> getRequiredExtensions();

    void createLogicalDevice();

    void createSurface();

    // ================================================================================================== //
};
