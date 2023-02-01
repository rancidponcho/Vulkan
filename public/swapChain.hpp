#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "global.hpp"

class tk_swapChain {
   public:
    void create(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, QueueFamilyIndices indices, GLFWwindow *window, VkDevice device);

    void destroy(VkDevice device);

    SwapChainSupportDetails querySupport(VkPhysicalDevice device, VkSurfaceKHR surface);

   private:
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    std::vector<VkImageView> swapChainImageViews;

    VkSurfaceFormatKHR selectSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availablePresentModes);

    VkPresentModeKHR selectPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

    VkExtent2D selectExtent(const VkSurfaceCapabilitiesKHR capabilities, GLFWwindow *window);

    void createImageViews(VkDevice device);

   public:
    VkSwapchainKHR get() { return swapChain; }
};