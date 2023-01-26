#pragma once

#include <iostream>
#include <optional>
#include <vector>
#include <vulkan/vulkan.hpp>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const uint32_t WIDTH{800}, HEIGHT{600};

//  lists of all vulkan validation layers & extensions
const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;       // Swap Extent (resolution of imgs in chain)
    std::vector<VkSurfaceFormatKHR> formats;     // Surface Format (pixel format, color space)
    std::vector<VkPresentModeKHR> presentModes;  // Presentation Modes (conditions for "swapping" imgs to the screen)
};