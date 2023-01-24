/*  Class desperately needs reordering  */
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include <stdexcept>
#include <typeinfo>
#include <vector>

#include "structs.hpp"

const uint32_t WIDTH{800}, HEIGHT{600};

//  lists of all vulkan validation layers & extensions
const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class vkApplication {
   public:
    void run();

   private:
    GLFWwindow *window;
    VkInstance instance;

    VkDebugUtilsMessengerEXT debugMessenger;

    VkSurfaceKHR surface;

    /*  Distinction between physical and logical device allows physical device extensions,
        queue counts, etc... whereas logical devices work as an interface  */
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

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
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    //  Populates CreateDebugUtilsMessengerEXT() with output from method below.
    void setupDebugMessenger();
    //  Proxy function for vkCreateDebugUtilsMessengerEXT & cdestoy method
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);
    /*  Callback function populateDebugMessengerCreateInfo() points to. It can filter messages
        through if-comparison to messenger parameter enumerations.
        NOTE: VKAPI_ATTR and VKAPI_CALL ensure it has the right signature for Vk to call it.   */
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
    //  Enumerates over available physical devices and chooses based on isDeviceSuitable()
    void pickPhysicalDevice();
    //  Only returns true for now (on integrated GPU). Some handy ideas at
    //  https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Physical_devices_queue_families
    bool isDeviceSuitable(VkPhysicalDevice device);
    // RHI
    void createLogicalDevice();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    void createSurface();

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    void createSwapChain();
};
