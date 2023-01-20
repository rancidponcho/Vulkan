#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <typeinfo>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class vkApplication
{
public:
    void run();

private:
    GLFWwindow *window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanup();

    /*
        The general pattern that object creation function parameters is as follows:
            1) Pointer to struct with creation info. Struct provides Vk with application
               information for optimization on different devices.
            2) Pointer to custom allocator callbacks. Allocator callback (using nullptr).
            3) Pointer to the variable that stores the handle to the new object. Tell Vk
               driver which 'global' (applies to the ENTIRE program & not a specific device)
               extensions and validation layers to use.
    */
    void createInstance();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    void setupDebugMessenger();

    std::vector<const char *> getRequiredExtensions();

    bool checkValidationLayerSupport();

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
};