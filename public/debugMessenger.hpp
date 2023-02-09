#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

#include "global.hpp"

class tk_instance;

class tk_debugMessenger {
   public:
    void setup(tk_instance &instance);
    void destroy(tk_instance &instance, const VkAllocationCallbacks *pAllocator);

    void populateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

   private:
    VkDebugUtilsMessengerEXT debugMessenger;

    /*  Callback function populateCreateInfo() points to.
    NOTE: VKAPI_ATTR and VKAPI_CALL ensure it has the right signature for Vk to call it.   */
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

    //  Proxy functions for vkCreateDebugUtilsMessengerEXT & eqiuvalent destroy method
    VkResult create(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
};