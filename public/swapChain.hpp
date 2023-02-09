#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "global.hpp"

class tk_physicalDevice;
class tk_logicalDevice;
class tk_window;
class tk_surface;

class tk_swapChain {
   public:
    void create(tk_surface &surface, tk_physicalDevice &physicalDevice, tk_logicalDevice &device, tk_window &window);
    void destroy(tk_logicalDevice &device);

    // TAKES VK TYPES
    SwapChainSupportDetails querySupport(VkPhysicalDevice device, VkSurfaceKHR surface);

   private:
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;  // this should be moved to image views class
    VkExtent2D swapChainExtent;

    std::vector<VkImageView> swapChainImageViews;

    VkSurfaceFormatKHR selectSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availablePresentModes);

    VkPresentModeKHR selectPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

    VkExtent2D selectExtent(const VkSurfaceCapabilitiesKHR capabilities, GLFWwindow *window);

    void createImageViews(VkDevice device);

   public:
    VkSwapchainKHR get() { return swapChain; }
    VkExtent2D getExtent() { return swapChainExtent; }
    VkFormat getImageFormat() { return swapChainImageFormat; }
    std::vector<VkImageView> getImageViews() { return swapChainImageViews; }
};