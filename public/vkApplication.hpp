#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <typeinfo>
#include <optional>

const uint32_t WIDTH{800}, HEIGHT{600};

// list of all available validation layers
const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

/*  Stores GPU's available queue families  */
struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value(); // optional::has_value() indicates existence of variable.
    }
};

class vkApplication
{
public:
    void run();

private:
    GLFWwindow *window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    /*  Physical and logical device distinction because physical device extensions,
        queue counts, etc... must be asked to the physical device, whereas logical devices
        work as an interface  */
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    void initWindow();
    void initVulkan();
    /*  Vulkan OBJECT CREATION requires passing parameters to initialization structs.
        The general pattern is:
        1)  Pointer to struct with creation info. Struct provides Vk with application
            information for optimization on different devices.
        2)  Pointer to custom allocator callbacks (using nullptr).
        3)  Pointer to the variable that stores the handle to the new object. Tell Vk
            driver which 'global' (applies to the ENTIRE program & not a specific device)
            extensions and validation layers to use.  */
    void createInstance();
    /*  Finds which validation layers are supported by the machine.  */
    bool checkValidationLayerSupport();
    /*  Returns list of GLFW required extensions & others manually specified.  */
    std::vector<const char *> getRequiredExtensions();
    /*  Generates debug messenger parameters and sets the callback function.
        Severity:
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT  --  Diagnostic
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT     --  Informational, like te creation of a resource
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT  --  Behavior that is not necessarily an error, but very likely a bug
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT    --  Behavior that is invalid and may cause crashes
        Type:
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT      --  Event unrealated to specification or performance has happened
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT   --  Something violates specification or indicates possible mistake
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT  --  Potential non-optimal use
        pfnUserCallback specifies the pointer to the callback function  */
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    /*  Populates CreateDebugUtilsMessengerEXT() with output from function above.  */
    void setupDebugMessenger();
    /*  Proxy function for vkCreateDebugUtilsMessengerEXT */
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger);

    /*  Enumerates over available physical devices and chooses based on isDeviceSuitable()  */
    void pickPhysicalDevice();
    /*  Only returns true for now (on integrated GPU). Some handy ideas at https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Physical_devices_queue_families  */
    bool isDeviceSuitable(VkPhysicalDevice device);
    /*  Finds and stores queue families  */
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    void mainLoop();

    void cleanup();
    /*  ...and subsequently destroyed on cleanup.  */
    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator);

    // ================================================================================================== //

    /*  Callback function populateDebugMessengerCreateInfo() points to. It can filter
        messages through if-comparison to messenger parameter enumerations.
        NOTE: VKAPI_ATTR and VKAPI_CALL ensure it has the right signature for Vk to call it.   */
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                        void *pUserData);
};