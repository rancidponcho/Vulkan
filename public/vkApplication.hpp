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
#include <typeinfo>
#include <vector>

#include "debugMessenger.hpp"
#include "global.hpp"
#include "swapChain.hpp"

const uint32_t WIDTH{800}, HEIGHT{600};

//  lists of all vulkan validation layers & extensions
const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

class vkApplication {
   public:
    void run();

   private:
    GLFWwindow *window;
    VkInstance instance;

    VkSurfaceKHR surface;

    tk_debugMessenger debugMessenger;

    /*  Distinction between physical and logical device allows physical device extensions,
        queue counts, etc... whereas logical devices work as an interface  */
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    tk_swapChain swapChain;

    // ===========================================================

    // ===========================================================

    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();
    void createInstance();

    //  Finds which validation layers are supported by the machine.
    bool checkValidationLayerSupport();

    //  Returns list of GLFW required extensions & others manually specified.
    std::vector<const char *> getRequiredExtensions();

    //  Generates debug messenger parameters and sets the callback function.
    void pickPhysicalDevice();

    //  Only returns true for now (on integrated GPU). Some handy ideas at
    //  https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Physical_devices_queue_families
    bool isDeviceSuitable(VkPhysicalDevice device);

    // RHI
    void createLogicalDevice();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    void createSurface();

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    // ================================================================================================== //
};
