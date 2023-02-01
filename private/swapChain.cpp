#include "../public/swapChain.hpp"

#include <algorithm>
#include <limits>

void tk_swapChain::create(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, QueueFamilyIndices indices, GLFWwindow *window, VkDevice device) {
    SwapChainSupportDetails swapChainSupport = querySupport(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat = selectSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = selectPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = selectExtent(swapChainSupport.capabilities, window);
    // Sticking to min means we may have to wait on driver operations before another image can be rendered to, hence +1
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    // Make sure not to exceed max # of imgs. 0 is a special value that means there is no maximum.
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;  // INPUT

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;                              // # of layers each img consists of. This is always 1 unless developing stereoscopic 3D application (VR).
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;  // Specifies swap operation. In this case, rendering directly to image (color attachment).

    uint32_t queueFamilyIndices[] = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()};
    // Specify how to handle swap chain images used across multiple queue families. Drawing on graphics queue, then submitting to presentation queue.
    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;  // Img can be used across multiple queue families without explicit ownership transfers.
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;  // Img is owned by one queue and must be explicitly transferred (best performance).
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;  // to specify no transformation, use current transformation
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;             // Specifies if alpha chennel should be used for blendin with other windows
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;  // disables rendering where app is clipped by other windows

    createInfo.oldSwapchain = VK_NULL_HANDLE;  // Check Vk-tut in case of invalid or unoptimized swap chain.

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to freate swap chain!");
    }
    // Retrieve handles
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

    // ============================
    createImageViews(device);
}

void tk_swapChain::destroy(VkDevice device) {
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);
}

SwapChainSupportDetails tk_swapChain::querySupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR tk_swapChain::selectSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availableFormats) {
    for (const auto &availableFormat : availableFormats) {
        // evaluate for BGRA 8bit unsigned integer format & sRGB color space
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    // Considering ranking system for other devices
    return availableFormats[0];
}

VkPresentModeKHR tk_swapChain::selectPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
    for (const auto &availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D tk_swapChain::selectExtent(const VkSurfaceCapabilitiesKHR capabilities, GLFWwindow *window) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)};

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}

void tk_swapChain::createImageViews(VkDevice device) {
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
        // components allows swizzling of color channels. Can map all channels to red, for example.
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        // subresourceRange describes image purpose and what parts should be accessed.
        // In this case, images will be used as color targets w/o mipmapping levels oor multiple layers
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;  // stereographic 3D would use one layer for each eye
        // creation
        if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
}